namespace Registrator.DB.EFClasses
{
    using System;
    using System.Data.Entity;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Linq;

    public partial class ContextMetroCard : DbContext
    {
        public ContextMetroCard()
            : base("name=ContextMetroCard")
        {
        }

        public virtual DbSet<Class> Classes { get; set; }
        public virtual DbSet<EquipmentFilter_Tbl> EquipmentFilter_Tbl { get; set; }
        public virtual DbSet<Equipment> Equipments { get; set; }
        public virtual DbSet<Group> Groups { get; set; }
        public virtual DbSet<Layout> Layouts { get; set; }
        public virtual DbSet<Layout1> Layout1 { get; set; }
        public virtual DbSet<Line> Lines { get; set; }
        public virtual DbSet<Main> Mains { get; set; }
        //public virtual DbSet<Object> Objects { get; set; }
        public virtual DbSet<ObjectsFrame> ObjectsFrames { get; set; }
        public virtual DbSet<Passege> Passeges { get; set; }
        public virtual DbSet<Passege1> Passeges1 { get; set; }
        public virtual DbSet<Picket> Pickets { get; set; }
        public virtual DbSet<Table_1> Table_1 { get; set; }
        public virtual DbSet<TEST1> TEST1 { get; set; }
        public virtual DbSet<Track> Tracks { get; set; }

        protected override void OnModelCreating(DbModelBuilder modelBuilder)
        {
            modelBuilder.Entity<Equipment>()
                .Property(e => e.Object)
                .IsUnicode(false);

            modelBuilder.Entity<Line>()
                .Property(e => e.LineCode)
                .IsUnicode(false);

            //modelBuilder.Entity<Object>()
            //    .Property(e => e.Object1)
            //    .IsUnicode(false);

            modelBuilder.Entity<Table_1>()
                .Property(e => e.name)
                .IsFixedLength();

            modelBuilder.Entity<TEST1>()
                .Property(e => e.name)
                .IsFixedLength();
        }
    }
}
