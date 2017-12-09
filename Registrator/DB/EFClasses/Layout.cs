namespace Registrator.DB.EFClasses
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    [Table("Layout")]
    public partial class Layout
    {
        [Key]
        [Column(Order = 0)]
        [DatabaseGenerated(DatabaseGeneratedOption.None)]
        public int Code { get; set; }

        [Key]
        [Column("Layout", Order = 1)]
        [StringLength(100)]
        public string Layout1 { get; set; }

        [StringLength(20)]
        public string SName { get; set; }

        public int? Line { get; set; }

        public int? NperegonBefore { get; set; }

        public int? NperegonAfter { get; set; }

        public int? PeregonLength { get; set; }
    }
}
