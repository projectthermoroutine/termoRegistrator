-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[delLineFromDB]( /*---LINE ------*/@Line	  int ) 
	
AS
BEGIN
		--DELETE FROM Layout WHERE Line=@Line 
		DELETE FROM Lines WHERE LineCode=@Line 
	
END
