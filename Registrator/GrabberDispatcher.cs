using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using ThermoRoutineLib;

namespace Registrator
{

    using NewFrameGrabbedEventHandler = _ITRWrapperEvents_FrameFromCamEventHandler;

    public enum GrabberState
    {
        NONE = 0,
        INITED = 1,
        SRC_CONNECTED = 2,
        GRABBING = 3,
        ERROR = 4
    }

    public delegate void GrabberStateEvent(GrabberState state,bool stop_grabbing, string info);
    public delegate void GrabberErrorEvent(string error);
    public class GrabberDispatcher : IDisposable
    {
        public GrabberDispatcher(TRWrapper wrapper, int camShift)
        {
            stop_grabbing_requested_ = false;
            _settings = new common_settings();
            this.wrapper = wrapper;

            wrapper.grabberDispatcherState += new _ITRWrapperEvents_grabberDispatcherStateEventHandler(Grabber_StateChanged);
            wrapper.grabberDispatcherError += new _ITRWrapperEvents_grabberDispatcherErrorEventHandler(Grabber_ErrorAquire);

            wrapper.SetMaxFramesInIRBFile(_settings.max_frames_in_file);
            wrapper.SetCameraOffset(camShift);

        }
        ~GrabberDispatcher()
        {
            Properties.Settings.Default.maxFramesInFile = Properties.Settings.Default.maxFramesInFile;
            Properties.Settings.Default.Save();
            Dispose();
        }

        public void Dispose()
        {
            if (wrapper != null)
            {
                stopRecording();
                wrapper.grabberDispatcherState -= new _ITRWrapperEvents_grabberDispatcherStateEventHandler(Grabber_StateChanged);
                wrapper.grabberDispatcherError -= new _ITRWrapperEvents_grabberDispatcherErrorEventHandler(Grabber_ErrorAquire);
                wrapper = null;
            }
        }


        private common_settings _settings;

        public event GrabberStateEvent GrabberStateChanged;
        public event GrabberErrorEvent GrabberErrorAquired;

        private TRWrapper wrapper;
        private bool stop_grabbing_requested_;

        public bool stop_grabbing_requested { get { return stop_grabbing_requested_; } }
        public void Grabber_StateChanged(byte state, bool stop_grabbing)
        {
            if (GrabberStateChanged != null)
                GrabberStateChanged((GrabberState)state,stop_grabbing, "");

        }
        public void Grabber_ErrorAquire(string error)
        {
            if (GrabberErrorAquired != null)
                GrabberErrorAquired(error);
        }

        public bool connectToSourceById(byte srcId)
        {
            return wrapper.ConnectCamera((byte)srcId);
        }

        public bool disconnectFromSourceById(byte srcId)
        {
            return wrapper.DisconnectCamera();
        }

        public bool startGrabbing()
        {
            wrapper.StartGrabbing();
            return true;
        }

        public bool stopGrabbing()
        {
            stop_grabbing_requested_ = true;
            wrapper.stopGrabbing(0,0);
            stop_grabbing_requested_ = false;
            return true;
        }

        public bool startRecording()
        {
            wrapper.StartRecord();
            return true;
        }

        public bool stopRecording()
        {
            wrapper.StopRecord(0, 0);
            return true;
        }

        public void FlushLastFrames()
        {
            wrapper.FlushGrabbedFramesToTmpFile();
        }

        public void setRecordedGrabbedFramesPath(string path)
        {
            wrapper.SetGrabberPath(path);
        }

        public void SendCommand(string command)
        {
            wrapper.SendCommandToCamera(command);
        }

       public bool GetGrabberSources(out Array sources_list)
        {
            wrapper.GetGrabberSources(out sources_list);
            return true;
        }

       public void connectToNewFrameEvent(NewFrameGrabbedEventHandler sink)
       {
           wrapper.FrameFromCam += new _ITRWrapperEvents_FrameFromCamEventHandler(sink);
       }
       public void disconnectFromNewFrameEvent(NewFrameGrabbedEventHandler sink)
       {
           wrapper.FrameFromCam -= new _ITRWrapperEvents_FrameFromCamEventHandler(sink);
       }

    }
}
