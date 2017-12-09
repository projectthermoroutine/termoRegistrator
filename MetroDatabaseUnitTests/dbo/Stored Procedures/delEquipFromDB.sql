-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================

CREATE PROCEDURE [dbo].[delEquipFromDB]( /*---EQUIPMENT-*/@EquipCode int
										--/*---PICKET----*/@Picket int,
										--				 @LastOrNot int
									   ) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	
	--DELETE FROM [Objects]	WHERE [Object] = @equipName

	/*IF @LastOrNot=1
		UPDATE Main SET Main.Code=0 WHERE Main.Npicket=@Picket
	ELSE
		DELETE FROM Main WHERE Code = @EquipCode AND Npicket = @Picket*/

	
	DELETE FROM [Objects]	WHERE [Object] = @EquipCode
END
