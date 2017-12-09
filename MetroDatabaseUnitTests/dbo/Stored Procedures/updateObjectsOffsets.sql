-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[updateObjectsOffsets] ( @pNum int, @delta int)
AS
BEGIN
	DECLARE @obj_num int, @offset int
	DECLARE cursor_objectID CURSOR READ_ONLY FOR
	SELECT Code, shiftLine FROM [Objects] WHERE Picket = @pNum
	
	OPEN  cursor_objectID
	FETCH NEXT FROM cursor_objectID INTO @obj_num, @offset
	
	WHILE @@FETCH_STATUS = 0
	BEGIN 
		UPDATE [Objects] SET shiftLine = (@offset - @delta) WHERE Code = @obj_num
		FETCH NEXT FROM cursor_objectID INTO @obj_num, @offset
	END

	CLOSE cursor_objectID
	DEALLOCATE cursor_objectID
END
