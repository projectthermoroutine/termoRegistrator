-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[PicketCreate] (	 @NpicketaDisplay nvarchar(50), @line int,@Path int,
										 @peregonNumber int,
										 @typeOfPicketCreation int, @Npicket int, @DlinaPicket int,@afterPicket1 int,@beforePicket2 int, @LeftShiftLine int, @RightShiftLine int) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;
	
	IF @typeOfPicketCreation=0
	BEGIN
		--INSERT INTO Pickets VALUES(@NpicketaDisplay,@DlinaPicket,@peregonNumber,0,@afterPicket1,@beforePicket2,@line,@Path,@LeftShiftLine,@RightShiftLine)
		UPDATE Pickets SET NpicketBefore = @Npicket WHERE number = @afterPicket1 
	END
	IF @typeOfPicketCreation=1
	BEGIN
		--INSERT INTO Pickets(Dlina) VALUES('1');
		--INSERT INTO Pickets VALUES(@NpicketaDisplay,@DlinaPicket,@peregonNumber,@beforePicket2,0,@afterPicket1,@line,@Path,@LeftShiftLine,@RightShiftLine)
		UPDATE Pickets SET NpicketAfter = @Npicket WHERE number = @beforePicket2 ;
	END
	IF @typeOfPicketCreation=3
		--INSERT INTO dbo.Pickets VALUES( @NpicketaDisplay,@DlinaPicket,@peregonNumber,0,0,@Npicket,@line,@Path,@LeftShiftLine,@RightShiftLine);
		INSERT INTO dbo.Pickets(Dlina) VALUES(1);
END