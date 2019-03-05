namespace Registrator.DB.EFClasses
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    public partial class AllEquipment
    {
        [Key]
        [DatabaseGenerated(DatabaseGeneratedOption.None)]
        public int Code { get; set; }

        public int EquipID { get; set; }

        public int EquipTypeID { get; set; }

        [StringLength(50)]
        public string Name { get; set; }

        [Column("Group")]
        public int Group { get; set; }

//        [ForeignKey("Line")]
        public int Line { get; set; }

 //       [ForeignKey("Path")]
        public int Path { get; set; }

 //       [ForeignKey("Picket")]
        public int Picket { get; set; }

        public int curTemperature { get; set; }

        public int maxTemperature { get; set; }

        public int EquipWorkState { get; set; }

        public long shiftLine { get; set; }

        public int shiftFromPicket { get; set; }

        public int strelkaLeftOrRight { get; set; }

        public int EquipLenght { get; set; }

        public int Area_X { get; set; }

        public int Area_Y { get; set; }

        public int Area_Width { get; set; }

        public int Area_Height { get; set; }

        public int Area_Type { get; set; }

        [StringLength(50)]
        public string Info { get; set; }

        public virtual EquipmentsClass EquipmentsClass { get; set; }

        [Column("Group")]
        public virtual Group GroupObject { get; set; }
    }
}
