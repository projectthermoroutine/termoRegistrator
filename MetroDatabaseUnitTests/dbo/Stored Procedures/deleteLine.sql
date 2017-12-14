-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[deleteLine](/*---CLASS-----*/	@ClassCode int,
									/*---GROUP-----*/	@GroupCode int,
									/*---LINE------*/	@Line	  int									
							   ) 
AS
BEGIN
	SET NOCOUNT ON;

	--
	-- delete from lines table
	--

	DELETE FROM Lines WHERE LineNum = @Line
 
	
	DECLARE @equip_id int
	DECLARE @track_id int
	DECLARE cursor_track_id CURSOR READ_ONLY FOR SELECT ID FROM [dbo].[Track] WHERE LineId = @Line 
	
	OPEN cursor_track_id
	FETCH NEXT FROM cursor_track_id INTO @track_id
	
	WHILE @@FETCH_STATUS = 0
	BEGIN
		
		--
		-- delete pickets 
		--
		DELETE FROM Pickets WHERE [path] = @track_id
		
		--
		-- delete equipments from Main table
		--
		DECLARE cursor_equip_id CURSOR READ_ONLY FOR 
			SELECT code FROM [dbo].[Equipments] WHERE [Path] = @track_id 

		OPEN cursor_equip_id 
			FETCH NEXT FROM cursor_equip_id INTO @equip_id

			WHILE @@FETCH_STATUS = 0
			BEGIN
				DELETE FROM Main WHERE EquipmentId = @equip_id
				FETCH NEXT FROM cursor_equip_id INTO @equip_id
			END

		--
		-- Delete equipment
		--
		DELETE FROM Equipments WHERE [Path] = @track_id

		FETCH NEXT FROM cursor_track_id INTO @track_id
	END

	CLOSE cursor_track_id
	DEALLOCATE cursor_track_id

	--
	-- delete tracks
	--

	DELETE FROM Track WHERE LineId = @Line
	
END