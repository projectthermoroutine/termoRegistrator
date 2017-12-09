-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE deleteTrack (	@num	   int,								  
								@ClassID   int,
								@GroupID   int,
								@LineID    int)
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    -- Insert statements for procedure here
	--DELETE FROM Main WHERE  Track = @num AND [GroupNum]=@GroupID AND LineNum=@LineID
END
