-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[delPeregonFromDB](--/*---CLASS-----*/@ClassCode int,
								  --/*---GROUP-----*/@GroupCode int,
								  --/*---LINE------*/@Line	  int,
								  --/*---TRACK-----*/@Track	  int,
								  --/*---PEREGON---*/@Peregon   int,
								  /*---PICKET----*/@Nperegon   int
								  --@LastOrNot int
								  ) 
	
AS
BEGIN

	declare @before int
	declare @after int

	SELECT @before = NperegonBefore, @after= NperegonAfter FROM Layout WHERE Code = @Nperegon
	DELETE FROM Layout WHERE Code = @Nperegon
	IF @before!=0
	BEGIN
			UPDATE Layout SET NperegonAfter = @after WHERE Code = @before
		IF @after!=0
			UPDATE Layout  SET NperegonBefore = @before WHERE Code = @after
		ELSE
			UPDATE Layout SET NperegonAfter = 0 WHERE Code = @before
	END
	ELSE
		UPDATE Layout SET NperegonBefore = 0 WHERE Code = @after

END