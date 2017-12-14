-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[PicketCreateFirst] (@NpicketaDisplay nvarchar(50),@Class int,@Group int, @line int,@Path int,
											@peregonNumber int,
											@Npicket int,@DlinaPicket int,@afterPicket1 int,@beforePicket2 int, @LeftShiftLine int, @RightShiftLine int) 
	
AS
BEGIN

	SET NOCOUNT ON;
	
	--INSERT INTO Pickets VALUES( @NpicketaDisplay,@DlinaPicket,@peregonNumber,0,0,@Npicket,@line,@Path,@LeftShiftLine,@RightShiftLine,@Class,@Group)
END