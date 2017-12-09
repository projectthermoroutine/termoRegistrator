-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================

-- =============================================
CREATE PROCEDURE [dbo].[renameTrack] (	/* TRACK */@Track   int, @selectedTrack int,
										/* CLASS */@ClassID int,
										/* GROUP */@GroupID int,
										/* LINE  */@LineID  int) 
	
AS
BEGIN
	SET NOCOUNT ON;

	--UPDATE   [Main] SET  [Track] = @Track	WHERE  (ClassNum = @ClassID AND GroupNum = @GroupID AND LineNum = @LineID AND Track = @selectedTrack)
END
