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

DECLARE @eq_minus table(code int);

insert into @eq_minus  select distinct [Code] from [AllEquipments]  where shiftFromPicket < 0;

declare @counter as int;
select @counter = count([Code]) from @eq_minus;


DECLARE @code int,  
		--@EquipID int,
		--@EquipTypeID int,
		@picketID int,
		@picketBefore int

DECLARE vendor_cursor CURSOR FOR   
SELECT [Code], [Picket] FROM [AllEquipments] where shiftFromPicket < 0;

OPEN vendor_cursor  

FETCH NEXT FROM vendor_cursor   
INTO @code, @picketID  

declare @cur_before_picket_number int;
declare @cur_before_picket_label as nvarchar(50);

WHILE @@FETCH_STATUS = 0  
BEGIN  
   
   select @cur_before_picket_number = [NpicketBefore] from Pickets where [number] = @picketID;
   select @cur_before_picket_label = Npiketa from Pickets where [number] = @cur_before_picket_number;
   
   if @cur_before_picket_label = N'-0'
   begin
	   PRINT '<---------------------------------------------------->'
	   PRINT '   before picket label:   ' + @cur_before_picket_label + ' picket id: ' + cast(@picketID as nvarchar(50))
	   update [AllEquipments] set Picket = @cur_before_picket_number where Code = @code and Picket = @picketID;
   end

   FETCH NEXT FROM vendor_cursor   
   INTO @code, @picketID  
END   
CLOSE vendor_cursor;  
DEALLOCATE vendor_cursor;  