namespace Registrator.DB.EFClasses
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    [Table("Group")]
    public partial class Group
    {
        [Key]
        [Column(Order = 0)]
        [DatabaseGenerated(DatabaseGeneratedOption.None)]
        public short Code { get; set; }

        [Key]
        [Column(Order = 1)]
        public byte Class { get; set; }

        [Key]
        [Column("Group", Order = 2)]
        [StringLength(20)]
        public string Group1 { get; set; }

        public string ObjSrc { get; set; }

        public string PtSrc { get; set; }

        [StringLength(10)]
        public string Color { get; set; }
    }
}
