﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms;
using System.Globalization;
using System.Drawing.Design;

using System.Drawing;
using System.ComponentModel;
using System.Windows.Forms.Design;


namespace Registrator.Equipment
{
    public class groupSettings
    {
        private DB.DataBaseHelper dbHelper;
        private EquGroup equGroup;
        private string Name = null;
        private int m_peregonLength;

        public groupSettings(DB.DataBaseHelper dbHelper_Arg)
        {
            dbHelper = dbHelper_Arg;
            
        }

        public void setObjDB( EquGroup equGroup_Arg)
        {
            equGroup = equGroup_Arg;
        }


        [DisplayName("название")]
        public string groupName
        {
            get
            {
                var res = from r in dbHelper.dataTable_GroupTable.AsEnumerable() where r.Code == equGroup.Code  select r;

                if (res.Count() == 1)
                {
                    Name = res.First().Group;
                    return Name;
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
                        dbHelper.TblAdapter_Group.renameGroup(str, (short)equGroup.Code);
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

        private MyColor _Color = new MyColor();
        private System.Windows.Media.Color c;

        [DisplayName("цвет")]
        [EditorAttribute(typeof(MyColorEditor), typeof(UITypeEditor))]
        [CategoryAttribute("Design"), DefaultValueAttribute(typeof(MyColor), "0 0 0"), DescriptionAttribute("Example of a ColorDialog in a PropertyGrid.")]

        public MyColor Color
        {
            get {
                    var res = from r in dbHelper.dataTable_GroupTable.AsEnumerable() where r.Code == equGroup.Code select r;

                    if (res.Count() == 1)
                    {
                        Name = res.First().Color;

                        _Color.Red = Convert.ToByte(Name.Substring(3, 2),16);
                        _Color.Green = Convert.ToByte(Name.Substring(5, 2), 16);
                        _Color.Blue = Convert.ToByte(Name.Substring(7, 2), 16);
                    }
                    return _Color; 
                }

            set { 
                    MyColor c1 = (MyColor)value;

                    string color_str = String.Concat(new object[] { "#FF", Convert.ToString(c1.Red, 16), Convert.ToString(c1.Green, 16), Convert.ToString(c1.Blue, 16) });
                    dbHelper.TblAdapter_Group.UpdateGrColor(equGroup.Code, color_str);
                    dbHelper.refresh();    
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
        public new string ToStringARGB()
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
