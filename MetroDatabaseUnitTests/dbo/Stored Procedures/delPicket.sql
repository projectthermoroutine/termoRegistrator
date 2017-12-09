-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[delPicket](/*---CLASS-----*/@ClassCode int,
								  /*---GROUP-----*/@GroupCode int,
								  /*---LINE------*/@Line	  int,
								  /*---TRACK-----*/@Track	  int,
								  /*---PEREGON---*/@Peregon   int,
								  /*---PICKET----*/@Npicket   int/*,*/
								  /*@LastOrNot int*/) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;
	----IF @LastOrNot = 1
	----BEGIN
	--	DELETE FROM Main WHERE  ClassNum=@ClassCode AND GroupNum=@GroupCode AND LineNum=@Line AND Track=@Track AND Layout=@Peregon AND Npicket=@Npicket
	--	--INSERT INTO Main VALUES(0,/*@Npicket*/0,@Track,0,0,null,0,0,0,0,@Peregon,@ClassCode,@GroupCode,@Line)
	--	--UPDATE Main SET Main.Code = 0, Main.Npicket = 0 WHERE ClassNum=@ClassCode AND GroupNum=@GroupCode AND LineNum=@Line AND Track=@Track AND Layout=@Peregon AND Npicket=@Npicket
	----END
	----ELSE
	----BEGIN
	--	--DELETE FROM Main WHERE  ClassNum=@ClassCode AND GroupNum=@GroupCode AND LineNum=@Line AND Track=@Track AND Layout=@Peregon AND Npicket=@Npicket
	--	--INSERT INTO Main VALUES(0,/*@Npicket*/0,@Track,0,0,null,0,0,0,0,@Peregon,@ClassCode,@GroupCode,@Line)
	----END
END
