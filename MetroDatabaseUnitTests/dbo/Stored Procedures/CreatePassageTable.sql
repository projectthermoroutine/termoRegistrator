-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[CreatePassageTable]( @TableName nvarchar(50))--, @ObjectID int, @FilePath nvarchar(MAX), @FrameID int, Fra) 
	
AS
BEGIN
	
	SET NOCOUNT ON;
	-- ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
	 DECLARE @SQLString NVARCHAR(MAX)
	 --IDENTITY(1,1)
	 SET @SQLString = 'CREATE TABLE ' + @TableName + '	([ID] [int]  NOT NULL,
														[ObjID] [int] NOT NULL,
														[FilePath] [nvarchar](max) NOT NULL,
														[FrameIndex] [int] NOT NULL,
														[FrameCoordinate] [bigint] NOT NULL,
														[Time] [datetime] NOT NULL
													) '

	 EXEC (@SQLString)
END