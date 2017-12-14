-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[updateStartLineCoordinate](  @lineNumber int,
													 @startlineCoordinate bigint) 
	
AS
BEGIN

	SET NOCOUNT ON;
	
	UPDATE Lines SET StartCoordinate = @startlineCoordinate WHERE LineNum = @lineNumber;

END