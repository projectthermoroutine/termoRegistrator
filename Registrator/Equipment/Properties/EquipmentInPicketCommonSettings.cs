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

        public EquipmentInPicketCommonSettings(DB.metro_db_controller db_controller)
            : base(db_controller)
        {

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
            code_equip = equObject.Code;
            PicketLength = equPicket.Length;

        }

        [DisplayName("смещение от начала пикета(мм)")]
        [PropertyOrder(10)]
        public int ShiftFromBegin
        {
            get { return _db_object.shiftLine >= 0 ? _db_object.shiftFromPicket : (PicketLength + _db_object.shiftFromPicket); }

            set
            {
                if (value >= 0 && value <= PicketLength)
                {
                    int offset = _db_object.shiftFromPicket;
                    _db_object.shiftFromPicket = _db_object.shiftLine >= 0 ? value : (value - PicketLength);
                    offset = _db_object.shiftFromPicket - offset;
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
