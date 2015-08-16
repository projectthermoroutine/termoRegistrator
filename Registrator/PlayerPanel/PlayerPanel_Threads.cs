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
        private uint _current_camera_frame_id = 0;
        private int curline;

        private void showGrabbingFramesLoop(stopRequestedPredicate stopRequestedFunc)
        {

            if (_camera_state == CameraState.NONE || stopRequestedFunc())
            {
                return;
            }

            if (equipmentMonitor != null)
            {
                equipmentMonitor.ProcessEquipObj.sampling_frequencies = Properties.Settings.Default.TrackHalfVeiwSector;
                equipmentMonitor.ProcessEquipObj.updateFreq = Properties.Settings.Default.TrackUpdateFrequency; 
                equipmentMonitor.ProcessEquipObj.refresh();
#if DEBUG 
                equipmentMonitor.ProcessEquipObj.refresh();
                equipmentMonitor.ProcessEquipObj.setLine(1,1);
                equipmentMonitor.ProcessEquipObj.mmCoordinate = 0;
                equipmentMonitor.ProcessEquipObj.direction = 0; 
#endif
            }

            connect_playerCtrl_Canvas_MouseEvents();
            irb_frame_helper _camera_frame = new irb_frame_helper();
            //object pixels = new ushort[1024 * 770];
            //object temp_values = new float[300];
            _irb_frame_info frame_info = new _irb_frame_info();

            //object raster = new byte[1024 * 770 * 4];
            object raster = new byte[640 * 480 * 4];
            _current_camera_frame_id = 0;
            uint frame_id = 0;

            long cur_coord = 0;
            while (!stopRequestedFunc())
            {
                bool res = false;
                try
                {
                    res = m_tvHandler.GetNextRealTimeFrameRaster(out frame_id,
                                                    out frame_info,
                                                    ref raster);

                    if (!res)
                        continue;

                    _current_camera_frame_id = frame_id;

                    if (res)
                    {
                       // _camera_frame.reset_measure();
                        _camera_frame.header.width = frame_info.image_info.width;
                        _camera_frame.header.height = frame_info.image_info.height;
                        _camera_frame.header.calibration_min = frame_info.measure.calibration_min;
                        _camera_frame.header.calibration_max = frame_info.measure.calibration_max;

                        //_camera_frame.temp_values = (float[])temp_values;

                        if (frame_info.image_info.width == 1024) SetPlayerControlImage((byte[])raster, 1024, 768);
                        else SetPlayerControlImage((byte[])raster, 640, 480);

                        cur_coord = (long)frame_info.coordinate.coordinate + frame_info.coordinate.camera_offset;

                        var measure = new CTemperatureMeasure(frame_info.measure.tmin, frame_info.measure.tmax, frame_info.measure.tavr,
                            frame_info.measure.object_tmin, frame_info.measure.object_tmax, 0,                            
                            _camera_frame.header.calibration_min, _camera_frame.header.calibration_max);

                        SetThermoScaleLimits(measure);

                        var args = new object[] { measure };
                        Invoke(new SetTemperatureMeasureDelegate(SetTemperatureMeasure), args);
                        //   Invoke(new SetTemperatureCalibrationLimitsDelegate(SetTemperatureCalibrationLimits), args);

                        Invoke(new SetTimeDelegate(SetTime), new object[] { frame_info.timestamp });
                        Invoke(new SetIRBFramePositionDelegate(SetIRBFramePosition), new object[] { cur_coord, frame_info.coordinate.picket, frame_info.coordinate.offset });

                        if (_is_cursor_position_valid)
                            get_cursor_point_temperature();

                        if (m_areasPanel != null && m_areasPanel.Template != null && m_areasPanel.Template.Areas != null)
                        {
                            get_areas_temperature_measure();
                           // get_areas_temperature_measure2(_grabber_areas_dispatcher);
                        }
                        //------------------------------------------------------- PROCESS EQUIPMENT ------------------------------------------------------------
                        if (!apply_camera_offset)
                        {
                            current_camera_offset = frame_info.coordinate.camera_offset;
                        }
                        equipmentMonitor.track_process(ref frame_info);
                        //--------------------------------------------------------------------------------------------------------------------------------------
                    }
                }
                catch (OutOfMemoryException)
                {
                    m_tvHandler.ClearGrabbingCache();
                }

            }//while (!stopRequestedFunc())

            _current_camera_frame_id = 0;
            disconnect_playerCtrl_Canvas_MouseEvents();
            _camera_frame = null;

        }

    }
}
