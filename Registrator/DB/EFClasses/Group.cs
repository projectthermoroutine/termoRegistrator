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
        [Column("Code")]
        [DatabaseGenerated(DatabaseGeneratedOption.None)]
        public int Code { get; set; }

        [Column("Class")]
        //[ForeignKey("Class")]
        public int ClassId { get; set; }

        [Column("Group")]
        [Required]
        [StringLength(20)]
        public string Name { get; set; }

        public string ObjSrc { get; set; }

        public string PtSrc { get; set; }

        [StringLength(10)]
        public string Color { get; set; }

        [Column("Class")]
        public virtual Class Class { get; set; }
    }
}
