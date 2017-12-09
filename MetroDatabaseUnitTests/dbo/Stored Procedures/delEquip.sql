-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[delEquip](--/*---CLASS-----*/@ClassCode int,
								  /*---GROUP-----*/@GroupCode int,
								 -- /*---LINE------*/@Line	  int,
								  /*---TRACK-----*/@Track	  int,
								 -- /*---PEREGON---*/@Peregon   int,
								  /*---PICKET----*/@Npicket   int,
								  /*---EQUIPMENT-*/@EquipCode int,
								  /*-last or not-*/@LastOrNot int) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

	/*IF @LastOrNot = 1
		UPDATE Main SET Main.Code = 0 WHERE Code = @EquipCode AND ClassNum=@ClassCode AND GroupNum=@GroupCode AND LineNum=@Line AND Track=@Track AND Layout=@Peregon AND Npicket=@Npicket
	ELSE
		DELETE FROM Main WHERE Code = @EquipCode AND ClassNum=@ClassCode AND GroupNum=@GroupCode AND LineNum=@Line AND Track=@Track AND Layout=@Peregon AND Npicket=@Npicket
		*/

	--IF @LastOrNot=1
	--	UPDATE Main SET Main.Code=0 WHERE Main.Npicket=@Npicket AND Main.GroupNum = @GroupCode AND Main.Track = @Track
	--ELSE
	--BEGIN
	--	DELETE FROM Main WHERE Code = @EquipCode
	--	--DELETE FROM [Objects]	WHERE Code = @EquipCode
	--END
END
