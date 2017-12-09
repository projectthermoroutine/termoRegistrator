-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[testParent]
	
AS
BEGIN
	SET NOCOUNT ON;
	
	
			--exec [dbo].[testChild];

			INSERT INTO Test_table_1 VALUES(1);
			--throw 50000,'ErrorMy',0;
			--IF @@TRANCOUNT <> 0 ROLLBACK TRANSACTION;
			--SELECT XACT_STATE() as 'XACT_STATE', @@TRANCOUNT as '@@TRANCOUNT';
		DECLARE @res as int;
		SET @res=1;
		SELECT @res as 'result';
		return 3;

	
END
