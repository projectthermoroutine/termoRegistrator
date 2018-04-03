namespace Registrator.DB.EFClasses
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    public partial class Object
    {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.None)]
        public int Code { get; set; }

        public short? Group { get; set; }

        [Column("Object")]
        [StringLength(50)]
        public string Object1 { get; set; }

        public long? shiftLine { get; set; }

        public int? x { get; set; }

        public int? y { get; set; }

        public int? curTemperature { get; set; }

        public int? maxTemperature { get; set; }

        public int? regularly { get; set; }

        public int? shiftFromPicket { get; set; }

        public int? typeId { get; set; }

        public int? typeEquip { get; set; }

        public int? strelkaLeftOrRight { get; set; }

        public int? Picket { get; set; }

        public int? Path { get; set; }

        public int? Line { get; set; }

        public int? Class { get; set; }

        public int? ObjectLenght { get; set; }

        public int? Area_X { get; set; }

        public int? Area_Y { get; set; }

        public int? Area_Width { get; set; }

        public int? Area_Height { get; set; }

        public int? Area_Type { get; set; }
    }
}
