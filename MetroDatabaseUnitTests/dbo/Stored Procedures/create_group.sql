-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[create_group]( @ClassCode int,
								      @GroupCode int, @GroupName nvarchar(20),@Color nvarchar(10)
									) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;
	--BEGIN TRAN;

	--	INSERT INTO [Group]([Code],[Class],[Group],[Color]) VALUES(@GroupCode,@ClassCode,@GroupName,@Color)
	
	--	DECLARE @count as int;
	--	SELECT @count = COUNT(Track) FROM Main WHERE ClassNum = @ClassCode AND GroupNum = @GroupCode

	--	IF @count = 1
	--		UPDATE Main SET Main.GroupNum = @GroupCode WHERE ClassNum = @ClassCode AND GroupNum = @GroupCode
	--	ELSE
	--		INSERT INTO Main      VALUES(/*equip*/0,/*Npicket*/0,/*Track*/0,0,0,null,0,0,0,0,/*Peregon*/0,@ClassCode,@GroupCode,0)

	--COMMIT TRAN;
END