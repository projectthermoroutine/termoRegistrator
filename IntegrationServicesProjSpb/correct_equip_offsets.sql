/****** Script for SelectTopNRows command from SSMS  ******/



/*SELECT [Code]
      ,[EquipID]
      ,[EquipTypeID]
      ,[Name]
      ,[Group]
      ,[Line]
      ,[Path]
      ,[Picket]
      ,[curTemperature]
      ,[maxTemperature]
      ,[EquipWorkState]
      ,[shiftLine]
      ,[shiftFromPicket]
      ,[strelkaLeftOrRight]
      ,[EquipLenght]
      ,[Area_X]
      ,[Area_Y]
      ,[Area_Width]
      ,[Area_Height]
      ,[Area_Type]
      ,[Info]
  FROM [METROCARD_NEW_SPB_MIGRATED].[dbo].[AllEquipments] where shiftFromPicket < 0;
  */

/*
DECLARE @eq_minus table(code int);

insert into @eq_minus  select distinct [Code] from [METROCARD_NEW_SPB_MIGRATED].[dbo].[AllEquipments]  where shiftFromPicket < 0;

declare @counter as int;
select @counter = count([Code]) from @eq_minus;
*/

DECLARE @code int,  
		--@EquipID int,
		--@EquipTypeID int,
		@picketID int,
		@picketBefore int

DECLARE vendor_cursor CURSOR FOR   
SELECT [Code], [Picket] FROM [METROCARD_NEW_SPB_MIGRATED].[dbo].[AllEquipments] where shiftFromPicket > 0;

OPEN vendor_cursor  

FETCH NEXT FROM vendor_cursor   
INTO @code, @picketID  

declare @cur_before_picket_number int;
declare @cur_before_picket_label as nvarchar(50);
declare @cur_picket_length as int;
declare @cur_equip_offset as int;
declare @cur_start_shift_line as int;

declare @cur_new_line_offset as int;
declare @cur_new_by_picket_offset as int;

WHILE @@FETCH_STATUS = 0  
BEGIN  
   
	select @cur_before_picket_label  = Npiketa, @cur_picket_length = Dlina, @cur_start_shift_line = StartShiftLine from Pickets where [number] = @picketID;
	select @cur_equip_offset = shiftFromPicket from [AllEquipments] where Code = @code;
   
	if CHARINDEX(N'-', @cur_before_picket_label) > 0
	begin
		set @cur_new_line_offset = @cur_start_shift_line - (@cur_picket_length - @cur_equip_offset)
		set @cur_new_by_picket_offset = - (@cur_picket_length - @cur_equip_offset);

		PRINT '<---------------------------------------------------->';
		PRINT 'previous offset: ' + cast(@cur_equip_offset as nvarchar(50))+ '. Picket length: ' + cast(@cur_picket_length as nvarchar(50)) + '. Picket offset: ' + cast(@cur_start_shift_line as nvarchar(50)) + '. Picket label: ' + cast(@cur_before_picket_label as nvarchar(50));
		PRINT 'new picket offset' + cast(@cur_new_by_picket_offset as nvarchar(50)) + '. New by line offset: ' + cast(@cur_new_line_offset as nvarchar(50));

		update [AllEquipments] set shiftLine = @cur_new_line_offset, shiftFromPicket = @cur_new_by_picket_offset  where Code = @code and Picket = @picketID;
	end
	else
	begin
		set @cur_new_line_offset = @cur_start_shift_line - (@cur_picket_length - @cur_equip_offset)

		PRINT '<---------------------------------------------------->';
		PRINT 'new picket offset' + cast(@cur_new_by_picket_offset as nvarchar(50)) + '. New by line offset: ' + cast(@cur_new_line_offset as nvarchar(50));

		
		update [AllEquipments] set shiftLine = @cur_new_line_offset  where Code = @code and Picket = @picketID;
	end

	/*if @cur_before_picket_label = N'-0'
	begin
		   PRINT '<---------------------------------------------------->'
		   PRINT '   before picket label:   ' + @cur_before_picket_label + ' picket id: ' + cast(@picketID as nvarchar(50))
		   update [AllEquipments] set Picket = @cur_before_picket_number where Code = @code and Picket = @picketID;
	end*/

	FETCH NEXT FROM vendor_cursor   
	INTO @code, @picketID  
END   
CLOSE vendor_cursor;  
DEALLOCATE vendor_cursor;  