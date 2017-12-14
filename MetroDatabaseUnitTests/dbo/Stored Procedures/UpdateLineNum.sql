-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [UpdateLineNum] (
										@lineNumNew int,
										@lineNum int
									) 	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    -- Insert statements for procedure here
	--UPDATE [dbo].[Main] SET LineNum = @lineNumNew  WHERE LineNum=@lineNum
END