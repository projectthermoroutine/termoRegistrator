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
		@offset_from_picket int,
		@picketID int,
		@picketBefore int

DECLARE vendor_cursor CURSOR FOR   
SELECT [Code], [Picket], [shiftFromPicket] FROM [AllEquipments];

OPEN vendor_cursor  

FETCH NEXT FROM vendor_cursor   
INTO @code, @picketID, @offset_from_picket  

declare @begin_picket_offset int;
declare @end_picket_offset int;

declare @picket_label as nvarchar(50);
declare @picket_length as int;

declare @new_line_offset as int;
declare @new_from_picket_offset as int;

declare @basic_picket_offset int;


WHILE @@FETCH_STATUS = 0  
BEGIN  
   
	select  @picket_label = Npiketa,
			@picket_length = Dlina, 
			@begin_picket_offset = StartShiftLine,
			@end_picket_offset = EndShiftLine 
	from Pickets 
	where [number] = @picketID;

  
	if @begin_picket_offset < 0 or @end_picket_offset < 0
	begin
		
		if @offset_from_picket <= 0
		begin
			set @new_from_picket_offset = - (@picket_length + @offset_from_picket);
			set @new_line_offset = @end_picket_offset + @new_from_picket_offset;

			PRINT '<---------------------------------------------------->';
			PRINT 'previous offset: ' + cast(@offset_from_picket as nvarchar(50))+ '. Picket length: ' + cast(@picket_length as nvarchar(50)) + '. Picket offset: ' + cast(@begin_picket_offset as nvarchar(50)) + '. Picket label: ' + cast(@picket_label as nvarchar(50));
			PRINT 'new picket offset: ' + cast(@new_from_picket_offset as nvarchar(50)) + '. New by line offset: ' + cast(@new_line_offset as nvarchar(50));

			update [AllEquipments] set shiftLine = @new_line_offset, shiftFromPicket = @new_from_picket_offset  where Code = @code and Picket = @picketID;
		end
		else
		begin
			PRINT '<----- Invalid object offset --------->';
			PRINT 'offset: ' + cast(@offset_from_picket as nvarchar(50))+ '. object code: ' + cast(@code as nvarchar(50)) + '. Picket label: ' + cast(@picket_label as nvarchar(50));
		end

	end


	FETCH NEXT FROM vendor_cursor   
	INTO @code, @picketID, @offset_from_picket 
END   
CLOSE vendor_cursor;  
DEALLOCATE vendor_cursor;  