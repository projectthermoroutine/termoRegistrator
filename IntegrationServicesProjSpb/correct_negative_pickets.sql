/****** Script for SelectTopNRows command from SSMS  ******/

DECLARE @picketID int,
		@begin_picket_offset int,
		@end_picket_offset  int

DECLARE vendor_cursor CURSOR FOR   
SELECT [number], [StartShiftLine], [EndShiftLine] FROM [METROCARD_NEW].[dbo].[Pickets] where EndShiftLine < 0;

OPEN vendor_cursor  

FETCH NEXT FROM vendor_cursor   
INTO @picketID, @begin_picket_offset, @end_picket_offset

declare @tmp int;

WHILE @@FETCH_STATUS = 0  
BEGIN  
   
	set @tmp = @begin_picket_offset
	set @begin_picket_offset = @end_picket_offset;
	set @end_picket_offset = @tmp;

--	PRINT '<---------------------------------------------------->';
--	PRINT 'previous offset: ' + cast(@cur_equip_offset as nvarchar(50))+ '. Picket length: ' + cast(@cur_picket_length as nvarchar(50)) + '. Picket offset: ' + cast(@cur_start_shift_line as nvarchar(50)) + '. Picket label: ' + cast(@cur_before_picket_label as nvarchar(50));
--	PRINT 'new picket offset' + cast(@cur_new_by_picket_offset as nvarchar(50)) + '. New by line offset: ' + cast(@cur_new_line_offset as nvarchar(50));

	update [Pickets] set StartShiftLine = @begin_picket_offset, EndShiftLine = @end_picket_offset  where number = @picketID;

	FETCH NEXT FROM vendor_cursor   
	INTO @picketID, @begin_picket_offset, @end_picket_offset
END   
CLOSE vendor_cursor;  
DEALLOCATE vendor_cursor;  