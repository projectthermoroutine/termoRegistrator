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

        //[ForeignKey("path")]
        public int path { get; set; }

        public int StartShiftLine { get; set; }

        public int EndShiftLine { get; set; }
        
        public virtual Track Track { get; set; }

        public static string NextNumber(string lastNumber, bool left)
        {
            int picket_number = Convert.ToInt32(lastNumber);
            string str_picket_number = "";

            if (left)
            {
                if (lastNumber == "0")
                    str_picket_number = "-0";
            }
            else
            {
                if (lastNumber == "-0")
                    str_picket_number = "0";

                if (lastNumber == "-1")
                    str_picket_number = "-0";

            }


            if (str_picket_number == "")
            {
                if (left)
                    str_picket_number = (picket_number - 1).ToString();
                else
                    str_picket_number = (picket_number + 1).ToString();
            }


            return str_picket_number;
        }

        public static class PicketTraits
        {
            public const int DefaultLengthMm = 100000;
        }


        public static Picket CreateMinusZeroPicket()
        {
            return new EFClasses.Picket
            {
                Dlina = PicketTraits.DefaultLengthMm,
                EndShiftLine = 0,
                StartShiftLine = -PicketTraits.DefaultLengthMm,
                Npiketa = "-0"
            };

        }

        public bool NonStandartLength
        {
            get { return Dlina != PicketTraits.DefaultLengthMm; }
        }

    }
}
