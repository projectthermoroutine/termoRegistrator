namespace Registrator.DB.EFClasses
{
    using System;
    using System.Data.Entity;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Linq;

    public partial class ContextMetroCard : DbContext
    {
        public virtual DbSet<Class> Classes { get; set; }
        public virtual DbSet<Equipment> Equipments { get; set; }
        public virtual DbSet<EquipmentsType> EquipmentsTypes { get; set; }
        public virtual DbSet<Group> Groups { get; set; }
        public virtual DbSet<Line> Lines { get; set; }
        public virtual DbSet<Picket> Pickets { get; set; }
        public virtual DbSet<Track> Tracks { get; set; }
        public virtual DbSet<EquipmentFilter_Tbl> EquipmentFilter_Tbl { get; set; }
        public virtual DbSet<ObjectsFrame> ObjectsFrames { get; set; }
        public virtual DbSet<Order> Orders { get; set; }

        protected override void OnModelCreating(DbModelBuilder modelBuilder)
        {
            modelBuilder.Entity<Class>()
                .HasMany(e => e.Groups)
                .WithRequired(e => e.Class1)
                .HasForeignKey(e => e.Class)
                .WillCascadeOnDelete(false);

            modelBuilder.Entity<Equipment>()
                .Property(e => e.Name)
                .IsUnicode(false);

            modelBuilder.Entity<Equipment>()
                .Property(e => e.Info)
                .IsUnicode(false);

            modelBuilder.Entity<EquipmentsType>()
                .Property(e => e.Name)
                .IsFixedLength();

            modelBuilder.Entity<EquipmentsType>()
                .HasMany(e => e.Equipments)
                .WithRequired(e => e.EquipmentsType)
                .HasForeignKey(e => e.EquipID)
                .WillCascadeOnDelete(false);

            modelBuilder.Entity<Line>()
                .Property(e => e.LineCode)
                .IsUnicode(false);

            modelBuilder.Entity<Line>()
                .HasMany(e => e.Tracks)
                .WithRequired(e => e.Line)
                .HasForeignKey(e => e.LineId)
                .WillCascadeOnDelete(false);

            modelBuilder.Entity<Track>()
                .HasMany(e => e.Pickets)
                .WithRequired(e => e.Track)
                .HasForeignKey(e => e.path)
                .WillCascadeOnDelete(false);
        }
    }
}
