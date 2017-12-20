-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[AddLine](	@LineNum    int,
									@LineName nvarchar(50),
									@StartCoordinat bigint,
									@LineCode varchar(100)
								) 
	
AS
BEGIN
	
	SET NOCOUNT ON;

	INSERT INTO Lines(LineNum,LineName,StartCoordinate,LineCode)  VALUES(@LineNum, @LineName, @StartCoordinat, @LineCode)
END