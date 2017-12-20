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
        [DatabaseGenerated(DatabaseGeneratedOption.None)]
        public int Code { get; set; }

        public int? Class { get; set; }

        [Column("Group")]
        [Required]
        [StringLength(20)]
        public string Group1 { get; set; }

        public string ObjSrc { get; set; }

        public string PtSrc { get; set; }

        [StringLength(10)]
        public string Color { get; set; }

        public virtual Class Class1 { get; set; }
    }
}
