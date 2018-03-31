namespace Registrator.DB.EFClasses
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    public partial class Picket
    {
        [Required]
        [StringLength(50)]
        public string Npiketa { get; set; }

        public int Dlina { get; set; }

        public int NpicketBefore { get; set; }

        public int NpicketAfter { get; set; }

        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.None)]
        public int number { get; set; }

        public int path { get; set; }

        public int StartShiftLine { get; set; }

        public int EndShiftLine { get; set; }

        public virtual Track Track { get; set; }

        public virtual Track Track1 { get; set; }
    }
}
