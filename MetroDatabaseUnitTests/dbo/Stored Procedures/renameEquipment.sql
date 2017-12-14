-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[renameEquipment](	@equipmentNumber int,
											@equipmentName nvarchar(100)) 
	
AS
BEGIN

	SET NOCOUNT ON;
	
	UPDATE [Objects] SET [Object] = @equipmentName WHERE Code = @equipmentNumber;

END