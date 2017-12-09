-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[PicketAdd] (/*---CLASS-----*/@ClassCode int,
								 /*---GROUP-----*/@GroupCode int,
								 /*---LINE------*/@Line int,
								 /*---TRACK-----*/@Track int,
								 /*---PEREGON---*/@peregonNumber int,
								 /*---PICKET----*/@Npicket int) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;


	declare @firstOrNorPicket int
	SET @firstOrNorPicket=0
	--SELECT @firstOrNorPicket=Npicket FROM Main WHERE  Track=@Track AND ClassNum = @ClassCode AND GroupNum=@GroupCode AND LineNum=@Line AND Layout = @peregonNumber

	--/*   MAIN   */
	--IF @firstOrNorPicket!=0
	--	INSERT INTO Main VALUES(0, @Npicket,@Track,0,0,null,0,0,0,0,@peregonNumber,@ClassCode,@GroupCode,@Line)
	--ELSE
	--	UPDATE Main SET Npicket = @Npicket WHERE  Track=@Track AND ClassNum = @ClassCode AND GroupNum=@GroupCode AND LineNum=@Line AND Layout = @peregonNumber
END
