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

        static class Constants
        {
            public const int wait_new_frame_event_timeout = 100;

        }
 
        private void startShowGrabbingFrames()
        {
            run_routine_in_com_apartment(showGrabbingFramesLoop);
        }
        private void stopShowGrabbingFrames()
        {
            if (_camera_state == CameraState.GRAB ||
                _camera_state == CameraState.PREVIEW_RECORD || 
                _camera_state == CameraState.RECORD
                )
            {
                stopGrabbingLoop();
            }
        }

        protected irb_frame_helper _camera_frame;
        private uint _current_camera_frame_id = 0;

        private void showGrabbingFramesLoop(stopRequestedPredicate stopRequestedFunc)
        {

            while (!Created && !stopRequestedFunc())
                Thread.Sleep(200);

            if (_camera_state == CameraState.NONE || stopRequestedFunc())
            {
                return;
            }

            connect_playerCtrl_Canvas_MouseEvents();
            irb_frame_helper _camera_frame = new irb_frame_helper();
            _irb_frame_info frame_info = new _irb_frame_info();

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



                    if (res)
                    {
                        _current_camera_frame_id = frame_id;
                        _camera_frame.header.width = frame_info.image_info.width;
                        _camera_frame.header.height = frame_info.image_info.height;
                        _camera_frame.header.calibration_min = frame_info.measure.calibration_min;
                        _camera_frame.header.calibration_max = frame_info.measure.calibration_max;


                        if (frame_info.image_info.width == 1024) SetPlayerControlImage((byte[])raster, 1024, 768);
                        else SetPlayerControlImage((byte[])raster, 640, 480);

                        cur_coord = (long)frame_info.coordinate.coordinate;// +frame_info.coordinate.camera_offset;

                        _measure = new CTemperatureMeasure(frame_info.measure.tmin, frame_info.measure.tmax, frame_info.measure.tavr,
                            frame_info.measure.object_tmin, frame_info.measure.object_tmax, 0,                            
                            _camera_frame.header.calibration_min, _camera_frame.header.calibration_max);

                        SetThermoScaleLimits(_measure);

                        var args = new object[] { _measure };
                        Invoke(new SetTemperatureMeasureDelegate(SetTemperatureMeasure), args);

                        Invoke(new SetTimeDelegate(SetTime), new object[] { frame_info.timestamp });
                        Invoke(new SetIRBFramePositionDelegate(SetIRBFramePosition), new object[] { frame_info.coordinate.line, frame_info.coordinate.path_name, cur_coord, frame_info.coordinate.picket, frame_info.coordinate.offset, frame_info.coordinate.counter });

                        if (_is_cursor_position_valid)
                            get_cursor_point_temperature();

                        _max_T_point = frame_info.max_T_point;
                        _min_T_point = frame_info.min_T_point;

                        display_min_max_point_temperature(frame_info.max_T_point, frame_info.min_T_point, _measure);

                        if (m_areasPanel != null && m_areasPanel.Template != null && m_areasPanel.Template.Areas != null)
                        {
                            get_areas_temperature_measure();
                        }
                        //------------------------------------------------------- PROCESS EQUIPMENT ------------------------------------------------------------
                        if (equipmentMonitor != null)
                        {
                            Invoke(new EventHandler(delegate { equipmentMonitor.track_process(frame_info); }));
                        }

                        if (_RuntimeAlarmCtrl != null)
                            _RuntimeAlarmCtrl.ProcessIRBFrame(frame_id, frame_info);

                        //--------------------------------------------------------------------------------------------------------------------------------------
                    }
                    else
                    {
                        if(!_new_frame_event.WaitOne(Constants.wait_new_frame_event_timeout))
                        {
                            NLog.LogManager.GetCurrentClassLogger().Warn("Timed out waiting for new frame.");
                        }

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
