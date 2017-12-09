namespace Registrator.DB
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    public partial class EquipmentFilter_Tbl
    {
        [Key]
        [Column(Order = 0)]
        [DatabaseGenerated(DatabaseGeneratedOption.None)]
        public int filter_id { get; set; }

        [Key]
        [Column(Order = 1)]
        [DatabaseGenerated(DatabaseGeneratedOption.None)]
        public int class_id { get; set; }

        [Key]
        [Column(Order = 2)]
        [StringLength(50)]
        public string class_Name { get; set; }

        [Key]
        [Column(Order = 3)]
        public string groups_Names { get; set; }

        [Key]
        [Column(Order = 4)]
        public string groups_Numbers { get; set; }

        [Key]
        [Column(Order = 5)]
        [DatabaseGenerated(DatabaseGeneratedOption.None)]
        public int apply { get; set; }
    }
}
