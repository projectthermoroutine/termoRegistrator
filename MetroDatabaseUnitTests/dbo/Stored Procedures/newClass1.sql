-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[newClass1] ( /*---CLASS-----*/@ClassCode int, @ClassName ntext) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    -- Insert statements for procedure here

	--INSERT INTO Class     VALUES(@ClassCode,@ClassName)
	--INSERT INTO Main      VALUES(/*equip*/0,/*Npicket*/0,/*Track*/0,0,0,null,0,0,0,0,/*Peregon*/0,@ClassCode,0,0)
END