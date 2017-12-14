-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[insertFilter](	/*---FILTER_ID--*/@Filter_ID int,
										/*---CLASS------*/@ClassCode  int, @ClassName nvarchar(50),
										/*---GROUPS-----*/@GroupsCodes  nvarchar(MAX), @GroupsNames nvarchar(MAX),
										/*---STATUS-----*/@Status int
									) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

	/*declare @firstOrNot int
	SET @firstOrNot = -1
	SELECT @firstOrNot = LineNum FROM Main WHERE ClassNum = @ClassCode AND GroupNum = @GroupCode

	declare @maxLayout int

	--IF @newLineOrNot=0
		--INSERT INTO Layout VALUES(0,'notExist','NE',@Line,0,0)
	IF @firstOrNot = 0
		UPDATE Main SET Main.LineNum = @Line WHERE ClassNum = @ClassCode AND GroupNum = @GroupCode
	ELSE
		INSERT INTO Main      VALUES(/*equip*/0,/*Npicket*/0,/*Track*/0,0,0,null,0,0,0,0,/*Peregon*/0,@ClassCode,@GroupCode,@Line)*/

	--DELETE  FROM EquipmentFilter_Tbl
	INSERT INTO EquipmentFilter_Tbl VALUES(@Filter_ID, @ClassCode,@ClassName,@GroupsNames,@GroupsCodes,@Status)
	

END