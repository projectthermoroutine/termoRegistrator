using Registrator.Equipment.CreateDbObjectsCtrls;
using Registrator.Equipment.Properties;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Registrator.Equipment
{
    public abstract class AbstractProperties
    {
        public event EventHandler<string> UpdateDisplyNameEvent;
        protected void NameChanged(string name)
        {
            UpdateDisplyNameEvent?.Invoke(this, name);
        }

    }

}
