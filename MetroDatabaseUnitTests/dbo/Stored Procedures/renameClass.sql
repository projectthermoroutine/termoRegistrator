-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[renameClass](  @ClassNumber int,
									   @ClassName nvarchar(20)) 
	
AS
BEGIN

	SET NOCOUNT ON;
	
	UPDATE Class SET Class = @ClassName WHERE Code = @ClassNumber;

END
