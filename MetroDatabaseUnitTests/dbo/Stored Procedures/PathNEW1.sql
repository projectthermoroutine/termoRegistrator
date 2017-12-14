-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE  PROCEDURE [dbo].[PathNEW1](	/*---CLASS-----*/@ClassCode  int,
									/*---GROUP-----*/@GroupCode  int,
									/*---LINE------*/@Line		 int,
									/*---TRACK-----*/@Track		 nvarchar(50)) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

	declare @firstOrNot int
	SET @firstOrNot = -1
	
	--SELECT @firstOrNot = Track FROM Main WHERE ClassNum = @ClassCode AND GroupNum = @GroupCode AND LineNum = @Line
	SELECT @firstOrNot = Track.ID FROM Track WHERE Track.Track = @Track

	--IF @firstOrNot = 0
	--	UPDATE Main SET Main.Track = @Track WHERE ClassNum = @ClassCode AND GroupNum = @GroupCode AND LineNum = @Line
	--ELSE
	--	INSERT INTO Main      VALUES(/*equip*/0,/*Npicket*/0,/*Track*/@Track,0,0,null,0,0,0,0,/*Peregon*/0,@ClassCode,@GroupCode,@Line)
END