﻿-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[line] (/*---CLASS-----*/@ClassCode int, @ClassName char(50),
												/*---GROUP-----*/@GroupCode int, @GroupName char(50),
												/*---LINE------*/@Line int,
												/*---TRACK-----*/@Track int,
												/*---PEREGON---*/@newPeregonRow BIT,@typeOfPeregonCreation int,@peregonNumber int, @beforePeregon1 int, @afterPeregon1 int, @beforePeregon2 int,@afterPeregon2 int, @beforePeregon3 int,@afterPeregon3 int, @peregonName char(50), @ShortPeregonName char(30),
												/*---PICKET----*/@newPicketRow int,@typeOfPicketCreation int, @Npicket int,@DlinaPicket int,@beforePicket1 int,@afterPicket1 int,@beforePicket2 int,@afterPicket2 int,
												/*--EQUIPMENT--*/@newEquipmentRow int, @EquipmentNumber int,/*@GroupNumber = @GroupCode*/ @EquipmentName char(50)) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

	--INSERT INTO  [Class]						VALUES(@ClassCode,@ClassName); 
	--INSERT INTO [Group]([Code],[Class],[Group]) VALUES(@GroupCode,@ClassCode,@GroupName);
	--UPDATE Layout SE 
	--

	--declare @newPeregonNumber int

	--IF (@newPeregonRow = 1)	
	--BEGIN
	--   IF @typeOfPeregonCreation = 7 /*---- выбрано создание перегона между крайними перегодами на пути -----*/
	--   BEGIN
	--	UPDATE Layout SET NperegonAfter = @beforePeregon3 WHERE Code = @beforePeregon2
	--	INSERT INTO Layout VALUES(@beforePeregon3,@peregonName,@ShortPeregonName,@Line,@beforePeregon2,@afterPeregon2)
	--	UPDATE Layout SET NperegonBefore = @afterPeregon1 WHERE Code = @afterPeregon2;
	--   END

	--   IF @typeOfPeregonCreation = 6 /*---- выбрано создание перегона между первыми перегонами на пути -----*/
	--   BEGIN
	--    UPDATE Layout SET NperegonAfter = @beforePeregon3 WHERE Code = @beforePeregon2
	--	INSERT INTO Layout VALUES(@beforePeregon3,@peregonName,@ShortPeregonName,@Line,@beforePeregon2,@afterPeregon2)
	--	UPDATE Layout SET NperegonBefore = @afterPeregon1 WHERE Code = @afterPeregon2
	--   END

	--   IF @typeOfPeregonCreation = 2 /*---- выбрано создание нового последнего перегона в линии ----*/
	--   BEGIN
	--    UPDATE Layout SET NperegonAfter = @afterPeregon1 WHERE Code = @beforePeregon2
	--	INSERT INTO Layout VALUES(@afterPeregon1,@peregonName,@ShortPeregonName,@Line,@beforePeregon2,0)
	--   END

	--   IF @typeOfPeregonCreation = 1 /*---- выбрано создание нового первого перегона в линии ----- */
	--   BEGIN
	--    INSERT INTO Layout VALUES(@beforePeregon2,@peregonName,@ShortPeregonName,@Line,0,@afterPeregon1)
	--    UPDATE Layout SET NperegonBefore = @beforePeregon2 WHERE Code = @afterPeregon1
	--   END
	--END
	
	--IF @newPicketRow = 1
	--BEGIN
	--	IF @typeOfPicketCreation=0 
	--	BEGIN
	--		INSERT INTO Pickets VALUES(@beforePicket2,@DlinaPicket,@peregonNumber,0,@afterPicket1)
	--		UPDATE Pickets SET NpicketBefore = @beforePicket2 WHERE Npiketa = @afterPicket1 
	--	END
	--	IF @typeOfPicketCreation=1
	--	BEGIN
	--		INSERT INTO Pickets VALUES(@afterPicket1,@DlinaPicket,@peregonNumber,@beforePicket2,0)
	--	END
	--END
	--IF @newEquipmentRow = 1
	--	INSERT INTO [Objects] VALUES (@EquipmentNumber,@GroupCode,@EquipmentName) 
  
	----INSERT INTO Main VALUES(@EquipmentNumber,@Npicket,@Track,0,0,null,0,0,0,0,@peregonNumber)
	--UPDATE Main SET Layout = @peregonNumber WHERE Code = @beforePeregon2
END
