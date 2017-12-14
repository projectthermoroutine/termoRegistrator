-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[insertOrder1] (
										@ID int,
										@Person nvarchar(MAX),
										@Desc   nvarchar(MAX),
										@CreationDate DATE,
										@FirstDate DATE,
										@FinishDate DATE,
										@State int,
										@id_equipment int
								      ) 	
AS
BEGIN
	SET NOCOUNT ON;
	INSERT INTO [dbo].[Orders]  VALUES (@ID, @Person, @Desc, @CreationDate, @FirstDate, @FinishDate, @State, @id_equipment)
END