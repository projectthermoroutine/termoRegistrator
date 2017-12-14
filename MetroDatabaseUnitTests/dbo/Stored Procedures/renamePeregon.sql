-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[renamePeregon](  @peregonNumber int,
										@PeregonName nvarchar(100)) 
	
AS
BEGIN

	SET NOCOUNT ON;
	
	UPDATE Layout SET Layout = @PeregonName WHERE Code = @peregonNumber;

END