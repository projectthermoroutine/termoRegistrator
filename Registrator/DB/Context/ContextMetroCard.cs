namespace Registrator.DB.EFClasses
{
    using System;
    using System.Data.Entity;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Linq;
    using Registrator.DB.EFClasses;

    public partial class ContextMetroCard : DbContext
    {
        public ContextMetroCard()
            : base("name=ContextMetroCard")
        {
        }

        public virtual DbSet<AllEquipment> AllEquipments { get; set; }
        public virtual DbSet<Class> Classes { get; set; }
        public virtual DbSet<EquipmentsClass> EquipmentsClasses { get; set; }
        public virtual DbSet<Group> Groups { get; set; }
        public virtual DbSet<Line> Lines { get; set; }
        public virtual DbSet<Picket> Pickets { get; set; }
        public virtual DbSet<Track> Tracks { get; set; }
        //public virtual DbSet<Registrator.DB.EFClasses.Object> Objects { get; set; }
        public virtual DbSet<ObjectsFrame> ObjectsFrames { get; set; }
        public virtual DbSet<Order> Orders { get; set; }
        public virtual DbSet<Passege> Passeges { get; set; }
        public virtual DbSet<Passege1> Passeges1 { get; set; }
        public virtual DbSet<EquipmentFilter_Tbl> EquipmentFilter_Tbl { get; set; }

        protected override void OnModelCreating(DbModelBuilder modelBuilder)
        {
            modelBuilder.Entity<Class>()
                .HasMany(e => e.Groups)
                .WithRequired(e => e.Class)
                .HasForeignKey(e => e.ClassId)
                .WillCascadeOnDelete(false);

            modelBuilder.Entity<EquipmentsClass>()
                .HasMany(e => e.AllEquipments)
                .WithRequired(e => e.EquipmentsClass)
                .HasForeignKey(e => e.EquipID)
                .WillCascadeOnDelete(false);

            modelBuilder.Entity<Line>()
                .HasMany(e => e.Tracks)
                .WithRequired(e => e.Line)
                .HasForeignKey(e => e.LineId)
                .WillCascadeOnDelete(false);


            modelBuilder.Entity<Track>()
                .HasMany(e => e.Pickets)
                .WithRequired(e => e.Track)
                .HasForeignKey(e => e.path);
        }
    }
}
