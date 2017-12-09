-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[UpdateObjectArea] (	@EquipNum int,
											@X int,
											@Y int,
											@Height int,
											@Width  int,
											@AreaType int
									  ) 	
AS
BEGIN
	
	SET NOCOUNT ON;
	  
	UPDATE [dbo].[Objects] SET Area_X = @X, Area_Y = @Y, Area_Height = @Height, Area_Width = @Width, Area_Type = @AreaType  WHERE Code = @EquipNum
END
