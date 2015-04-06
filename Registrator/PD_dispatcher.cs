using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using ThermoRoutineLib;

namespace Registrator
{
    public delegate void PD_ConnectionStateEvent(bool state, string info);
    public delegate void PD_ConnectionErrorEvent(string error);

    public class PD_dispatcher : IDisposable
    {
        public PD_dispatcher(TRWrapper wrapper)
        {
            _pd_state = false;
            this.wrapper = wrapper;

            _position_detector_settings = new position_detector_settings();

            _counter_size = Properties.Settings.Default.synchronizer_counter_size;

            wrapper.coordinatesDispatcherState += new _ITRWrapperEvents_coordinatesDispatcherStateEventHandler(PD_StateChanged);
            wrapper.coordinatesDispatcherError += new _ITRWrapperEvents_coordinatesDispatcherErrorEventHandler(PD_ErrorAquire);

        }
        ~PD_dispatcher()
        {
            Dispose();
        }

        public void Dispose()
        {
            _pd_state = false;
            if (wrapper != null)
            {
                wrapper.StopRecieveCoordinates();
                wrapper.coordinatesDispatcherState -= new _ITRWrapperEvents_coordinatesDispatcherStateEventHandler(PD_StateChanged);
                wrapper.coordinatesDispatcherError -= new _ITRWrapperEvents_coordinatesDispatcherErrorEventHandler(PD_ErrorAquire);
                wrapper = null;
            }
        }

        public void start()
        {
            if (_pd_state)
                return;
            _pd_state = true; 
            wrapper.StartRecieveCoordinates(
                            _position_detector_settings.pd_ip,
                            _position_detector_settings.pd_i_ip,
                            _position_detector_settings.pd_port,
                            _position_detector_settings.pd_events_ip,
                            _position_detector_settings.pd_i_events_ip,
                            _position_detector_settings.pd_events_port);

        }

        private position_detector_settings _position_detector_settings;
        private byte _counter_size;
        private bool _pd_state;
        private string _error;
        public string Error { get { return _error; } }
        public bool PD_State { get { return _pd_state; } }

        public event PD_ConnectionStateEvent PD_ConnectionStateChanged;
        public event PD_ConnectionErrorEvent PD_ConnectionError;

        private TRWrapper wrapper;
        public void PD_StateChanged(bool state)
        {
            _pd_state = state;
            if (PD_ConnectionStateChanged != null)
                PD_ConnectionStateChanged(state, "");

        }
        public void PD_ErrorAquire(string error)
        {
            _pd_state = false;
            _error = error;
            if (PD_ConnectionError != null)
                PD_ConnectionError(error);

        }

        public void PD_SettingsChanged(position_detector_settings pd_settings)
        {
            if (_position_detector_settings == pd_settings)
                return;

            _error = "";
            _position_detector_settings = pd_settings;

            _pd_state = false;
            wrapper.StopRecieveCoordinates();
            _pd_state = true;
            wrapper.StartRecieveCoordinates(
                            _position_detector_settings.pd_ip,
                            _position_detector_settings.pd_i_ip,
                            _position_detector_settings.pd_port,
                            _position_detector_settings.pd_events_ip,
                            _position_detector_settings.pd_i_events_ip,
                            _position_detector_settings.pd_events_port);

        }

        public void Sync_SettingsChanged(synchronizer_settings sync_settings)
        {
            if (_counter_size == sync_settings.counter_size)
                return;

            _counter_size = sync_settings.counter_size;

            wrapper.SetCounterSize(_counter_size);

        }

    }
}
