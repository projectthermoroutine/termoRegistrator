-- =============================================
-- =============================================
CREATE PROCEDURE [dbo].[delPath]( @num	    int,								  
								 @ClassID   int,
								 @GroupID   int,
								 @LineID    int)
	
AS
BEGIN
	SET NOCOUNT ON;
		
	--BEGIN TRY
	--	BEGIN TRAN;

	--	DECLARE @track_count int
	--	SELECT @track_count = count(Track) FROM Main WHERE Track=@num AND [GroupNum]=@GroupID AND LineNum=@LineID


	--	IF @track_count = 1
	--		UPDATE Main SET Main.Track = 0 WHERE  Track=@num AND [GroupNum]=@GroupID AND LineNum=@LineID
	--	ELSE
	--	BEGIN
	--		DELETE FROM Main WHERE  Track = @num AND [GroupNum]=@GroupID AND LineNum=@LineID
	--		INSERT INTO Main VALUES(0,0,0,0,0,NULL,0,0,0,0,0,@ClassID,@GroupID,@LineID)
	--	END

	--	DELETE FROM [Objects] WHERE [Path] = @num AND [Group]=@GroupID AND Line=@LineID
	--	DELETE FROM Pickets	  WHERE [path] = @num AND [Group]=@GroupID AND Line=@LineID

	--	DECLARE @path int
	--	SELECT  @path = count(Track) From Main where Track=@num AND [GroupNum]!=@GroupID AND LineNum!=@LineID
	
	--	IF (@path=0)
	--	BEGIN
	--		DELETE FROM Track WHERE ID = @num 
	--	END
	--	COMMIT TRAN;
	--	RETURN 0;
	--END  TRY
	--BEGIN CATCH
	--	SELECT XACT_STATE() as 'XACT_STATE', @@TRANCOUNT as '@@TRANCOUNT';
	--	IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION;
	--	THROW;
	--END CATCH;
	
END