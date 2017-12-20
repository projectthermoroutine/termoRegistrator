-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================

CREATE PROCEDURE [dbo].[delEquipAsType]( @EquipCode int   ) 
	
AS
BEGIN
	DECLARE @equipmentType int

	SELECT @equipmentType=typeEquip FROM Equipments WHERE Code =  @EquipCode

	DECLARE @EquipId table (ID int NOT NULL);

	-- get all equipments of target type
	INSERT INTO @EquipId SELECT Code FROM Equipments WHERE typeEquip = @equipmentType
	
	DELETE FROM Main WHERE EquipmentId IN (SELECT ID FROM @EquipId)
	DELETE FROM Equipments WHERE Code IN(SELECT ID FROM @EquipId)
	DELETE FROM EquipmentsTypes WHERE Id = @equipmentType

END