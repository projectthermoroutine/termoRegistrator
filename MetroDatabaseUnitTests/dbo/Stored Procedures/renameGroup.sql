-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[renameGroup](	@grNumber int,
										@grName nvarchar(20)) 
	
AS
BEGIN

	SET NOCOUNT ON;
	
	UPDATE [Group] SET [Group] = @grName WHERE Code = @grName;

END