namespace Registrator.DB.EFClasses
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    public partial class Order
    {
        [Key]
        [Column(Order = 0)]
        [DatabaseGenerated(DatabaseGeneratedOption.None)]
        public int ID { get; set; }

        [Key]
        [Column(Order = 1)]
        public string Person { get; set; }

        [Key]
        [Column(Order = 2)]
        public string Desc { get; set; }

        [Key]
        [Column(Order = 3, TypeName = "date")]
        public DateTime CreationDate { get; set; }

        [Key]
        [Column(Order = 4, TypeName = "date")]
        public DateTime FirstDate { get; set; }

        [Key]
        [Column(Order = 5, TypeName = "date")]
        public DateTime FinishDate { get; set; }

        [Key]
        [Column(Order = 6)]
        [DatabaseGenerated(DatabaseGeneratedOption.None)]
        public int State { get; set; }

        [Key]
        [Column(Order = 7)]
        [DatabaseGenerated(DatabaseGeneratedOption.None)]
        public int id_equipment { get; set; }
    }
}
