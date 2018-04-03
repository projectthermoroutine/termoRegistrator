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

        protected int PicketLenght { get; set; }

        public EquipmentInPicketCommonSettings(DB.metro_db_controller db_controller)
            : base(db_controller)
        {

        }

        [DisplayName("техническое состояние")]
        [TypeConverter(typeof(DrinkerClassConverter))]
        public bool texStatus
        {
            get { return Convert.ToBoolean(_db_controller.dbContext.AllEquipments.Where(eq => eq.Code == code_equip).Distinct().FirstOrDefault().EquipWorkState); }
            set
            {
                int status = (value) ? 1 : 0;
                DB.EFClasses.AllEquipment equip = _db_controller.dbContext.AllEquipments.Where(eq => eq.Code == code_equip).Distinct().FirstOrDefault();
                equip.EquipWorkState = status;
                _db_controller.dbContext.AllEquipments.Attach(equip);
                var entry = _db_controller.dbContext.Entry(equip);
                entry.Property(e => e.EquipWorkState).IsModified = true;
                _db_controller.dbContext.SaveChanges();
            }
        }

        public new void SetObjDB(EquTreeNode ObjectTreeNode)
        {
            EquObject equObject = ObjectTreeNode.ObjectDB as EquObject;
            EquPicket equPicket = (ObjectTreeNode.Parent as EquTreeNode).ObjectDB as EquPicket;

            code_equip = equObject.Code;
            PicketLenght = equPicket.lenght;

        }

        [DisplayName("смещение от начала пикета(см)")]
        public int ShiftFromBegin
        {
            get { return _db_controller.dbContext.AllEquipments.Where(eq => eq.Code == code_equip).Distinct().FirstOrDefault().shiftFromPicket; }

            ///TODO
            //set
            //{
            //    int shift = value;

            //    if (PicketLenght > shift)
            //    {
            //        //_db_controller.dbContext.AllEquipments
            //        DB.EFClasses.AllEquipment equip = _db_controller.dbContext.AllEquipments.Where(eq => eq.Code == code_equip).Distinct().FirstOrDefault();
            //        equip.shiftFromPicket = shift;
            //        _db_controller.dbContext.AllEquipments.Attach(equip);
            //        var entry = _db_controller.dbContext.Entry(equip);

            //        entry.Property(e => e.shiftFromPicket).IsModified = true;
            //        _db_controller.dbContext.SaveChanges();

            //        //_db_controller.queriesAdapter.UpdateShiftBeginEquip(equObject.Code, shift);
            //    }
            //    else
            //        MessageBox.Show("Значение выходит за пределы пикета", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            //}
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
