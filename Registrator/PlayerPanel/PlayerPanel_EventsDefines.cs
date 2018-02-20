using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Registrator
{
    using map_objects_list = List<measure_object>;

    public partial class PlayerPanel
    {
       public event EventHandler<EquListLoadedEvent> EquListLoadedHandler;
        public event EventHandler<FrameChangedEvent> FrameChangedHandler;
        public event EventHandler<MapObjectsLoadedEvent> MapObjectsLoadedHandler;
        public event EventHandler<EditingFinishedEvent> EditingFinishedHandler;
        public event EventHandler<NeedToEraseTrackEvent> NeedToEraseTrackEventHandler;
        public event EventHandler<FrameShotListenerStateChangeEvent> FrameShotListenerStateChangeEventHandler;
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
             EventHandler<PlayerStateEvent> handler = PlayerStateEventHandler;
             if (handler != null)
                 handler(this, new PlayerStateEvent(state));
         }

        private void PlayerErrorAquired(ErrorEvent e)
        {
            EventHandler<ErrorEvent> handler = ErrorHandler;
            if (handler != null)
                handler(this, e);
        }
        private void ResetPlayerErrors()
        {
            EventHandler handler = ResetErrorsHandler;
            if (handler != null)
                handler(this, new EventArgs());
        }

        private void FireChangeMode(EventPlayerChangeMode e)
        {
            EventHandler<EventPlayerChangeMode> handler = EventHandlerChangeMode;

            if (handler != null)
            {
                handler(this, e);
            }
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


    public class FrameShotedEvent : EventArgs
    {

        ShotDesc m_shotDesc = null;

        public FrameShotedEvent(ShotDesc.ShotType shotType = ShotDesc.ShotType.SHOT_TYPE_USER, int fileNum = -1, int frameNum = -1)
            : base()
        {
            m_shotDesc = new ShotDesc();
            m_shotDesc.TypeOfShot = shotType;
            m_shotDesc.FileNum = fileNum;
            m_shotDesc.FrameNum = frameNum;
        }

        public FrameShotedEvent(ShotDesc shot)
            : base()
        {
            m_shotDesc = shot;
        }

        public ShotDesc Shot { get { return m_shotDesc; } set { m_shotDesc = value; } }
    }

    public class EquListLoadedEvent : EventArgs
    {
        map_objects_list _equipment_list = null;

        public EquListLoadedEvent(map_objects_list equipment_list)
            : base()
        {
            _equipment_list = equipment_list;
        }
        public map_objects_list EquipmentList { get { return _equipment_list; } }
    }

    public class FrameChangedEvent : EventArgs
    {
        private int m_index;
        private int m_distance;
        private int m_coord = 0;

        public FrameChangedEvent(int index, int distance, int coord)
            : base()
        {
            m_index = index;
            m_distance = distance;
            m_coord = coord;
        }

        public int Index { get { return m_index; } set { m_index = value; } }
        public int Distance { get { return m_distance; } set { m_distance = value; } }
        public int Coord { get { return m_coord; } set { m_coord = value; } }
    }

    public class NeedToEraseTrackEvent : EventArgs
    {
    }

    public class MapObjectsLoadedEvent : EventArgs
    {
        private map_objects_list m_mapObjects;
        private int m_pathLength = 0;

        public MapObjectsLoadedEvent(map_objects_list mapObjects, int pathLength)
            : base()
        {
            m_mapObjects = mapObjects;
            m_pathLength = pathLength;
        }

        public map_objects_list MapObjects { get { return m_mapObjects; } set { m_mapObjects = value; } }
        public int PathLength { get { return m_pathLength; } set { m_pathLength = value; } }
    }

    public class EditingFinishedEvent : EventArgs
    {

        private Area m_area;

        public EditingFinishedEvent()
            : base()
        {
        }
        public EditingFinishedEvent(Area area)
            : this()
        {
            m_area = area;
        }

        public Area Area { get { return m_area; } set { m_area = value; } }
    }

    public class FrameShotListenerStateChangeEvent : EventArgs
    {

        public enum StateChangeType
        {
            STATE_CHANGE_TYPE_ADD,
            STATE_CHANGE_TYPE_DEL
        }

        StateChangeType m_type = StateChangeType.STATE_CHANGE_TYPE_ADD;

        public FrameShotListenerStateChangeEvent()
            : base()
        {
        }

        public FrameShotListenerStateChangeEvent(StateChangeType type)
            : this()
        {
            m_type = type;
        }

        public StateChangeType Type { get { return m_type; } set { m_type = value; } }
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
