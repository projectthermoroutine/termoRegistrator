namespace Registrator.DB.EFClasses
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    public partial class EquipmentsType
    {
        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2214:DoNotCallOverridableMethodsInConstructors")]
        public EquipmentsType()
        {
            Equipments = new HashSet<Equipment>();
        }

        [DatabaseGenerated(DatabaseGeneratedOption.None)]
        public int Id { get; set; }

        [Required]
        [StringLength(20)]
        public string Name { get; set; }

        public int MinTemperature { get; set; }

        public int MaxTemperature { get; set; }

        public int Width { get; set; }

        public int Height { get; set; }

        public int X { get; set; }

        public int Y { get; set; }

        public int AreaType { get; set; }

        [Column(TypeName = "xml")]
        [Required]
        public string AdditionalOptions { get; set; }

        public int EquipType { get; set; }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2227:CollectionPropertiesShouldBeReadOnly")]
        public virtual ICollection<Equipment> Equipments { get; set; }
    }
}
