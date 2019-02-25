namespace Registrator.DB.EFClasses
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    public partial class ObjectsFrame
    {
        [Key]
        [Column(Order = 0)]
        [DatabaseGenerated(DatabaseGeneratedOption.None)]
        public int ObjID { get; set; }

        [Key]
        [Column(Order = 1)]
        public string FilePath { get; set; }

        [Key]
        [Column(Order = 2)]
        [DatabaseGenerated(DatabaseGeneratedOption.None)]
        public long FrameCoordinate { get; set; }

        [Key]
        [Column(Order = 3)]
        public DateTime Time { get; set; }
        public int? Area_X { get; set; } = 0;

        public int? Area_Y { get; set; } = 0;

        public int? Area_Height { get; set; } = 0;

        public int? Area_Width { get; set; } = 0;

        public int? Area_Type { get; set; } = 0;
    }
}
