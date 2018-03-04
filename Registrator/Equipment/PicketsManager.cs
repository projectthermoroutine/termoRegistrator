using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Windows.Documents;
using Registrator.DB.EFClasses;
using NLog;

namespace Registrator
{
    public class PicketsManager
    {
        DB.metro_db_controller _db_controller;
        public bool AddToLeft;
        private static readonly Logger logger = LogManager.GetCurrentClassLogger();

        public PicketsManager(DB.metro_db_controller db_controller)
        {
            _db_controller = db_controller;
            mPicketsList = new List<Picket>();
        }

        Picket FindFirstPicket(IQueryable<Picket> PicketsForSort)
        {
            bool whileIndex = true;
            Picket p = PicketsForSort.First();

            while (whileIndex)
            {
                whileIndex = false;
                foreach (var item in PicketsForSort)
                {
                    if (p.number == item.NpicketAfter)
                    {
                        p = item;
                        whileIndex = true;
                    }
                }

                if (p.NpicketBefore == 0) break;
            }
            return p;
        }

        List<Picket> mPicketsList;
        public List<Picket> PicketsList { get { return mPicketsList; } }

        public void createLogicalPicketList(EquPath path)
        {
            var _EquLine = path.Line;
            var _EquGroup = _EquLine.Group;
            var _EquClass = _EquGroup.Class;


            var pickets = _db_controller.dbContext.Pickets
                .Where(e => e.path == path.Code)
                .Distinct();

                //.Select(e => new EquPicket {  Code = e.number,
                //                              after = e.NpicketAfter,
                //                              before = e.NpicketBefore,
                //                              LeftLineShift = e.StartShiftLine,
                //                              RightLineShift = e.EndShiftLine,
                //                              lenght = e.Dlina,
                //                              Path = path,
                //                              keyNumber = e.number,
                //                              Name = e.Npiketa,
                //                              npicket = e.Npiketa,
                //                              picketTag =PicketTag.Old, Parent=path }).ToList().Distinct();
                
            //.Where(e => _db_controller.dbContext.Mains
                //            .Where(m => _db_controller.dbContext.Groups
                //                                .Where(g => g.Class == _EquClass.Code).Distinct().Select(g => g.Code).Contains(m.GroupId)).Select(g => g.EquipmentId).Contains(e.Code)).Select(e=>new EquPicket { Name = e, Code = e.  }).Distinct();


            //_db_controller.dbContext.Pickets.Where()


            //IEnumerable<EquPicket> pickets = (from r in _db_controller.pickets_table.AsEnumerable()
            //                                   where r.number != 0 && r.line == _EquLine.Code && r.path == path.Code && r.Group == _EquGroup.Code && r.Class == _EquClass.Code
            //                                   orderby r.Npiketa
            //                                   select new EquPicket(r.Npiketa, r.number, r.NpicketAfter, r.NpicketBefore, r.StartShiftLine, r.EndShiftLine, r.Dlina, path)).GroupBy(x => x.keyNumber).Select(g => g.First());

            mPicketsList.Clear();

            if (pickets.Count() == 0)
                return;

            Picket curPicket = FindFirstPicket(pickets);
            mPicketsList.Add(curPicket);

            bool whileIndex = true;
            while (whileIndex)
            {
                whileIndex = false;
                foreach (Picket item in pickets)
                {
                    if (curPicket.number == item.NpicketBefore)
                    {
                        curPicket = item;
                        mPicketsList.Add(item);
                        whileIndex = true;
                    }
                }
            }
        }
        public List<Picket> Matching(EquPath path_object)
        {
            var _EquLine  = path_object.Line;
            var _EquGroup = _EquLine.Group;
            var _EquClass = _EquGroup.Class;

            IEnumerable<Picket> IPicketsForMatching = null;

            if (IPicketsForMatching == null)
                throw new NotImplementedException("Matching");

            List<Picket> PicketsMatchingList = IPicketsForMatching.ToList();
            List<Picket> PicketsList         = new List<Picket>();

            for (int i = 0; i < mPicketsList.Count; i++)
            {
                foreach(Picket p in PicketsMatchingList)
                {
                    if (p.number == mPicketsList[i].number)
                    {
                        PicketsList.Add(mPicketsList[i]);
                        break;
                    }
                }
            }

            mPicketsList = PicketsList;
            return mPicketsList;
        }

