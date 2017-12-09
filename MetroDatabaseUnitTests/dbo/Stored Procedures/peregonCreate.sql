-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[peregonCreate](/*---LINE------*/@Line int,
									  /*---PEREGON---*/@typeOfPeregonCreation int,@peregonNumber int, @afterPeregon1 int, @beforePeregon2 int,@afterPeregon2 int, @beforePeregon3 int, @peregonName nvarchar(100), @ShortPeregonName nvarchar(20), @PeregonLength int) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;
	
	   IF @typeOfPeregonCreation = 9 /*----- создание первого перегона линии ------*/
		INSERT INTO Layout VALUES(@peregonNumber,@peregonName,@ShortPeregonName,@Line,0,0,@PeregonLength)
	   
	   IF @typeOfPeregonCreation = 7 OR @typeOfPeregonCreation = 6 /*---- выбрано создание перегона между перегонами на пути -----*/
	   BEGIN
		UPDATE Layout SET NperegonAfter = @beforePeregon3 WHERE Code = @beforePeregon2
		INSERT INTO Layout VALUES(@beforePeregon3,@peregonName,@ShortPeregonName,@Line,@beforePeregon2,@afterPeregon2,@PeregonLength)
		UPDATE Layout SET NperegonBefore = @afterPeregon1 WHERE Code = @afterPeregon2;
	   END

	   IF @typeOfPeregonCreation = 2 /*---- выбрано создание нового последнего(по порядку следования) перегона в линии ----*/
	   BEGIN
	    UPDATE Layout SET NperegonAfter = @afterPeregon1 WHERE Code = @beforePeregon2
		INSERT INTO Layout VALUES(@afterPeregon1,@peregonName,@ShortPeregonName,@Line,@beforePeregon2,0,@PeregonLength)
	   END

	   IF @typeOfPeregonCreation = 1 /*---- выбрано создание нового первого по (порядку следования) перегона в линии ----- */
	   BEGIN
	    INSERT INTO Layout VALUES(@beforePeregon2,@peregonName,@ShortPeregonName,@Line,0,@afterPeregon1,@PeregonLength)
	    UPDATE Layout SET NperegonBefore = @beforePeregon2 WHERE Code = @afterPeregon1
	   END
END
