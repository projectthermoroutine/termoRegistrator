-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[insertEquipTemperature] (/*-----EQUIPMENT------*/@EquipCode int,
												 /*---CurTemperature---*/@curTemperature int
												) 
	
AS
BEGIN
	SET NOCOUNT ON;
	UPDATE [Objects] SET [Objects].curTemperature=@curTemperature WHERE [Objects].Code = @EquipCode
END