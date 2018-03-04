using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Registrator.DB.MetrocardDataSetTableAdapters
{

    /// <summary>
    ///
    ///</summary>

    public partial class QueriesTableAdapter 
    {
        public object GetReturnValue(int commandIndex)
        {
            return ((global::System.Data.SqlClient.SqlCommand)(this._commandCollection[commandIndex])).Parameters[0].Value;
        }
    }
}
