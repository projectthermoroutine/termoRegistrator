-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[testChild]
	
AS
BEGIN
	SET NOCOUNT ON;
	BEGIN TRAN;

		DECLARE @track_count AS int;
		DECLARE @er_msg AS NVARCHAR(1000), @error_severity AS INT;
		SELECT @er_msg = ERROR_MESSAGE(),
		@error_severity = ERROR_SEVERITY();
		
		--SELECT @track_count = count(Track) FROM Main ;

		

		--RAISERROR(@er_msg,@error_severity,1)
		--ROLLBACK;
		
	COMMIT TRAN;
END