        public Picket AddPicketToDB(    
            string addedPicketDisplayNum,
            int Class,
            int Group,
            int LineCode,
            int PathCode,
            int PickeID,
            int PicketLength)
        {
            Picket addingPicket = new Picket
            {
                Dlina = PicketLength,
                //p.picketTag = PicketTag.New;
                Npiketa = addedPicketDisplayNum,
                number = PickeID
            };

            //p.Name = "Пикет " + addedPicketDisplayNum.ToString();

            if (mPicketsList.Count == 0)
            {
                int picketNum = Convert.ToInt32(addedPicketDisplayNum);

                if (addedPicketDisplayNum[0] == '-') {
                    addingPicket.EndShiftLine = picketNum * PicketLength;
                    addingPicket.StartShiftLine = addingPicket.EndShiftLine - PicketLength;
                }
                else  {
                    addingPicket.StartShiftLine  = picketNum * PicketLength;
                    addingPicket.EndShiftLine = addingPicket.StartShiftLine + PicketLength;
                }


                addingPicket.NpicketBefore = 0;
                addingPicket.NpicketAfter = 0;

                mPicketsList.Insert(0, addingPicket);

                _db_controller.queriesAdapter.PicketCreateFirst(   addedPicketDisplayNum,
                                                                   PathCode,
                                                                   PickeID,
                                                                   PicketLength,
                                                                   mPicketsList[0].NpicketAfter,
                                                                   mPicketsList[0].NpicketBefore,
                                                                   mPicketsList[0].StartShiftLine,
                                                                   mPicketsList[0].EndShiftLine   );
                return mPicketsList[0];
            }

            if (AddToLeft)
            {

                //IQueryable<Picket> p_lst = _db_controller.dbContext.Pickets.Where(e => e.number == 0).Select(e=>e);

                //if (p_lst.Count() != 1)
                //    throw new Exception("Add picket failed. Database contains duplicate or corrupted");

                Picket after_picket = mPicketsList[0];

                addingPicket.StartShiftLine  = after_picket.StartShiftLine - PicketLength;
                addingPicket.EndShiftLine = after_picket.StartShiftLine;
                addingPicket.NpicketBefore = 0;
               
                addingPicket.NpicketAfter = after_picket.number;
                after_picket.NpicketBefore = PickeID;

                mPicketsList.Insert(0, addingPicket);

                var res =  _db_controller.queriesAdapter.PicketCreateLeft(  addedPicketDisplayNum,
                                                                            PathCode,
                                                                            PicketLength,
                                                                            PickeID,
                                                                            mPicketsList[0].StartShiftLine,
                                                                            mPicketsList[0].EndShiftLine,
                                                                            after_picket.number);
                return addingPicket;
            }
            else
            {
                mPicketsList.Add(addingPicket);
                mPicketsList.Last().NpicketAfter= 0;
                mPicketsList.Last().NpicketBefore = mPicketsList[mPicketsList.Count - 2].number;
                mPicketsList[mPicketsList.Count - 2].NpicketAfter= PickeID;

                mPicketsList.Last().StartShiftLine= mPicketsList[mPicketsList.Count - 2].EndShiftLine;
                mPicketsList.Last().EndShiftLine = mPicketsList.Last().StartShiftLine+ PicketLength;

                _db_controller.queriesAdapter.PicketCreateRight( addedPicketDisplayNum,
                                                                  PathCode,
                                                                  PicketLength,
                                                                  PickeID,
                                                                  mPicketsList.Last().NpicketBefore,
                                                                  mPicketsList.Last().StartShiftLine,
                                                                  mPicketsList.Last().EndShiftLine);
                return mPicketsList.Last();
            }
        }

        public void changePicketLength( int Length, DB.EFClasses.Picket picket)
        {
            int selPicket = mPicketsList.IndexOf(picket);

            if (selPicket != -1)
            {
                int delta = mPicketsList[selPicket].Dlina - Length;

                if (picket.Npiketa[0] == '-')
                {
                    var r = _db_controller.queriesAdapter.UpdateNegativePickets(picket.number, delta, Length);
                }
                else
                {
                    var r = _db_controller.queriesAdapter.UpdatePositivePickets(picket.number, delta, Length);
                }
            }
            else
            {
                logger.Error("changePicketLength function. Can not found picket.");
            }
        }
    }
}
