using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms;
using System.Globalization;
using System.Drawing.Design;

using System.Drawing;
using System.Windows.Forms.Design;
using Registrator.Equipment.CreateDbObjectsCtrls;

namespace Registrator.Equipment
{
    public class groupSettings : AbstractProperties
    {
        private DB.metro_db_controller _db_controller;
        private EquGroup equGroup;
        private string Name = null;

        public groupSettings(DB.metro_db_controller db_controller)
        {
            _db_controller = new DB.metro_db_controller(db_controller);
            hex = new StringBuilder();
        }

        public void SetObjDB(EquDbObject equObject)
        {
            _db_object = _db_controller.dbContext.Groups.Where(eq => eq.Code == equObject.Code).Distinct().FirstOrDefault();
        }

        protected DB.EFClasses.Group _db_object;

        [DisplayName("название")]
        public string groupName
        {
            get
            {
                return _db_object.Name;
            }
            set
            {
                string str = value;
                if (str.IndexOfAny(RegistratorFormStrings.incorrect_symbols) == -1)
                {
                    if (str.Length < 100)
                    {
                        _db_object.Name = str;
                        _db_controller.dbContext.Groups.Attach(_db_object);
                        _db_controller.dbContext.Entry(_db_object).State = System.Data.Entity.EntityState.Modified;
                        _db_controller.dbContext.SaveChanges();

                        NameChanged(str);

                    }
                    else
                        MessageBox.Show("Введено слишком длинное название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Некорректно введено название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        private MyColor _Color = new MyColor(0,0,0);
        private StringBuilder hex;
        [DisplayName("Цвет")]
        [EditorAttribute(typeof(MyColorEditor), typeof(UITypeEditor))]
        [CategoryAttribute("Design"), DefaultValueAttribute(typeof(MyColor), "0 0 0"), DescriptionAttribute("Example of a ColorDialog in a PropertyGrid.")]

        public MyColor Color
        {
            get
            {
                Name = _db_object.Color;

                try
                {
                    _Color.Red = Convert.ToByte(Name.Substring(3, 2), 16);
                    _Color.Green = Convert.ToByte(Name.Substring(5, 2), 16);
                    _Color.Blue = Convert.ToByte(Name.Substring(7, 2), 16);
                }
                catch (Exception)
                {
                    ///TODO error logging
                }

                return _Color;
            }

            set
            {
                MyColor c1 = (MyColor)value;
                hex.AppendFormat("#FF");
                hex.AppendFormat("{0:x2}", c1.Red);
                hex.AppendFormat("{0:x2}", c1.Green);
                hex.AppendFormat("{0:x2}", c1.Blue);

                string color_str = hex.ToString();
                _db_object.Color = color_str;
                _db_controller.dbContext.Groups.Attach(_db_object);
                _db_controller.dbContext.Entry(_db_object).State = System.Data.Entity.EntityState.Modified;
                _db_controller.dbContext.SaveChanges();

                hex.Clear();
            }
        }
    }

    public class MyColorEditor : UITypeEditor
    {
        private IWindowsFormsEditorService service;

        public override UITypeEditorEditStyle GetEditStyle(ITypeDescriptorContext context)
        {
            // This tells it to show the [...] button which is clickable firing off EditValue below.
            return UITypeEditorEditStyle.Modal;
        }
        public override object EditValue(ITypeDescriptorContext context, IServiceProvider provider, object value)
        {
            if (provider != null)
                service = (IWindowsFormsEditorService)provider.GetService(typeof(IWindowsFormsEditorService));

            if (service != null)
            {
                // This is the code you want to run when the [...] is clicked and after it has been verified.

                // Get our currently selected color.
               MyColor color = (MyColor)value;

                // Create a new instance of the ColorDialog.
                ColorDialog selectionControl = new ColorDialog();

                // Set the selected color in the dialog.
                int color_n = color.GetARGB();
                selectionControl.Color = Color.FromArgb(color.GetARGB());

                // Show the dialog.
                selectionControl.ShowDialog();

                // Return the newly selected color.
                value = new MyColor(selectionControl.Color.ToArgb());
            }

            return value;
        }
    }
    public class MyColorConverter : TypeConverter
    {
        // This is used, for example, by DefaultValueAttribute to convert from string to MyColor.
        public override object ConvertFrom(ITypeDescriptorContext context, CultureInfo culture, object value)
        {
            if (value.GetType() == typeof(string))
                return new MyColor((string)value);
            return base.ConvertFrom(context, culture, value);
        }
        // This is used, for example, by the PropertyGrid to convert MyColor to a string.
        public override object ConvertTo(ITypeDescriptorContext context, CultureInfo culture, object value, Type destType)
        {
            if ((destType == typeof(string)) && (value is MyColor))
            {
                MyColor color = (MyColor)value;
                return color.ToString();
            }
            return base.ConvertTo(context, culture, value, destType);
        }
    }

    [TypeConverter(typeof(MyColorConverter))]
    public class MyColor
    {
        #region " The color channel variables w/ accessors/mutators. "
        private byte _Red;
        public byte Red
        {
            get { return _Red; }
            set { _Red = value; }
        }

        private byte _Green;
        public byte Green
        {
            get { return _Green; }
            set { _Green = value; }
        }

        private byte _Blue;
        public byte Blue
        {
            get { return _Blue; }
            set { _Blue = value; }
        }
        #endregion

        #region " Constructors "
        public MyColor()
        {
            _Red = 0;
            _Green = 0;
            _Blue = 0;
        }
        public MyColor(byte red, byte green, byte blue)
        {
            _Red = red;
            _Green = green;
            _Blue = blue;
        }
        public MyColor(byte[] rgb)
        {
            if (rgb.Length != 3)
                throw new Exception("Array must have a length of 3.");
            _Red = rgb[0];
            _Green = rgb[1];
            _Blue = rgb[2];
        }
        public MyColor(int argb)
        {
            byte[] bytes = BitConverter.GetBytes(argb);
            _Red = bytes[2];
            _Green = bytes[1];
            _Blue = bytes[0];
        }
        public MyColor(string rgb)
        {
            string[] parts = rgb.Split(' ');
            if (parts.Length != 3)
                throw new Exception("Array must have a length of 3.");
            _Red = Convert.ToByte(parts[0]);
            _Green = Convert.ToByte(parts[1]);
            _Blue = Convert.ToByte(parts[2]);
        }
        #endregion

        #region " Methods "
        public new string ToString()
        {
            return String.Format("{0} {1} {2}", _Red, _Green, _Blue); ;
        }
        public /*new*/ string ToStringARGB()
        {
            return String.Format("{0} {1} {2}", _Red, _Green, _Blue); ;
        }
        public byte[] GetRGB()
        {
            return new byte[] { _Red, _Green, _Blue };
        }
        public int GetARGB()
        {
            byte[] temp = new byte[] { _Blue, _Green, _Red, 255 };
            return BitConverter.ToInt32(temp, 0);
        }
        #endregion
    }
}
