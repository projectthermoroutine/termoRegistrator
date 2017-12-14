-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[delPathFromMainTable](	@Track	  int,								  
												@lastOrNot int
											) 
	
AS
BEGIN

	SET NOCOUNT ON;
	--IF @LastOrNot = 1
	--	UPDATE Main SET Main.Track = 0 WHERE  Track=@Track
	--ELSE
	--	DELETE FROM Main WHERE  Track=@Track 

	--DELETE FROM Track WHERE ID=@Track
	
END