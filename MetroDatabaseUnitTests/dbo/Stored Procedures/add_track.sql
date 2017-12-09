-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[add_track] ( @track_code int, @track_name ntext) 
	
AS
BEGIN
	SET NOCOUNT ON;

	INSERT INTO Track VALUES(@track_code,@track_name)
END
