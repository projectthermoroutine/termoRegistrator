-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>	

-- =============================================
CREATE PROCEDURE [dbo].[UpdatePositivePickets](@pNum int, @delta int, @lenght int)
AS
BEGIN
	SET NOCOUNT ON;
	BEGIN TRANSACTION
	
		DECLARE @RShift int, @LShift int
		DECLARE @cur_picket int
	
		SET @cur_picket = @pNum

		SELECT @RShift = EndShiftLine FROM [dbo].[Pickets] WHERE number = @pNum
		UPDATE [dbo].[Pickets] SET EndShiftLine = (@RShift - @delta) , Dlina = @lenght WHERE number = @cur_picket

		SELECT @cur_picket = NpicketAfter FROM Pickets WHERE number = @cur_picket
	
		IF @cur_picket=0 RETURN

		WHILE 1=1
		BEGIN 
			SELECT @LShift = StartShiftLine, @RShift = EndShiftLine FROM [dbo].[Pickets] WHERE number = @cur_picket 
			UPDATE [dbo].[Pickets] SET StartShiftLine = (@LShift-@delta) , EndShiftLine = (@RShift-@delta) WHERE number = @cur_picket

			exec updateObjectsOffsets @cur_picket,@delta
		
			SELECT @cur_picket = NpicketAfter FROM Pickets WHERE number = @cur_picket

			IF @cur_picket=0 BREAK
		END

	COMMIT TRANSACTION
END
