namespace Registrator.DB.EFClasses
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    public partial class Line
    {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.None)]
        public int LineNum { get; set; }

        [Required]
        [StringLength(50)]
        public string LineName { get; set; }

        public long StartCoordinate { get; set; }

        [Required]
        [StringLength(100)]
        public string LineCode { get; set; }
    }
}
