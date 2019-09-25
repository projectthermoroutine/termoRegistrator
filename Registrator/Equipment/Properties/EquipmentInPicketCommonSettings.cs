using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Registrator.Equipment.Properties 
{
    public class EquipmentInPicketCommonSettings : EquipmentsCommonSettings
    {

        protected int PicketLength { get; set; }

        private bool _auto_negative_offset = false;
        private bool _negative_picket = false;

        public EquipmentInPicketCommonSettings(DB.metro_db_controller db_controller)
            : base(db_controller)
        {
            _auto_negative_offset = Registrator.Properties.Settings.Default.auto_negative_offset_for_object_in_negative_picket;
        }

        [DisplayName("техническое состояние")]
        [TypeConverter(typeof(DrinkerClassConverter))]
        public bool texStatus
        {
            get { return Convert.ToBoolean(_db_object.EquipWorkState); }
            set
            {
                _db_object.EquipWorkState = (value) ? 1 : 0;
                _db_controller.dbContext.AllEquipments.Attach(_db_object);
                _db_controller.dbContext.Entry(_db_object).State = System.Data.Entity.EntityState.Modified;
                _db_controller.dbContext.SaveChanges();
            }
        }

        public new void SetObjDB(EquDbObject equObject)
        {
            _db_object = _db_controller.dbContext.AllEquipments.Where(eq => eq.Code == equObject.Code).Distinct().FirstOrDefault();

            EquPicket equPicket = equObject.Parent as EquPicket;
            PicketLength = equPicket.Length;
            _negative_picket = equPicket.LeftLineShift < 0;

        }

        [DisplayName("смещение от начала пикета(мм)")]
        [PropertyOrder(10)]
        public int ShiftFromBegin
        {
            get
            {
                if (_db_object.shiftLine >= 0) return _db_object.shiftFromPicket;

                return _auto_negative_offset ? -_db_object.shiftFromPicket : (PicketLength + _db_object.shiftFromPicket);
            }

            set
            {
                if (value >= 0 && value <= PicketLength)
                {
                    int offset = _db_object.shiftFromPicket;
                    _db_object.shiftFromPicket = value;

                    if (_negative_picket)
                    {
                        if(_auto_negative_offset)
                        {
                            _db_object.shiftFromPicket = -value;
                        }
                        else
                        {
                            _db_object.shiftFromPicket = value - PicketLength;
                        }

                        offset = _db_object.shiftFromPicket - offset;
                    }

                    _db_object.shiftLine += offset;
                    _db_controller.dbContext.AllEquipments.Attach(_db_object);
                    _db_controller.dbContext.Entry(_db_object).State = System.Data.Entity.EntityState.Modified;
                    _db_controller.dbContext.SaveChanges();
                }
                else
                {
                    MessageBox.Show("Значение выходит за пределы пикета", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
        }


    }

    class DrinkerClassConverter : BooleanConverter
    {
        public override object ConvertTo(ITypeDescriptorContext context,
        CultureInfo culture,
        object value,
        Type destType)
        {
            return (bool)value ?
            "неисправно" : "исправно";
        }

        public override object ConvertFrom(ITypeDescriptorContext context,
        CultureInfo culture,
        object value)
        {
            return (string)value == "неисправно";
        }
    }

}
