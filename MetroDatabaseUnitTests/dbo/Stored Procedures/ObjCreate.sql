-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[ObjCreate] (@Class int,
								   @GroupCode int,
								   @Line int,
								   @Path int,
								   @Picket int,
								   @EquipmentNum int,@EquipmentName varchar(50),@shift bigint, @maxEquipTemperature int, @X int, @Y int, @curTemperature int, @regularlyOrNot int,@shiftFromPicket int,@typeObjIndex int, @objectLenght int, @typeEquip int, @StrelkaLeftOrRight int) 
	
AS
BEGIN
	SET NOCOUNT ON;
	INSERT INTO [Objects] VALUES(@EquipmentNum, @GroupCode,@EquipmentName,@shift,@X,@Y, @curTemperature,@maxEquipTemperature,@regularlyOrNot,@shiftFromPicket,@typeObjIndex, @typeEquip, @StrelkaLeftOrRight,@Picket,@Path,@Line,@Class,@objectLenght,NULL,null,null,null,null)
	
END