-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[updateOrder] (
										@ID int,
										@Person nvarchar(MAX),
										@Desc   nvarchar(MAX),
										@CreationDate DATE,
										@FirstDate DATE,
										@FinishDate DATE,
										@State int,
										@result int OUT
									) 	
AS
BEGIN
	SET NOCOUNT ON;
	BEGIN TRAN
		BEGIN TRY
			UPDATE [dbo].[Orders] SET [Orders].Person = @Person, [Orders].[Desc] = @Desc, [Orders].CreationDate = @CreationDate, [Orders].FirstDate = @FirstDate, [Orders].FinishDate=@FinishDate,[Orders].[State] = @State   WHERE [Orders].ID = @ID
		END TRY
		BEGIN CATCH
			SELECT 
			ERROR_NUMBER() AS ErrorNumber
			,ERROR_SEVERITY() AS ErrorSeverity
			,ERROR_STATE() AS ErrorState
			,ERROR_PROCEDURE() AS ErrorProcedure
			,ERROR_LINE() AS ErrorLine
			,ERROR_MESSAGE() AS ErrorMessage;

			IF @@TRANCOUNT > 0
			BEGIN
				ROLLBACK TRANSACTION;
				SET	@result = 0
			END
		END CATCH;

	IF @@TRANCOUNT > 0
	BEGIN
		COMMIT TRANSACTION;
	    SET	@result = 1
	END
END