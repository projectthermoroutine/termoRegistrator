using Registrator.DB.MetrocardDataSetTableAdapters;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Registrator.DB.MetrocardDataSetTableAdapters
{
    public partial class QueriesTableAdapter
    {
        public int GetResultUpdatePositivePickets()
        {
            var res = ((global::System.Data.SqlClient.SqlCommand)(this._commandCollection[25])).Parameters[0].Value;
            var res1 = ((global::System.Data.SqlClient.SqlCommand)(this._commandCollection[25])).Parameters[4].Value;
           
            return (int)res;
        }

        public int GetResult(int index)
        {
           return (int)((global::System.Data.SqlClient.SqlCommand)(this._commandCollection[index])).Parameters[0].Value;
        }
    }

    class QueriesHelper
    {


    }
}
