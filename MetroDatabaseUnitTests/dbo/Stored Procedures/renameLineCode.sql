-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[renameLineCode](  @lineNumber int,
										  @lineCode nvarchar(100)) 
	
AS
BEGIN

	SET NOCOUNT ON;
	
	UPDATE Lines SET LineCode = @lineCode WHERE LineNum = @lineNumber;

END
