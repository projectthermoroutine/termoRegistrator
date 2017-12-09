namespace Registrator.DB.EFClasses
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    public partial class TEST1
    {
        public int id { get; set; }

        [StringLength(10)]
        public string name { get; set; }

        public string nbbnb { get; set; }
    }
}
