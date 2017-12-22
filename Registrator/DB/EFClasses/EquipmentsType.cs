namespace Registrator.DB.EFClasses
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    public partial class EquipmentsType
    {
        [DatabaseGenerated(DatabaseGeneratedOption.None)]
        public int Id { get; set; }

        [StringLength(20)]
        public string Name { get; set; }

        public int? MinTemperature { get; set; }

        public int? MaxTemperature { get; set; }

        public int? Width { get; set; }

        public int? Height { get; set; }

        public int? X { get; set; }

        public int? Y { get; set; }

        public int? AreaType { get; set; }

        [Column(TypeName = "xml")]
        public string AdditionalOptions { get; set; }

        public int EquipType { get; set; }
    }
}
