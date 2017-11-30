﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace Registrator
{
    delegate void ComCreatePredicate();
    delegate void ComDeletePredicate();
    delegate void ComDispatcherPredicate(stopRequestedPredicate stopRequestedFunc);
    delegate bool stopRequestedPredicate();

    internal sealed class COM_dispatcher
    {
        private ComDispatcherPredicate _predicate;
        private ComDeletePredicate _delPred;
        private ComCreatePredicate _createPred;

        public COM_dispatcher(ComCreatePredicate createPred, ComDeletePredicate delPred)
        {
            _predicate = null;
            _delPred = delPred;
            _createPred = createPred;
            _is_object_created = false;
            _job_running = false;
            create_com_communication_thread();

        }

        void create_com_object()
        {
            _createPred();
        }
        ~COM_dispatcher()
        {
            CloseObject();
        }
        public void CloseObject()
        {
            _com_object_delete = true;
            _event.Set();
            _thread.Join();

            release_com_object();

        }
        public void DoPredicate(ComDispatcherPredicate predicate)
        {
            _predicate = predicate;
            if (_job_running)
                StopJob();
            _event.Set();
        }
        bool stop_requested()
        {
            return _stop_requested;
        }
        public void StopJob()
        {
            if (_job_running)
            {
                _stop_requested = true;
                while (_stop_requested)
                {
                    Thread.Sleep(500);
                    System.Windows.Forms.Application.DoEvents();
                }

            }
        }

        void release_com_object()
        {
            _delPred();
        }

        Thread _thread;
        AutoResetEvent _event;
        volatile bool _com_object_delete;
        bool _job_running;

        void create_com_communication_thread()
        {
            _event = new AutoResetEvent(false);
            _com_object_delete = false;
            _thread = new Thread(com_communication_thread);
            // _thread.ApartmentState = ApartmentState.STA;
            _thread.IsBackground = true;
            _thread.Start();

            create_com_object();

            while (!_is_object_created)
            {
                Thread.Sleep(200);
            }


        }
        private bool _is_object_created;
        volatile bool _stop_requested;
        private void com_communication_thread()
        {
            //create_com_object();
            _is_object_created = true;
            _job_running = false;
            _stop_requested = false;
            while (!_com_object_delete)
            {
                if (_predicate != null)
                {
                    _job_running = true;
                    _predicate(this.stop_requested);
                    _stop_requested = false;
                    _job_running = false;
                }

                _event.WaitOne();
            }
            // release_com_object();
            _is_object_created = false;
            _job_running = false;
        }

    }


    public sealed class filter_table
    {

        public filter_table()
        {
            _mask = new List<int>();
            _all_enabled = true;
            _all_disabled = false;
        }
        public int Size { get { return _mask.Count; } }
        public void Clear() { _all_enabled = false; _all_disabled = false; _mask.Clear(); }

        bool _all_enabled;
        bool _all_disabled;
        public bool all_enabled { get { return _all_enabled; } set { _all_enabled = value; if (value) { _all_disabled = false; _mask.Clear(); } } }
        public bool all_disabled { get { return _all_disabled; } set { _all_disabled = value; if (value) { _all_enabled = false; _mask.Clear(); } } }
        public bool is_filtered(ref int index)
        {
            if (all_enabled)
                return true;

            if (all_disabled)
                return false;

            if (_mask.Count == 0)
                return true;

            if (index >= _mask.Count)
                return false;

            index = _mask[index];
            return true;
        }

        List<int> _mask;

        public void set_filtered_index(int index)
        {
            int max_value = 0;
            var last_index = _mask.Count;
            if (last_index > 0)
                max_value = _mask[last_index - 1];

            if (index > max_value)
            {
                _mask.Add(index);
                return;
            }
        }

    }

}