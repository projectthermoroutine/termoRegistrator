-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[UpdateObjectFramesArea] (	@EquipNum int,
											@Time datetime,
											@X int,
											@Y int,
											@Height int,
											@Width  int,
											@AreaType int
									  ) 	
AS
BEGIN
	
	SET NOCOUNT ON;
	  
	UPDATE [dbo].[ObjectsFrames] SET Area_X = @X, Area_Y = @Y, Area_Height = @Height, Area_Width = @Width, Area_Type = @AreaType  WHERE [ObjID] = @EquipNum AND [Time]=@Time
END
