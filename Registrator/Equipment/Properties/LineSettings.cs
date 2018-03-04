using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Windows.Forms;
using Registrator.Equipment.CreateDbObjectsCtrls;

namespace Registrator.Equipment
{
    public class LineSettings
    {
        private DB.metro_db_controller _db_controller;
        private EquLine equLine;
        public LineSettings(DB.metro_db_controller db_controller)
        {
            _db_controller = new DB.metro_db_controller(db_controller);
        }

        public void setObjDB(EquTreeNode LineTreeNode)
        {
            equLine = LineTreeNode.ObjectDB as EquLine;
        }

        [DisplayName("код линии")]
        public string LineCode
        {
            get { return _db_controller.dbContext.Lines.Where(l => l.LineNum == equLine.Code).Distinct().FirstOrDefault().LineCode; }
            set 
            {
                string str = value;
                if (str.IndexOfAny(RegistratorFormStrings.incorrect_symbols) == -1)
                {
                    if (str.Length < 100)
                    {
                        DB.EFClasses.Line line = _db_controller.dbContext.Lines.Where(l => l.LineNum == equLine.Code).Distinct().FirstOrDefault();
                        line.LineCode = str;
                        _db_controller.dbContext.Lines.Attach(line);
                        var entry = _db_controller.dbContext.Entry(line);
                        entry.Property(e => e.LineCode).IsModified = true;
                        _db_controller.dbContext.SaveChanges();
                    }
                    else
                        MessageBox.Show("Введено слишком длинное название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                    MessageBox.Show("Некорректно введено название", "", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        [DisplayName(" смещение от начала координат(см)")]
        public long shiftFromBegin
        { 
            get
            {
                return _db_controller.dbContext.Lines
                            .Where(l => l.LineNum == equLine.Code)
                            .Select(l=>l.StartCoordinate)
                            .DefaultIfEmpty(Int32.MinValue)
                            .Distinct()
                            .FirstOrDefault();
            }
            set 
            {
                long startCoordinate = (long)value;

                DB.EFClasses.Line line = _db_controller.dbContext.Lines.Where(l => l.LineNum == equLine.Code).Distinct().FirstOrDefault();

                if (line != null)
                {
                    line.StartCoordinate = startCoordinate;
                    _db_controller.dbContext.Lines.Attach(line);
                    var entry = _db_controller.dbContext.Entry(line);
                    entry.Property(e => e.StartCoordinate).IsModified = true;


                    _db_controller.dbContext.SaveChanges();
                }
            }
        }
    }
}
