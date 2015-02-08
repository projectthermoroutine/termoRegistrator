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
            wrapper.StartRecieveCoordinates(
                            _position_detector_settings.pd_ip,
                            _position_detector_settings.pd_port,
                            _position_detector_settings.pd_events_ip,
                            _position_detector_settings.pd_events_port);

            _pd_state = true;

        }

        private position_detector_settings _position_detector_settings;
        private bool _pd_state;
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
            if (PD_ConnectionError != null)
                PD_ConnectionError(error);

        }

        public void PD_SettingsChanged(position_detector_settings pd_settings)
        {
            if (_position_detector_settings == pd_settings)
                return;

            _position_detector_settings = pd_settings;

            wrapper.StopRecieveCoordinates();
            _pd_state = false;
            wrapper.StartRecieveCoordinates(
                            _position_detector_settings.pd_ip,
                            _position_detector_settings.pd_port,
                            _position_detector_settings.pd_events_ip,
                            _position_detector_settings.pd_events_port);

            _pd_state = true;

        }


    }
}
