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

    public delegate void GrabberStateEvent(GrabberState state, string info);
    public delegate void GrabberErrorEvent(string error);
    public class GrabberDispatcher : IDisposable
    {
        public GrabberDispatcher(TRWrapper wrapper)
        {
            this.wrapper = wrapper;

            wrapper.grabberDispatcherState += new _ITRWrapperEvents_grabberDispatcherStateEventHandler(Grabber_StateChanged);
            wrapper.grabberDispatcherError += new _ITRWrapperEvents_grabberDispatcherErrorEventHandler(Grabber_ErrorAquire);

            wrapper.SetMaxFramesInIRBFile(Properties.Settings.Default.maxFramesInFile);

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


        public event GrabberStateEvent GrabberStateChanged;
        public event GrabberErrorEvent GrabberErrorAquired;

        private TRWrapper wrapper;

        public void Grabber_StateChanged(byte state)
        {
            if (GrabberStateChanged != null)
                GrabberStateChanged((GrabberState)state, "");

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
            wrapper.StopGrabbing(0,0);
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

        public void setRecordedGrabbedFramesPath(string path)
        {
            wrapper.SetGrabberPath(path);
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
