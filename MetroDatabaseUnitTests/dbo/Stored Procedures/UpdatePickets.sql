-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE UpdatePickets(@pNum int, @delta int)
AS
BEGIN
	SET NOCOUNT ON;
	
	DECLARE @next_picket int
	DECLARE @while_stop bit
	SET @while_stop = 1	
	
	WHILE @while_stop=1
	BEGIN 
		SELECT @next_picket = number FROM Pickets WHERE NpicketAfter = @next_picket
	END
END