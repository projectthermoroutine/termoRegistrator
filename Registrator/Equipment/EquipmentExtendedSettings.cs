﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms;
using System.Globalization;

namespace Registrator.Equipment
{
    public class EquipmentExtendedSettings
    {
        private DB.DataBaseHelper dbHelper;
        private EquObject equObject;
        private string equipmentName = null;
        private int regularly;

        public EquipmentExtendedSettings(DB.DataBaseHelper dbHelper_Arg, EquObject equObject_Arg)
        {
            dbHelper = dbHelper_Arg;

            equObject = equObject_Arg;
        }

        [DisplayName("название")]
        public string equipmentKName
        {
            get
            {
                var res = from r in dbHelper.dataTable_Objects.AsEnumerable() where r.Code == equObject.Code select r;

                if (res.Count() == 1)
                {
                    equipmentName = res.First().Object;
                    return equipmentName;
                }

                return "";
            }
            set
            {
                string str = value;
                if (str.IndexOfAny(new char[] { '@', '.', ',', '!', '\'', ';', '[', ']', '{', '}', '"', '?', '>', '<', '+', '$', '%', '^', '&', '*', '`', '№', '\\', '|' }) == -1)
                {
                    if (str.Length < 100)
                    {
                        dbHelper.TblAdapter_Objects.renameEquipment(equObject.Code, str);
                        dbHelper.refresh();
                        FireRename(new RenameEvent(str));
                    }
                    else
                        MessageBox.Show("Введено слишком длинное название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Некорректно введено название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        [DisplayName("техническое состояние")]
        [TypeConverter(typeof(DrinkerClassConverter))]
        public bool texStatus
        {
            get
            {
                var res = from r in dbHelper.dataTable_Objects.AsEnumerable() where r.Code == equObject.Code select r;

                if (res.Count() == 1)
                {
                    regularly = res.First().regularly;
                    
                    if (regularly == 0)
                        return false;
                    if (regularly == 1)
                        return true;
                }

                return false;
            }
            set
            {
                int status = (value)? 1:0;
                dbHelper.TblAdapter_Objects.UpdateStatusEquip(equObject.Code, status);
                dbHelper.refresh();
            }
        }

        [DisplayName("смещение от начала пикета")]
        public int shiftFromBegin
        {
            get
            {
                 var res = from r in dbHelper.dataTable_Objects.AsEnumerable() where r.Code == equObject.Code select r;

                 int shift;

                 if (res.Count() == 1)
                 {
                     shift = res.First().shiftFromPicket;
                     return shift;
                 }
                 return -1;
            }
            set
            {
                int shift = value;

                if(shift > 0)
                { 
                    if(shift < 900000)
                    {
                        dbHelper.TblAdapter_Objects.UpdateShiftBeginEquip(equObject.Code, shift);
                        dbHelper.refresh();
                    }
                    else
                        MessageBox.Show("Значение слишком велико", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Смещение должно быть больше 0", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        [DisplayName("смещение от конца пикета")]
        public int shiftFromEnd
        {
            get
            {
                var res = from r in dbHelper.dataTable_Objects.AsEnumerable() where r.Code == equObject.Code select r;

                int shift;

                if (res.Count() == 1)
                {
                    shift = res.First().shiftFromEndPicket;
                    return shift;
                }

                return -1;
            }
            set
            {
                int shift = value;

                if (shift > 0)
                {
                    if (shift < 900000)
                    {
                        dbHelper.TblAdapter_Objects.UpdateShiftEndEquip(equObject.Code, shift);
                        dbHelper.refresh();
                    }
                    else
                        MessageBox.Show("Значение слишком велико", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Смещение должно быть больше 0", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

        }

        public event EventHandler<RenameEvent> RenameEventHandler;

        public virtual void FireRename(RenameEvent e)
        {
            EventHandler<RenameEvent> handler = RenameEventHandler;

            if (handler != null)
            {
                handler(this, e);
            }
        }
    }
}