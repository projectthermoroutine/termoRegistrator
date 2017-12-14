-- Batch submitted through debugger: SQLQuery1.sql|7|0|C:\Users\VPC\AppData\Local\Temp\~vs44D0.sql
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[insertRowInPassageTable](	@ObjID int,
													@FilePath nvarchar(MAX), 
													@FrameCoordinate bigint,
													@Time datetime ) 
AS
BEGIN
   SET NOCOUNT ON;

   /*DECLARE @SQLString NVARCHAR(MAX), @ParamDefinition NVARCHAR(MAX)*/

 /* SET @SQLString = 'INSERT INTO ObjectsFrames VALUES(@ObjID_, @FilePath_, @FrameCoordinate_, @Time_)'
   SET @ParamDefinition = N'@ObjID_ int, @FilePath_ nvarchar(MAX), @FrameCoordinate_ bigint, @Time_ datetime'
		
   EXECUTE sp_executeSQL @SQLString,
						 @ParamDefinition,
						 @ObjID_ = @ObjID,
						 @FilePath_ = @FilePath,
						 @FrameCoordinate_ = @FrameCoordinate,
						 @Time_ = @Time*/
	
	INSERT INTO ObjectsFrames VALUES(@ObjID,@FilePath,@FrameCoordinate,@Time,0,0,0,0,0)
   /*SET @SQLString = 'INSERT INTO ' + @TableName + 
					' VALUES(@ObjID_, @Time_)'
   SET @ParamDefinition = N'@ObjID_ int, @Time_ datetime'
		
   EXECUTE sp_executeSQL @SQLString,
						 @ParamDefinition,
						 @ObjID_ = @ObjID,
						 @Time_ = @Time*/

END