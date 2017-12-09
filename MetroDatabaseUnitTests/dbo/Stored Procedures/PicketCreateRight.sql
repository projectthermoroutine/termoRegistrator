-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[PicketCreateRight] ( @NpicketaDisplay nvarchar(50),@Class int,@Group int, @line int,@Path int,
											@peregonNumber int,
											@DlinaPicket int,@addingPicketNumber int,@before int, @LeftShiftLine int, @RightShiftLine int) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

	INSERT INTO Pickets VALUES(@NpicketaDisplay,@DlinaPicket,@peregonNumber,@before,0,@addingPicketNumber,@line,@Path,@LeftShiftLine,@RightShiftLine,@Class,@Group)
	UPDATE Pickets SET NpicketAfter = @addingPicketNumber WHERE number = @before 
END
