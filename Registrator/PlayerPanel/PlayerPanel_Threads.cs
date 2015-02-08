using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI.Docking;
using System.Runtime.InteropServices;
using System.Threading;
using ThermoRoutineLib;
using System.Collections;
using DrawToolsLib;
using System.IO;
using System.Windows.Input;
using IRControls;
using System.Windows.Threading;
using System.Deployment.Application;
using System.Reflection;

namespace Registrator
{
    public partial class PlayerPanel
    {
        private void startShowGrabbingFrames()
        {
            run_routine_in_com_apartment(showGrabbingFramesLoop);
        }
        private void stopShowGrabbingFrames()
        {
            if (_camera_state == CameraState.GRAB || _camera_state == CameraState.RECORD)
            {
                stopGrabbingLoop();
            }
        }

        protected irb_frame_helper _camera_frame;

        private void showGrabbingFramesLoop(stopRequestedPredicate stopRequestedFunc)
        {

           


            if (_camera_state == CameraState.NONE || stopRequestedFunc())
            {
                return;
            }

            equipmentMonitor.ProcessEquipObj.refresh();
            equipmentMonitor.ProcessEquipObj.setLine(1);
            equipmentMonitor.ProcessEquipObj.tempCounter = 0;


            connect_playerCtrl_Canvas_MouseMove();
            irb_frame_helper _camera_frame = new irb_frame_helper();
            //object pixels = new ushort[1024 * 770];
            //object temp_values = new float[300];
            _irb_frame_info frame_info = new _irb_frame_info();

            object raster = new byte[1024 * 770 * 4];

            while (!stopRequestedFunc())
            {
                uint cur_frame_id = 0;
                bool res = false;
                try
                {
                    res = m_tvHandler.GetNextRealTimeFrameRaster(out cur_frame_id,
                                                    out frame_info,
                                                    ref raster
                                                    );

                    //res = m_tvHandler.GetNextRealTimeFrame(out cur_frame_id,
                    //                                out frame_info,
                    //                                ref pixels,
                    //                                ref temp_values
                    //                                );
                    if (!res)
                        continue;

                    if (res)
                    {
                        _camera_frame.reset_measure();
                        _camera_frame.header.width = frame_info.image_info.width;
                        _camera_frame.header.height = frame_info.image_info.height;
                        //_camera_frame.header.firstValidX = frame_info.image_info.firstValidX;
                        //_camera_frame.header.lastValidX = frame_info.image_info.lastValidX;
                        //_camera_frame.header.firstValidY = frame_info.image_info.firstValidY;
                        //_camera_frame.header.lastValidY = frame_info.image_info.lastValidY;
                        _camera_frame.header.calibration_min = frame_info.measure.calibration_min;
                        _camera_frame.header.calibration_max = frame_info.measure.calibration_max;

                        //_camera_frame.pixels = (ushort[])pixels;
                        //_camera_frame.temp_values = (float[])temp_values;

                        //byte[] raster;
                        //temp_interval calibration_interval = new temp_interval(0, 0);
                        //res = _image_helper.get_formated_frame_raster(_camera_frame, _grabber_areas_dispatcher, calibration_interval, out raster);

                        if (frame_info.image_info.width == 1024) SetPlayerControlImage((byte[])raster, 1024, 768);
                        else SetPlayerControlImage((byte[])raster, 640, 480);

                        var measure = new CTemperatureMeasure(frame_info.measure.tmin, frame_info.measure.tmax, frame_info.measure.tavr,
                            frame_info.measure.object_tmin, frame_info.measure.object_tmax, 0,                            
                            _camera_frame.header.calibration_min, _camera_frame.header.calibration_max);

                        SetThermoScaleLimits(measure);

                        var args = new object[] { measure };
                        Invoke(new SetTemperatureMeasureDelegate(SetTemperatureMeasure), args);
                        //   Invoke(new SetTemperatureCalibrationLimitsDelegate(SetTemperatureCalibrationLimits), args);

                        Invoke(new SetTimeDelegate(SetTime), new object[] { frame_info.timestamp });

                        if (m_areasPanel != null && m_areasPanel.Template != null && m_areasPanel.Template.Areas != null)
                        {
                            get_areas_temperature_measure();
                           // get_areas_temperature_measure2(_grabber_areas_dispatcher);
                        }


                        //------------------------------------------------------- PROCESS EQUIPMENT ------------------------------------------------------------
                        //equipmentMonitor.ProcessEquipObj.setLine(frame_info.coordinate.line);
                        equipmentMonitor.ProcessEquipObj.process(ref frame_info);
                        //--------------------------------------------------------------------------------------------------------------------------------------




                    }
                }
                catch (OutOfMemoryException)
                {
                    m_tvHandler.ClearGrabbingCache();
                }

            }//while (!stopRequestedFunc())
            disconnect_playerCtrl_Canvas_MouseMove();
            _camera_frame = null;

        }

    }
}
