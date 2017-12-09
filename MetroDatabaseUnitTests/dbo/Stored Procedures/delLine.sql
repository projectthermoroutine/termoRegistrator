-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[delLine](/*---CLASS-----*/@ClassCode int,
								/*---GROUP-----*/@GroupCode int,
								/*---LINE------*/@Line	  int,
								 @error_msg nvarchar(1000) OUT	
							   ) 
AS
BEGIN
	declare @aaa int
	--BEGIN TRY
	--	BEGIN TRAN;

	--	DECLARE @track_id int
	--	DECLARE cursor_track_id CURSOR READ_ONLY FOR 
	--	SELECT Track FROM Main WHERE LineNum=@Line 
	
	--	OPEN cursor_track_id
	--	FETCH NEXT FROM cursor_track_id INTO @track_id
	
	--	WHILE @@FETCH_STATUS = 0
	--	BEGIN 
	--		exec [dbo].[delPath] @track_id, @ClassCode, @GroupCode, @Line;
	--		FETCH NEXT FROM cursor_track_id INTO @track_id
	--	END

	--	CLOSE cursor_track_id
	--	DEALLOCATE cursor_track_id

	--	DECLARE @lines_count int
	--	SELECT @lines_count = count(Main.LineNum) FROM Main WHERE LineNum = @Line AND GroupNum = @GroupCode AND ClassNum = @ClassCode

	--	IF(@lines_count=1)
	--		UPDATE Main SET [Main].[LineNum] = 0 WHERE  ClassNum=@ClassCode AND [GroupNum]=@GroupCode AND LineNum=@Line
	--	ELSE
	--		DELETE FROM Main WHERE ClassNum=@ClassCode AND [GroupNum]=@GroupCode AND LineNum=@Line
	
	--	SELECT  @lines_count = count(LineNum) From Main where LineNum=@Line AND [GroupNum]!=@GroupCode AND ClassNum=@ClassCode
	
	--	IF (@lines_count = 0)
	--	BEGIN
	--		DELETE FROM Lines WHERE LineNum = @Line 
	--	END

	--	COMMIT TRAN;
	--	RETURN 0;
	--END TRY
	--BEGIN CATCH
	--	SELECT XACT_STATE() as 'XACT_STATE', @@TRANCOUNT as '@@TRANCOUNT';
	--	IF @@TRANCOUNT > 1
	--	BEGIN
	--		ROLLBACK TRANSACTION;
	--		THROW;
	--	END

	--	SET @error_msg = ERROR_MESSAGE();
		
		
	--	THROW;
	--END CATCH;
END

