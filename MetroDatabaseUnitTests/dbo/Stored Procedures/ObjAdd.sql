-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[ObjAdd] (/*---CLASS------*/@ClassCode int,
								 /*---GROUP-----*/@GroupCode int,
								 /*---LINE------*/@Line int,
								 /*---TRACK-----*/@Track int,
								 /*---PEREGON---*/@peregonNumber int,
								 /*---PICKET----*/@Npicket int,
								 /*---EQUIP-----*/@EquipmentNum int) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;


	declare @firstOrNot int
	--SET @firstOrNot=0
	--SELECT @firstOrNot=Code FROM Main WHERE Npicket=@Npicket AND  Track=@Track AND ClassNum = @ClassCode AND GroupNum=@GroupCode AND LineNum=@Line
		
	--/*   MAIN   */
	--IF @firstOrNot!=0
	--	INSERT INTO Main VALUES(@EquipmentNum, @Npicket,@Track,0,0,null,0,0,0,0,@peregonNumber,@ClassCode,@GroupCode,@Line)
	--ELSE
	--	UPDATE Main SET Code = @EquipmentNum WHERE Npicket=@Npicket  AND Track=@Track AND ClassNum = @ClassCode AND GroupNum=@GroupCode AND LineNum=@Line
END