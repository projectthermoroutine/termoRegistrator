-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[delGroup]( @ClassCode int,
								  @GroupNum	  int,
								  @error_msg nvarchar(1000) OUT	  
								) 
	
AS
BEGIN
	declare @aaa int
	--BEGIN TRY
	--	BEGIN TRAN;

	--		DECLARE @line_id int
	--		DECLARE cursor_line_id CURSOR READ_ONLY FOR 
	--		SELECT LineNum FROM Main WHERE GroupNum=@GroupNum 
	
	--		OPEN cursor_line_id
	--		FETCH NEXT FROM cursor_line_id INTO @line_id
	
	--		WHILE @@FETCH_STATUS = 0
	--		BEGIN 
	--			exec [dbo].[delLine]  @ClassCode,@GroupNum,@line_id, @error_msg
	--			FETCH NEXT FROM cursor_line_id INTO @line_id
	--		END

	--		CLOSE cursor_line_id
	--		DEALLOCATE cursor_line_id

	--		DECLARE @group_count int
	--		SELECT @group_count = count(Main.LineNum) FROM Main WHERE GroupNum = @GroupNum AND ClassNum = @ClassCode

	--		IF(@group_count=1)
	--			UPDATE Main SET [Main].[GroupNum] = 0 WHERE  ClassNum=@ClassCode AND [GroupNum]=@GroupNum

	--		SELECT  @group_count = count(GroupNum) From Main where [GroupNum]=@GroupNum AND ClassNum!=@ClassCode
	
	--		IF (@group_count = 0)
	--		BEGIN
	--			DELETE FROM [Group] WHERE Code = @GroupNum 
	--		END

	--	COMMIT TRAN;
	--	RETURN (0);
	--END TRY
	--BEGIN CATCH
	--	SELECT XACT_STATE() as 'XACT_STATE', @@TRANCOUNT as '@@TRANCOUNT';
	--	IF @@TRANCOUNT > 1
	--	BEGIN
	--		ROLLBACK TRANSACTION;
	--		THROW;
	--	END

	--	SET @error_msg = ERROR_MESSAGE();
	--END CATCH;
END
