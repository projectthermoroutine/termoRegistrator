using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Registrator;
using System.Collections.Generic;

namespace RegistratorUnitTestsProject
{
    [TestClass]
    public class RegistratorTests
    {
        class objFrameInfo
        {
            public objFrameInfo()
            { objectId = 0; object_coordinate = 0; nearest_frame_index = 0; frame_coordinate = 0; frame_timestamp = 0.0; }
            public objFrameInfo(int objId, long obj_coordinate)
                : this()
            {
                objectId = objId; object_coordinate = obj_coordinate;
            }
            public int objectId;
            public long object_coordinate;
            public Int32 nearest_frame_index;
            public long frame_coordinate;
            public double frame_timestamp;
        }

        [TestMethod]
        public void ChoiceFrameForObjectAlgoritmTest()
        {
            ChoiceFrameObject choice_frames = new ChoiceFrameObject();
            choice_frames.SaveObjectFrameProcessHandler += save_object_termogramme;

            objFrameInfo[] _objs = {new objFrameInfo(0,0),
                                   new objFrameInfo(1,1),
                                   new objFrameInfo(2,2),
                                   new objFrameInfo(3,3)
                                   };
            //Assert.AreEqual
            List<objFrameInfo> objects = new List<objFrameInfo>(_objs);
            choice_frames.process_objects(objects,
            delegate(objFrameInfo obj, out int objId, out long obj_coord)
            {
                objId = obj.objectId;
                obj_coord = obj.object_coordinate;
            },
            0, 0, 0.0);

            choice_frames.close();

        }
        void save_object_termogramme(object sender, SaveObjectFrameProcessEvent arg)
        {
        }

    }
}
