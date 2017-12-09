namespace Registrator.DB.EFClasses
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    [Table("Main")]
    public partial class Main
    {
        [Key]
        [Column(Order = 0)]
        [DatabaseGenerated(DatabaseGeneratedOption.None)]
        public int Code { get; set; }

        [Key]
        [Column(Order = 1)]
        [DatabaseGenerated(DatabaseGeneratedOption.None)]
        public int Npicket { get; set; }

        public int? Track { get; set; }

        [Key]
        [Column(Order = 2)]
        public byte Border { get; set; }

        public int? ObjData { get; set; }

        public int? PtData { get; set; }

        [Key]
        [Column(Order = 3)]
        [DatabaseGenerated(DatabaseGeneratedOption.None)]
        public short Point { get; set; }

        [Key]
        [Column(Order = 4)]
        public float Displacement { get; set; }

        public int? Area { get; set; }

        public float? Y { get; set; }

        public int? Layout { get; set; }

        public int? ClassNum { get; set; }

        public int? GroupNum { get; set; }

        public int? LineNum { get; set; }
    }
}
