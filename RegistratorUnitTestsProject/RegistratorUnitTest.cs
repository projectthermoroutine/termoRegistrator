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

        List<int> _saved_object_ids;
        [TestMethod]
        public void ChoiceFrameForObjectAlgoritmTest()
        {
            ChoiceFrameObject choice_frames = new ChoiceFrameObject();
            choice_frames.SaveObjectFrameProcessHandler += save_object_termogramme;

            _saved_object_ids = new List<int>();
            objFrameInfo[] _objs = {new objFrameInfo(0,0),
                                   new objFrameInfo(1,-1*1000),
                                   new objFrameInfo(2,2*1000),
                                   new objFrameInfo(3,3*1000)
                                   };
            //Assert.AreEqual
            List<objFrameInfo> objects = new List<objFrameInfo>(_objs);


            for (int index = 0, coord = -10 * 1000; coord < 10 * 1000; index++, coord += 1000)
            {
                choice_frames.process_objects(objects,
                delegate(objFrameInfo obj, out int objId, out long obj_coord)
                {
                    objId = obj.objectId;
                    obj_coord = obj.object_coordinate;
                },
                (long)coord, (uint)index, 0.0);
            }
            choice_frames.close();
            Assert.AreEqual(_saved_object_ids.Count, _objs.Length);

        }
        void save_object_termogramme(object sender, SaveObjectFrameProcessEvent arg)
        {
            Assert.IsFalse(_saved_object_ids.Exists(id => arg.ObjectId == id));
            _saved_object_ids.Add(arg.ObjectId);
        }

    }
}
