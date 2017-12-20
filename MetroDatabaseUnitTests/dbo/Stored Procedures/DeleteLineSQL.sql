CREATE PROCEDURE [dbo].[DeleteLineSQL](	@Line	int	) 
AS

	-- 1. Pickets 
	-- 2. Tracks 
	-- 3. Lines
	-- 4. Main 
	-- 5. Equipments
	BEGIN TRY
		BEGIN TRAN;
			DECLARE @TracksId table(ID int NOT NULL);
			INSERT INTO @TracksId SELECT ID FROM Tracks WHERE LineId = @Line

			DELETE FROM Pickets WHERE [Path] IN (SELECT ID FROM  @TracksId)
			DELETE FROM Tracks WHERE LineId = @Line
			DELETE FROM Lines WHERE LineNum = @Line

			DELETE FROM Main WHERE EquipmentId IN 
											(SELECT Code FROM Equipments WHERE [Path] IN 
																					(SELECT ID FROM  @TracksId) )

			DELETE FROM Equipments WHERE [Path] IN (SELECT ID FROM  @TracksId)
		COMMIT TRAN;
		RETURN (1);
	END TRY
	BEGIN CATCH
		--SELECT XACT_STATE() as 'XACT_STATE', @@TRANCOUNT as '@@TRANCOUNT';
			--IF @@TRANCOUNT > 1
			--BEGIN
				--ROLLBACK TRANSACTION;
				--THROW;
			--END

			--SET @error_msg = ERROR_MESSAGE();
	END CATCH;

RETURN 0