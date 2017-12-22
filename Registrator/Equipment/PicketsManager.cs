using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Windows.Documents;

namespace Registrator
{
    public class PicketsManager
    {
        DB.metro_db_controller _db_controller;
        public bool AddToLeft;
        public PicketsManager(DB.metro_db_controller db_controller)
        {
            _db_controller = db_controller;
            mPicketsList = new List<EquPicket>();
        }

        EquPicket findFirstPicket(IEnumerable<EquPicket> PicketsForSort)
        {
            bool whileIndex = true;
            EquPicket p = PicketsForSort.First();
            while (whileIndex)
            {
                whileIndex = false;
                foreach (var item in PicketsForSort)
                {
                    if (p.keyNumber == item.after)
                    {
                        p = item;
                        whileIndex = true;
                    }
                }
                if (p.before == 0) break;
            }
            return p;
        }

        List<EquPicket> mPicketsList;
        public List<EquPicket> PicketsList { get { return mPicketsList; } }

        public void createLogicalPicketList(EquPath path)
        {
            var _EquLine = path.Line;
            var _EquGroup = _EquLine.Group;
            var _EquClass = _EquGroup.Class;


            _db_controller.pickets_table.Clear();
            _db_controller.pickets_adapter.Fill(_db_controller.pickets_table);

            IEnumerable<EquPicket> _Pickets = (from r in _db_controller.pickets_table.AsEnumerable()
                                               where r.number != 0 && r.line == _EquLine.Code && r.path == path.Code && r.Group == _EquGroup.Code && r.Class == _EquClass.Code
                                               orderby r.Npiketa
                                               select new EquPicket(r.Npiketa, r.number, r.NpicketAfter, r.NpicketBefore, r.StartShiftLine, r.EndShiftLine, r.Dlina, path)).GroupBy(x => x.keyNumber).Select(g => g.First());

            mPicketsList.Clear();

            if (_Pickets.Count() == 0)
                return;

            EquPicket CurPicket = findFirstPicket(_Pickets);
            mPicketsList.Add(CurPicket);

            bool whileIndex = true;
            while (whileIndex)
            {
                whileIndex = false;
                foreach (EquPicket item in _Pickets)
                {
                    if (CurPicket.keyNumber == item.before)
                    {
                        CurPicket = item;
                        mPicketsList.Add(item);
                        whileIndex = true;
                    }
                }
            }
        }
        public List<EquPicket> Matching(EquPath path_object)
        {
            var _EquLine  = path_object.Line;
            var _EquGroup = _EquLine.Group;
            var _EquClass = _EquGroup.Class;

            IEnumerable<EquPicket> IPicketsForMatching = null;

            if (IPicketsForMatching == null)
                throw new NotImplementedException("Matching");

            List<EquPicket> PicketsMatchingList = IPicketsForMatching.ToList();
            List<EquPicket> PicketsList         = new List<EquPicket>();

            for (int i = 0; i < mPicketsList.Count; i++)
            {
                foreach(EquPicket p in PicketsMatchingList)
                {
                    if (p.keyNumber == mPicketsList[i].keyNumber)
                    {
                        PicketsList.Add(mPicketsList[i]);
                        break;
                    }
                }
            }

            mPicketsList = PicketsList;
            return mPicketsList;
        }

        public EquPicket AddPicketToDB( string addedPicketDisplayNum,int Class,int Group,
                                   int LineCode,
                                   int PathCode,
                                   int PickeID,
                                   int PicketLength)
        {
            EquPicket p = new EquPicket();
            p.lenght = PicketLength;
            p.picketTag = PicketTag.New;
            p.npicket = addedPicketDisplayNum;
            p.keyNumber = PickeID;
            p.Name = "Пикет " + addedPicketDisplayNum.ToString();

            if (mPicketsList.Count == 0)
            {
                int picketNum = Convert.ToInt32(addedPicketDisplayNum);

                if (addedPicketDisplayNum[0] == '-') {
                    p.RightLineShift = picketNum * PicketLength;
                    p.LeftLineShift = p.RightLineShift - PicketLength;
                }
                else  {
                    p.LeftLineShift  = picketNum * PicketLength;
                    p.RightLineShift = p.LeftLineShift + PicketLength;
                }


                p.before = 0;
                p.after = 0;

                mPicketsList.Insert(0, p);

                _db_controller.pickets_adapter.PicketCreateFirst(   addedPicketDisplayNum,
                                                                    Class,Group,
                                                                    LineCode,
                                                                    PathCode,
                                                                    0,
                                                                    PickeID,
                                                                    PicketLength,
                                                                    mPicketsList[0].after,
                                                                    mPicketsList[0].before,
                                                                    mPicketsList[0].LeftLineShift,
                                                                    mPicketsList[0].RightLineShift
                                                                    );
                return mPicketsList[0];
            }

            if (AddToLeft)
            {
                
                p.LeftLineShift  = mPicketsList[0].LeftLineShift - PicketLength;
                p.RightLineShift = mPicketsList[0].LeftLineShift;
                p.before = 0;
               
                p.after = mPicketsList[0].keyNumber;
                mPicketsList[0].before = PickeID;

                mPicketsList.Insert(0, p);

                var res =  _db_controller.pickets_adapter.PicketCreateLeft( addedPicketDisplayNum, Class, Group,
                                                                            LineCode,
                                                                            PathCode,
                                                                            0,
                                                                            PicketLength,
                                                                            mPicketsList[0].after,
                                                                            mPicketsList[1].before,
                                                                            mPicketsList[0].LeftLineShift,
                                                                            mPicketsList[0].RightLineShift);
                return mPicketsList[0];
            }
            else
            {
                mPicketsList.Add(p);
                mPicketsList.Last().after = 0;
                mPicketsList.Last().before = mPicketsList[mPicketsList.Count - 2].keyNumber;
                mPicketsList[mPicketsList.Count - 2].after = PickeID;

                mPicketsList.Last().LeftLineShift = mPicketsList[mPicketsList.Count - 2].RightLineShift;
                mPicketsList.Last().RightLineShift = mPicketsList.Last().LeftLineShift + PicketLength;

                _db_controller.pickets_adapter.PicketCreateRight( addedPicketDisplayNum, Class, Group,
                                                                  LineCode,
                                                                  PathCode,
                                                                  0,
                                                                  PicketLength,
                                                                  PickeID,
                                                                  mPicketsList.Last().before,
                                                                  mPicketsList.Last().LeftLineShift,
                                                                  mPicketsList.Last().RightLineShift);
                return mPicketsList.Last();
            }
        }

        public void changePicketLength(EquPicket picket, int Length)
        {
            int selPicket = mPicketsList.IndexOf(picket);
            int delta = mPicketsList[selPicket].lenght - Length;

            if(picket.npicket[0] == '-')
            {
                var r = _db_controller.pickets_adapter.UpdateNegativePickets(picket.keyNumber, delta, Length);
            }
            else 
            {
              var r =   _db_controller.pickets_adapter.UpdatePositivePickets(picket.keyNumber, delta, Length);
            }
        }
    }
}
