using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms;
using System.Globalization;
using Registrator.Equipment.CreateDbObjectsCtrls;

namespace Registrator.Equipment
{
    public class EquipmentExtendedSettings
    {
        private DB.metro_db_controller _db_controller;
        private EquObject equObject;

        public EquipmentExtendedSettings(DB.metro_db_controller db_controller)
        {
            _db_controller = new DB.metro_db_controller(db_controller);
        }

        public void setObjDB(EquTreeNode ObjectTreeNode)
        {
            equObject = ObjectTreeNode.ObjectDB as EquObject;
        }

        [DisplayName("название")]
        public string equipmentKName
        {
            get { return _db_controller.dbContext.Equipments.Where(e => e.Code == equObject.Code).Distinct().FirstOrDefault().Name; }
            set
            {
                string str = value;
                if (str.IndexOfAny(RegistratorFormStrings.incorrect_symbols) == -1)
                {
                    if (str.Length < 100)
                    {
                        DB.EFClasses.Equipment equip = _db_controller.dbContext.Equipments.Where(eq => eq.Code == equObject.Code).Distinct().FirstOrDefault();
                        equip.Name = str;
                        _db_controller.dbContext.Equipments.Attach(equip);
                        var entry = _db_controller.dbContext.Entry(equip);
                        entry.Property(e => e.Name).IsModified = true;
                        _db_controller.dbContext.SaveChanges();
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
            get  {  return Convert.ToBoolean(_db_controller.dbContext.Equipments.Where(eq => eq.Code == equObject.Code).Distinct().FirstOrDefault().EquipWorkState);  }
            set
            {
                int status = (value)? 1:0;
                DB.EFClasses.Equipment equip = _db_controller.dbContext.Equipments.Where(eq => eq.Code == equObject.Code).Distinct().FirstOrDefault();
                equip.EquipWorkState = status;
                _db_controller.dbContext.Equipments.Attach(equip);
                var entry = _db_controller.dbContext.Entry(equip);
                entry.Property(e => e.EquipWorkState).IsModified = true;
                _db_controller.dbContext.SaveChanges();
            }
        }

        [DisplayName("смещение от начала пикета(см)")]
        public int shiftFromBegin
        {
            get {  return _db_controller.dbContext.Equipments.Where(eq => eq.Code == equObject.Code).Distinct().FirstOrDefault().shiftFromPicket;  }
            set
            {
                int shift = value;

                if(shift > 0)
                { 
                    if(shift < 900000)
                    {
                        DB.EFClasses.Equipment equip = _db_controller.dbContext.Equipments.Where(eq => eq.Code == equObject.Code).Distinct().FirstOrDefault();
                        equip.shiftFromPicket = shift;
                        _db_controller.dbContext.Equipments.Attach(equip);
                        var entry = _db_controller.dbContext.Entry(equip);

                        entry.Property(e => e.shiftFromPicket).IsModified = true;
                        _db_controller.dbContext.SaveChanges();
                    }
                    else
                        MessageBox.Show("Значение слишком велико", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Смещение должно быть больше 0", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        [DisplayName("длина объекта(мм)")]
        public int shiftFromEnd
        {
            get {  return _db_controller.dbContext.Equipments.Where(eq => eq.Code == equObject.Code).Distinct().FirstOrDefault().EquipLenght;  }
            set
            {
                int shift = value;

                if (shift > 0)
                {
                    if (shift < int.MaxValue)
                    {
                        DB.EFClasses.Equipment equip = _db_controller.dbContext.Equipments.Where(eq => eq.Code == equObject.Code).Distinct().FirstOrDefault();
                        equip.EquipLenght = shift;
                        _db_controller.dbContext.Equipments.Attach(equip);
                        var entry = _db_controller.dbContext.Entry(equip);
                        entry.Property(e => e.EquipLenght).IsModified = true;
                        _db_controller.dbContext.SaveChanges();
                    }
                    else
                        MessageBox.Show("Значение слишком велико", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Длина должна быть больше 0", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }

        }

        [DisplayName("максимально допустимая температура(°C)")]
        public int maxTemperature
        {
            get {  return _db_controller.dbContext.Equipments.Where(eq => eq.Code == equObject.Code).Distinct().FirstOrDefault().maxTemperature;  }
            set
            {
                int maxTemp = value;

                if (maxTemp > 0)
                {
                    if (maxTemp < 900000)
                    {
                        DB.EFClasses.Equipment equip = _db_controller.dbContext.Equipments.Where(eq => eq.Code == equObject.Code).Distinct().FirstOrDefault();
                        equip.maxTemperature = maxTemp;
                        _db_controller.dbContext.Equipments.Attach(equip);
                        var entry = _db_controller.dbContext.Entry(equip);

                        entry.Property(e => e.maxTemperature).IsModified = true;
                        _db_controller.dbContext.SaveChanges();
                    }
                    else
                        MessageBox.Show("Значение слишком велико", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Значение должно быть больше 0", "", MessageBoxButtons.OK, MessageBoxIcon.Information);

            }
        }

        [DisplayName("дополнительная информация")]
        public string additionalInfo
        {
            get
            {
                return _db_controller.dbContext.Equipments.Where(eq => eq.Code == equObject.Code).Distinct().Select(e => e.Info).DefaultIfEmpty("").FirstOrDefault();
            }
            set
            {
                string info = value;

                if (info.Length < 1024)
                {
                    DB.EFClasses.Equipment equip = _db_controller.dbContext.Equipments.Where(eq => eq.Code == equObject.Code).Distinct().FirstOrDefault();
                    equip.Info = info;
                    _db_controller.dbContext.Equipments.Attach(equip);
                    var entry = _db_controller.dbContext.Entry(equip);

                    entry.Property(e => e.Info).IsModified = true;
                    _db_controller.dbContext.SaveChanges();
                }
                else
                    MessageBox.Show("Максимальная длина строки 1024 символа", "Информация", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }
    }
}
