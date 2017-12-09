-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[PicketCreateLeft] (	@NpicketaDisplay nvarchar(50),@Class int,@Group int, @line int,@Path int,
											@peregonNumber int,
											@DlinaPicket int,@afterPicket1 int,@nextPicketBefore int, @LeftShiftLine int, @RightShiftLine int) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;
	
	INSERT INTO Pickets VALUES(@NpicketaDisplay,@DlinaPicket,@peregonNumber,0,@afterPicket1,@nextPicketBefore,@line,@Path,@LeftShiftLine,@RightShiftLine,@Class,@Group)
	UPDATE Pickets SET NpicketBefore = @nextPicketBefore WHERE number = @afterPicket1 
END
