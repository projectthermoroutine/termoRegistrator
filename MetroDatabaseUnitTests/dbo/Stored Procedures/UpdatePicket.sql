-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[UpdatePicket] (	@EquipNum int,
										@leftShift int,
										@rightShift int,
										@lenght int
									  ) 	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    -- Insert statements for procedure here
	UPDATE [dbo].[Pickets] SET StartShiftLine = @leftShift, EndShiftLine =@rightShift, Dlina = @lenght  WHERE number = @EquipNum
END
