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
        int m_FirstPicketBeginCoordinate;
        public PicketsManager(DB.metro_db_controller db_controller, long beginCoordinate)
        {
            _db_controller = new DB.metro_db_controller(db_controller);
            mPicketsList = new List<EquPicket>();
            m_FirstPicketBeginCoordinate = (int)beginCoordinate;
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
                    if (p.number == item.after)
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
      
        public void createLogicalPicketList(int path, int line/*, int peregonID*/)
        {
            _db_controller.pickets_table.Clear();
            _db_controller.pickets_adapter.Fill(_db_controller.pickets_table);

            IEnumerable<EquPicket> _Pickets = (from r in _db_controller.pickets_table.AsEnumerable()
                                                  where r.number != 0 && r.line == line && r.path == path
                                                  orderby r.Npiketa
                                                  select new EquPicket("Пикет " + r.Npiketa, r.number, r.Npiketa, r.number, r.NpicketAfter, r.NpicketBefore, r.StartShiftLine, r.EndShiftLine)).GroupBy(x => x.number).Select(g => g.First());

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
                    if (CurPicket.number == item.before)
                    {
                        CurPicket = item;
                        mPicketsList.Add(item);
                        whileIndex = true;
                    }
                }
            }
        }
        public List<EquPicket> Matching(IEnumerable<EquPicket> IPicketsForMatching)
        {
            List<EquPicket> PicketsMatchingList = IPicketsForMatching.ToList();
            List<EquPicket> PicketsList = new List<EquPicket>();

            for (int i = 0; i < mPicketsList.Count; i++)
            {
                if (PicketsMatchingList.Contains(mPicketsList[i]))
                    PicketsList.Add(mPicketsList[i]);
            }

            mPicketsList = PicketsList;
            return mPicketsList;
        }

        public EquPicket AddPicketToDB( string addedPicketDisplayNum,
                                   int LineCode,
                                   int PathCode,
                                   int PickeID,
                                   int PicketLength)
        {
            EquPicket p = new EquPicket();
            p.picketTag = PicketTag.New;
            p.npicket = addedPicketDisplayNum;
            p.number = PickeID;
            p.Name = "Пикет " + addedPicketDisplayNum.ToString();

            if (mPicketsList.Count == 0)
            {
                mPicketsList.Insert(0, p);
                mPicketsList[0].LeftLineShift = m_FirstPicketBeginCoordinate;
                mPicketsList[0].RightLineShift = m_FirstPicketBeginCoordinate + PicketLength;
                mPicketsList[0].before = 0;
                mPicketsList[0].after = 0;

                _db_controller.pickets_adapter.PicketCreateFirst(   addedPicketDisplayNum,
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
                mPicketsList.Insert(0, p);
                mPicketsList[0].LeftLineShift = mPicketsList[1].LeftLineShift - PicketLength;
                mPicketsList[0].RightLineShift = mPicketsList[1].LeftLineShift;
                mPicketsList[0].before = 0;
               
                mPicketsList[0].after = mPicketsList[1].number;
                mPicketsList[1].before = PickeID;
                    
                _db_controller.pickets_adapter.PicketCreateLeft(    addedPicketDisplayNum,
                                                                    LineCode,
                                                                    PathCode,
                                                                    0,
                                                                    PickeID,
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
                mPicketsList.Last().before = mPicketsList[mPicketsList.Count - 2].number;
                mPicketsList[mPicketsList.Count - 2].after = PickeID;

                mPicketsList.Last().LeftLineShift = mPicketsList[mPicketsList.Count - 2].RightLineShift;
                mPicketsList.Last().RightLineShift = mPicketsList.Last().LeftLineShift + PicketLength;

                _db_controller.pickets_adapter.PicketCreateRight( addedPicketDisplayNum,
                                                                  LineCode,
                                                                  PathCode,
                                                                  0,
                                                                  PickeID,
                                                                  PicketLength,
                                                                  PickeID,
                                                                  mPicketsList.Last().before,
                                                                  mPicketsList.Last().LeftLineShift,
                                                                  mPicketsList.Last().RightLineShift);
                return mPicketsList.Last();
            }
        }
    }
}
