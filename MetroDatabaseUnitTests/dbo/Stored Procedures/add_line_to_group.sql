-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[add_line_to_group](	@ClassCode  int,
											@GroupCode  int,
											@Line		 int
										 ) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

	declare @firstOrNot int
	SET @firstOrNot = -1
	--SELECT @firstOrNot = LineNum FROM Main WHERE ClassNum = @ClassCode AND GroupNum = @GroupCode

	--declare @maxLayout int

	--IF @firstOrNot = 0
	--	UPDATE Main SET Main.LineNum = @Line WHERE ClassNum = @ClassCode AND GroupNum = @GroupCode
	--ELSE
	--	INSERT INTO Main      VALUES(/*equip*/0,/*Npicket*/0,/*Track*/0,0,0,null,0,0,0,0,/*Peregon*/0,@ClassCode,@GroupCode,@Line)
END
