-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[delClass](  @ClassCode int, @error_msg nvarchar(1000) OUT ) 
	
AS
BEGIN
	declare @aaa int
	--BEGIN TRY
	--	BEGIN TRAN;
	--		DECLARE @group_id int
	--		DECLARE cursor_group_id CURSOR READ_ONLY FOR 
	--		SELECT LineNum FROM Main WHERE ClassNum = @ClassCode 
	
	--		OPEN cursor_group_id
	--		FETCH NEXT FROM cursor_group_id INTO @group_id
	
	--		WHILE @@FETCH_STATUS = 0
	--		BEGIN 
	--			exec [dbo].[delGroup] @ClassCode,@group_id,@error_msg
	--			FETCH NEXT FROM cursor_group_id INTO @group_id
	--		END

	--		CLOSE cursor_group_id
	--		DEALLOCATE cursor_group_id

	--		DELETE FROM Main WHERE ClassNum = @ClassCode  
	--		DELETE FROM [Class] WHERE Code = @ClassCode
	--		COMMIT TRAN;
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
