-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[setPeregonLength](   @peregonNumber int,
											@PeregonLength int) 
AS
BEGIN

	SET NOCOUNT ON;
	
	UPDATE Layout SET PeregonLength = @PeregonLength WHERE Code = @peregonNumber;

END
