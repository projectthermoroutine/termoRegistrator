CREATE PROCEDURE [dbo].[DeleteLineSQL](	@ClassCode int,
										@GroupCode int,
										@Line	int	) 
AS

	-- 1. Pickets 
	-- 2. Tracks 
	-- 3. Lines
	-- 4. Main 
	-- 5. Equipments

	DECLARE @TracksId table(ID int NOT NULL);
	INSERT INTO @TracksId SELECT ID FROM Tracks WHERE LineId = @Line

	DELETE FROM Pickets WHERE [Path] IN (SELECT ID FROM  @TracksId)
	DELETE FROM Tracks WHERE LineId = @Line
	DELETE FROM Lines WHERE LineNum = @Line

	DELETE FROM Main WHERE EquipmentId IN 
									(SELECT Code FROM Equipments WHERE [Path] IN 
																			(SELECT ID FROM  @TracksId) )

	DELETE FROM Equipments WHERE [Path] IN (SELECT ID FROM  @TracksId)
	

RETURN 0