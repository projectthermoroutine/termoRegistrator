-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[insertStoredProcedure3](/*---CLASS-----*/@ClassCode int,
												/*---GROUP-----*/@GroupCode int,
												/*---LINE------*/@Line int,
												/*---TRACK-----*/@Track int,
												/*---PEREGON---*/@peregonNumber int) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

	declare @firstOrNorPeregon int
	SET @firstOrNorPeregon=0
	--SELECT @firstOrNorPeregon=Layout FROM Main  WHERE Track=@Track AND ClassNum = @ClassCode AND GroupNum=@GroupCode AND LineNum=@Line

	/*IF (@newPeregonRow = 1)	
	BEGIN
	   IF @typeOfPeregonCreation = 8/*---- выбрано создание нового перегона линии -----*/
	   BEGIN
		IF @firstOrNorPeregon=0
			INSERT INTO Layout VALUES(@peregonNumber,@peregonName,@ShortPeregonName,@Line,0,0)
		ELSE
			UPDATE Layout SET Code = @peregonNumber, Layout=@peregonName,SName = @ShortPeregonName,NperegonBefore= 0,NperegonAfter=0 WHERE Line=@Line
	   END
	   IF @typeOfPeregonCreation = 7 /*---- выбрано создание перегона между крайними перегодами на пути -----*/
	   BEGIN
		UPDATE Layout SET NperegonAfter = @beforePeregon3 WHERE Code = @beforePeregon2
		INSERT INTO Layout VALUES(@beforePeregon3,@peregonName,@ShortPeregonName,@Line,@beforePeregon2,@afterPeregon2)
		UPDATE Layout SET NperegonBefore = @afterPeregon1 WHERE Code = @afterPeregon2;
	   END

	   IF @typeOfPeregonCreation = 6 /*---- выбрано создание перегона между первыми перегонами на пути -----*/
	   BEGIN
	    UPDATE Layout SET NperegonAfter = @beforePeregon3 WHERE Code = @beforePeregon2
		INSERT INTO Layout VALUES(@beforePeregon3,@peregonName,@ShortPeregonName,@Line,@beforePeregon2,@afterPeregon2)
		UPDATE Layout SET NperegonBefore = @afterPeregon1 WHERE Code = @afterPeregon2
	   END

	   IF @typeOfPeregonCreation = 2 /*---- выбрано создание нового последнего перегона в линии ----*/
	   BEGIN
	    UPDATE Layout SET NperegonAfter = @afterPeregon1 WHERE Code = @beforePeregon2
		INSERT INTO Layout VALUES(@afterPeregon1,@peregonName,@ShortPeregonName,@Line,@beforePeregon2,0)
	   END

	   IF @typeOfPeregonCreation = 1 /*---- выбрано создание нового первого перегона в линии ----- */
	   BEGIN
	    INSERT INTO Layout VALUES(@beforePeregon2,@peregonName,@ShortPeregonName,@Line,0,@afterPeregon1)
	    UPDATE Layout SET NperegonBefore = @beforePeregon2 WHERE Code = @afterPeregon1
	   END
	END*/

	--IF @firstOrNorPeregon!=0
	--	INSERT INTO Main VALUES(0,0,@Track,0,0,null,0,0,0,0,@peregonNumber,@ClassCode,@GroupCode,@Line)
	--ELSE
	--	UPDATE Main SET Layout=@peregonNumber WHERE Track=@Track AND ClassNum = @ClassCode AND GroupNum=@GroupCode AND LineNum=@Line
END
