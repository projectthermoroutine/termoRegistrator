/*declare @lineID int;

SET @lineID = 1;

UPDATE [E:\THERMOROUTINE\OUTPUT\REGISTRATOR\DEBUG\X86\DB\NEW_SCHEMA\METROCARD_NEW.MDF].[dbo].[AllEquipments]
SET shiftLine += (SELECT shiftFromPicket, Line
                  FROM AllEquipments
                  WHERE AllEquipments.Code = 6) ;
				  */

declare @correct_line_shifts table(code int NOT NULL, offset int not null);

insert into @correct_line_shifts select code, sum(case when SUBSTRING([dbo].[Pickets].[Npiketa],1,1)!='-' then AllEquipments.shiftFromPicket /*+ Lines.StartCoordinate*/ + Pickets.StartShiftLine else  AllEquipments.shiftFromPicket /*+ Lines.StartCoordinate*/ + Pickets.EndShiftLine end) AS offset  
from AllEquipments 
left join Lines 
on AllEquipments.Line = Lines.LineNum
left join Pickets
on AllEquipments.Picket = Pickets.number
group by code;


update AllEquipments 
set shiftLine = (	select offset 
					from @correct_line_shifts 
					where AllEquipments.Code = code);