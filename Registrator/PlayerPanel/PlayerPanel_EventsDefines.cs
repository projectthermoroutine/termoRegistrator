using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{
    using map_objects_list = List<measure_object>;

    public partial class PlayerPanel
    {
        //public event EventHandler<NeedToEraseTrackEvent> NeedToEraseTrackEventHandler;
        //public event EventHandler<FrameShotListenerStateChangeEvent> FrameShotListenerStateChangeEventHandler;
        public event EventHandler<FileFromGrabberEvent> FileFromGrabberEventHandler;
        public event EventHandler<RecModeChangeEvent> RecModeChangeEventHandler;

        public event EventHandler<ErrorEvent> ErrorHandler;
        public event EventHandler ResetErrorsHandler;

        public event EventHandler<PlayerStateEvent> PlayerStateEventHandler;
        public event EventHandler<EventPlayerChangeMode> EventHandlerChangeMode;

         public enum PlayerState
         {
             READY,
             BUSY
         }


         private void PlayerStateAquired(PlayerState state)
         {
            PlayerStateEventHandler?.Invoke(this, new PlayerStateEvent(state));
        }

        private void PlayerErrorAquired(ErrorEvent e)
        {
            ErrorHandler?.Invoke(this, e);
        }
        private void ResetPlayerErrors()
        {
            ResetErrorsHandler?.Invoke(this, new EventArgs());
        }

        private void FireChangeMode(EventPlayerChangeMode e)
        {
            EventHandlerChangeMode?.Invoke(this, e);
        }


     }

     public class EventPlayerChangeMode : EventArgs
     {
         private PlayerPanel.PlayerMode m_mode;
         public EventPlayerChangeMode(PlayerPanel.PlayerMode mode)
         {
             m_mode = mode;
         }
         public PlayerPanel.PlayerMode Mode { get { return m_mode; } }
     }


     public class PlayerStateEvent : EventArgs
     {

         public PlayerPanel.PlayerState state { get; private set; }

         public PlayerStateEvent(PlayerPanel.PlayerState state)
             : base()
         {
             this.state = state;
         }
     }



     public class ErrorEvent : EventArgs
     {

         public string ErrorText {get;private set;}

         public ErrorEvent(string error)
             : base()
         {
             ErrorText = error;
         }
     }


    public class FileFromGrabberEvent : EventArgs
    {
        String m_filePath = "";

        public FileFromGrabberEvent(String filePath = "")
            : base()
        {
            m_filePath = filePath;
        }

        public String FilePath { get { return m_filePath; } set { m_filePath = value; } }
    }

    public class RecModeChangeEvent : EventArgs
    {

        bool m_recModeActive = true;

        public RecModeChangeEvent(bool isRecModeActivated = true)
            : base()
        {
            m_recModeActive = isRecModeActivated;
        }

        public bool IsRecModeActivated { get { return m_recModeActive; } set { m_recModeActive = value; } }
    }

}
