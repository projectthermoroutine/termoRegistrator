USE [METROCARD_NEW]
GO
/****** Object:  Table [dbo].[AllEquipments]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[AllEquipments](
	[Code] [int] NOT NULL,
	[EquipID] [int] NOT NULL,
	[EquipTypeID] [int] NOT NULL,
	[Name] [nvarchar](50) NULL,
	[Group] [int] NOT NULL,
	[Line] [int] NOT NULL,
	[Path] [int] NOT NULL,
	[Picket] [int] NOT NULL,
	[curTemperature] [int] NOT NULL,
	[maxTemperature] [int] NOT NULL,
	[EquipWorkState] [int] NOT NULL,
	[shiftLine] [bigint] NOT NULL,
	[shiftFromPicket] [int] NOT NULL,
	[strelkaLeftOrRight] [int] NOT NULL,
	[EquipLenght] [int] NOT NULL,
	[Area_X] [int] NOT NULL,
	[Area_Y] [int] NOT NULL,
	[Area_Width] [int] NOT NULL,
	[Area_Height] [int] NOT NULL,
	[Area_Type] [int] NOT NULL,
	[Info] [nvarchar](50) NULL,
PRIMARY KEY CLUSTERED 
(
	[Code] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Class]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Class](
	[Code] [int] NOT NULL,
	[Class] [nvarchar](20) NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[Code] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[EquipmentFilter_Tbl]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[EquipmentFilter_Tbl](
	[filter_id] [int] NOT NULL,
	[class_id] [int] NOT NULL,
	[class_Name] [nvarchar](50) NOT NULL,
	[groups_Names] [nvarchar](max) NOT NULL,
	[groups_Numbers] [nvarchar](max) NOT NULL,
	[apply] [int] NOT NULL
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  Table [dbo].[EquipmentsClasses]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[EquipmentsClasses](
	[Id] [int] NOT NULL,
	[Name] [nvarchar](50) NOT NULL,
	[MinTemperature] [int] NOT NULL,
	[MaxTemperature] [int] NOT NULL,
	[Width] [int] NOT NULL,
	[Height] [int] NOT NULL,
	[X] [int] NOT NULL,
	[Y] [int] NOT NULL,
	[AreaType] [int] NOT NULL,
	[AdditionalOptions] [xml] NOT NULL,
	[strelkaLeftOrRight] [int] NOT NULL,
	[EquipType] [int] NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[Id] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Group]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Group](
	[Code] [int] NOT NULL,
	[Class] [int] NOT NULL,
	[Group] [nvarchar](20) NOT NULL,
	[ObjSrc] [nvarchar](max) NULL,
	[PtSrc] [nvarchar](max) NULL,
	[Color] [nvarchar](10) NULL,
PRIMARY KEY CLUSTERED 
(
	[Code] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Layout]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Layout](
	[Code] [int] NOT NULL,
	[Layout] [nvarchar](100) NOT NULL,
	[SName] [nvarchar](20) NULL,
	[Line] [int] NULL,
	[NperegonBefore] [int] NULL,
	[NperegonAfter] [int] NULL,
	[PeregonLength] [int] NULL
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Layout1]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Layout1](
	[Code] [int] NOT NULL,
	[Layout] [nvarchar](50) NOT NULL,
	[SName] [nvarchar](8) NULL
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Lines]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Lines](
	[LineNum] [int] NOT NULL,
	[LineName] [nvarchar](50) NOT NULL,
	[StartCoordinate] [bigint] NOT NULL,
	[LineCode] [nvarchar](100) NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[LineNum] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Objects]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Objects](
	[Code] [int] NOT NULL,
	[Group] [smallint] NULL,
	[Object] [nvarchar](50) NULL,
	[shiftLine] [bigint] NULL,
	[x] [int] NULL,
	[y] [int] NULL,
	[curTemperature] [int] NULL,
	[maxTemperature] [int] NULL,
	[regularly] [int] NULL,
	[shiftFromPicket] [int] NULL,
	[typeId] [int] NULL,
	[typeEquip] [int] NULL,
	[strelkaLeftOrRight] [int] NULL,
	[Picket] [int] NULL,
	[Path] [int] NULL,
	[Line] [int] NULL,
	[Class] [int] NULL,
	[ObjectLenght] [int] NULL,
	[Area_X] [int] NULL,
	[Area_Y] [int] NULL,
	[Area_Width] [int] NULL,
	[Area_Height] [int] NULL,
	[Area_Type] [int] NULL
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[ObjectsFrames]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[ObjectsFrames](
	[ObjID] [int] NOT NULL,
	[FilePath] [nvarchar](max) NOT NULL,
	[FrameCoordinate] [bigint] NOT NULL,
	[Time] [datetime] NOT NULL,
	[Area_X] [int] NULL,
	[Area_Y] [int] NULL,
	[Area_Height] [int] NULL,
	[Area_Width] [int] NULL,
	[Area_Type] [int] NULL
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Orders]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Orders](
	[ID] [int] NOT NULL,
	[Person] [nvarchar](max) NOT NULL,
	[Desc] [nvarchar](max) NOT NULL,
	[CreationDate] [date] NOT NULL,
	[FirstDate] [date] NOT NULL,
	[FinishDate] [date] NOT NULL,
	[State] [int] NOT NULL,
	[id_equipment] [int] NOT NULL
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Passege]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Passege](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[ObjID] [int] NOT NULL,
	[FilePath] [nvarchar](max) NOT NULL,
	[FrameIndex] [int] NOT NULL,
	[FrameCoordinate] [bigint] NOT NULL,
	[Time] [datetime] NOT NULL
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Passeges]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Passeges](
	[PassegeID] [int] NOT NULL,
	[ObjectID] [int] NOT NULL,
	[FrameID] [int] NOT NULL,
	[FrameCoordinate] [int] NOT NULL,
	[FrameTimeStamp] [datetime] NOT NULL
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Pickets]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Pickets](
	[Npiketa] [nvarchar](50) NOT NULL,
	[Dlina] [int] NOT NULL,
	[NpicketBefore] [int] NOT NULL,
	[NpicketAfter] [int] NOT NULL,
	[number] [int] NOT NULL,
	[path] [int] NOT NULL,
	[StartShiftLine] [int] NOT NULL,
	[EndShiftLine] [int] NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[number] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Table_1]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Table_1](
	[id] [int] IDENTITY(1,1) NOT NULL,
	[name] [nchar](10) NULL
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Table_3]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Table_3](
	[id] [int] NULL,
	[date] [datetime] NULL
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Tracks]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Tracks](
	[ID] [int] NOT NULL,
	[Track] [nvarchar](50) NOT NULL,
	[LineId] [int] NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
ALTER TABLE [dbo].[AllEquipments]  WITH CHECK ADD FOREIGN KEY([EquipID])
REFERENCES [dbo].[EquipmentsClasses] ([Id])
GO
ALTER TABLE [dbo].[AllEquipments]  WITH CHECK ADD FOREIGN KEY([EquipID])
REFERENCES [dbo].[EquipmentsClasses] ([Id])
GO
ALTER TABLE [dbo].[Group]  WITH CHECK ADD FOREIGN KEY([Class])
REFERENCES [dbo].[Class] ([Code])
GO
ALTER TABLE [dbo].[Group]  WITH CHECK ADD FOREIGN KEY([Class])
REFERENCES [dbo].[Class] ([Code])
GO
ALTER TABLE [dbo].[Pickets]  WITH NOCHECK ADD FOREIGN KEY([path])
REFERENCES [dbo].[Tracks] ([ID])
GO
ALTER TABLE [dbo].[Pickets]  WITH NOCHECK ADD FOREIGN KEY([path])
REFERENCES [dbo].[Tracks] ([ID])
GO
ALTER TABLE [dbo].[Tracks]  WITH CHECK ADD FOREIGN KEY([LineId])
REFERENCES [dbo].[Lines] ([LineNum])
GO
ALTER TABLE [dbo].[Tracks]  WITH CHECK ADD FOREIGN KEY([LineId])
REFERENCES [dbo].[Lines] ([LineNum])
GO
/****** Object:  StoredProcedure [dbo].[ChangeEquipmentsOffsets]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[ChangeEquipmentsOffsets](@ClassCode int)
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    -- Insert statements for procedure here
	
END
GO
/****** Object:  StoredProcedure [dbo].[CreatePassageTable]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
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

GO
/****** Object:  StoredProcedure [dbo].[delClass]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[delClass](  @ClassCode int, @error_msg nvarchar(1000) OUT ) 
	
AS
BEGIN
	declare @aaa int
	--BEGIN TRY
	--	BEGIN TRAN;
	--		DECLARE @group_id int
	--		DECLARE cursor_group_id CURSOR READ_ONLY FOR 
	--		SELECT LineNum FROM Main WHERE ClassNum = @ClassCode 
	
	--		OPEN cursor_group_id
	--		FETCH NEXT FROM cursor_group_id INTO @group_id
	
	--		WHILE @@FETCH_STATUS = 0
	--		BEGIN 
	--			exec [dbo].[delGroup] @ClassCode,@group_id,@error_msg
	--			FETCH NEXT FROM cursor_group_id INTO @group_id
	--		END

	--		CLOSE cursor_group_id
	--		DEALLOCATE cursor_group_id

	--		DELETE FROM Main WHERE ClassNum = @ClassCode  
	--		DELETE FROM [Class] WHERE Code = @ClassCode
	--		COMMIT TRAN;
	--	RETURN (0);
	--END TRY
	--BEGIN CATCH
	--	SELECT XACT_STATE() as 'XACT_STATE', @@TRANCOUNT as '@@TRANCOUNT';
	--	IF @@TRANCOUNT > 1
	--	BEGIN
	--		ROLLBACK TRANSACTION;
	--		THROW;
	--	END

	--	SET @error_msg = ERROR_MESSAGE();

	--END CATCH;
END
GO
/****** Object:  StoredProcedure [dbo].[delEquip]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
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
GO
/****** Object:  StoredProcedure [dbo].[delEquipAsType]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================

CREATE PROCEDURE [dbo].[delEquipAsType]( @EquipCode int   ) 
	
AS
BEGIN
BEGIN TRANSACTION
declare @error as int;
SET @error =0;
BEGIN TRY
	DELETE FROM AllEquipments WHERE EquipID = @EquipCode;
	DELETE FROM EquipmentsClasses WHERE Id = @EquipCode;

COMMIT TRANSACTION
END TRY
BEGIN CATCH
ROLLBACK TRANSACTION
	
	SET @error = ERROR_NUMBER();
	
END CATCH

RETURN @error;
END
GO
/****** Object:  StoredProcedure [dbo].[DeleteClass]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- ================================================
--
-- Deletes all equipments that contains target class
--
-- ================================================
CREATE PROCEDURE [dbo].[DeleteClass](  @ClassCode int, @error_msg nvarchar(1000) OUT ) 
	
AS
BEGIN
	
	--
	--select all groups of target class and then select all equipment by groups
	--
	DECLARE @groupsID table(ID int NOT NULL);
	INSERT INTO @groupsID SELECT Code FROM [Group] WHERE Class = @ClassCode;
	
	DECLARE @equipsByGroups table(ID int NOT NULL);
	INSERT INTO @equipsByGroups SELECT EquipmentId FROM Main WHERE GroupId IN (SELECT ID FROM @groupsID );
	
	-- delete from MAIN
	DELETE FROM Main WHERE GroupId IN (SELECT ID FROM @groupsID);
	
	-- delete form AllEquipments
	DELETE FROM AllEquipments WHERE Code IN ( SELECT ID FROM @equipsByGroups );

	-- delete groups
	DELETE FROM [Group] WHERE Code IN (SELECT ID FROM @groupsID);

	--delete class
	DELETE FROM [Class] WHERE Code = @ClassCode;
END
GO
/****** Object:  StoredProcedure [dbo].[DeleteGroup]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[DeleteGroup](  @GroupID int, @error_msg nvarchar(1000) OUT ) 
	
AS
BEGIN
	
	DECLARE @equipsByGroups table(ID int NOT NULL);
	INSERT INTO @equipsByGroups SELECT EquipmentId FROM Main WHERE GroupId = @GroupID;
	
	-- delete from MAIN
	DELETE FROM Main WHERE GroupId = @GroupID;
	
	-- delete form AllEquipments
	DELETE FROM AllEquipments WHERE Code IN ( SELECT ID FROM @equipsByGroups );

	-- delete groups
	DELETE FROM [Group] WHERE Code = @GroupID;

END
GO
/****** Object:  StoredProcedure [dbo].[deleteLine]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[deleteLine](/*---CLASS-----*/	@ClassCode int,
									/*---GROUP-----*/	@GroupCode int,
									/*---LINE------*/	@Line	  int									
							   ) 
AS
BEGIN
	SET NOCOUNT ON;

	--
	-- delete from lines table
	--

	DELETE FROM Lines WHERE LineNum = @Line
 
	
	DECLARE @equip_id int
	DECLARE @track_id int
	DECLARE cursor_track_id CURSOR READ_ONLY FOR SELECT ID FROM [dbo].[Track] WHERE LineId = @Line 
	
	OPEN cursor_track_id
	FETCH NEXT FROM cursor_track_id INTO @track_id
	
	WHILE @@FETCH_STATUS = 0
	BEGIN
		
		--
		-- delete pickets 
		--
		DELETE FROM Pickets WHERE [path] = @track_id
		
		--
		-- delete equipments from Main table
		--
		DECLARE cursor_equip_id CURSOR READ_ONLY FOR 
			SELECT code FROM [dbo].[AllEquipments] WHERE [Path] = @track_id 

		OPEN cursor_equip_id 
			FETCH NEXT FROM cursor_equip_id INTO @equip_id

			WHILE @@FETCH_STATUS = 0
			BEGIN
				DELETE FROM Main WHERE EquipmentId = @equip_id
				FETCH NEXT FROM cursor_equip_id INTO @equip_id
			END

		--
		-- Delete equipment
		--
		DELETE FROM AllEquipments WHERE [Path] = @track_id

		FETCH NEXT FROM cursor_track_id INTO @track_id
	END

	CLOSE cursor_track_id
	DEALLOCATE cursor_track_id

	--
	-- delete tracks
	--

	DELETE FROM Track WHERE LineId = @Line
	
END
GO
/****** Object:  StoredProcedure [dbo].[DeleteLineSQL]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[DeleteLineSQL](	@Line	int	) 
AS

	-- 1. Pickets 
	-- 2. Tracks 
	-- 3. Lines
	-- 4. Main 
	-- 5. AllEquipments
	BEGIN TRY
		BEGIN TRAN;

			DECLARE @TracksId table(ID int NOT NULL);
			INSERT INTO @TracksId SELECT ID FROM Tracks WHERE LineId = @Line
			
			

			DELETE FROM AllEquipments WHERE [Path] IN (SELECT ID FROM @TracksId);
			DELETE FROM Pickets WHERE [path] IN (SELECT ID FROM @TracksId);
			DELETE FROM Tracks WHERE ID IN (SELECT ID FROM @TracksId); 

			DELETE FROM Lines WHERE LineNum = @Line;
		COMMIT TRAN;
		RETURN (1);
	END TRY
	BEGIN CATCH
		--SELECT XACT_STATE() as 'XACT_STATE', @@TRANCOUNT as '@@TRANCOUNT';
			--IF @@TRANCOUNT > 1
			--BEGIN
				--ROLLBACK TRANSACTION;
				--THROW;
			--END

			--SET @error_msg = ERROR_MESSAGE();
			ROLLBACK TRANSACTION;
	END CATCH;

RETURN 0
GO
/****** Object:  StoredProcedure [dbo].[DeleteTrackTransaction]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- ================================================
-- Template generated from Template Explorer using:
-- Create Procedure (New Menu).SQL
--
-- Use the Specify Values for Template Parameters 
-- command (Ctrl-Shift-M) to fill in the parameter 
-- values below.
--
-- This block of comments will not be included in
-- the definition of the procedure.
-- ================================================

CREATE PROCEDURE [dbo].[DeleteTrackTransaction](@num int)
AS
BEGIN

BEGIN TRANSACTION
declare @error as int;
SET @error =0;
BEGIN TRY
	--DECLARE @EquipByTrack table(ID int NOT NULL);
	--INSERT INTO @EquipByTrack SELECT Code FROM AllEquipments WHERE [Path] = @num;
	
	--DELETE FROM Main WHERE EquipmentId IN ( SELECT ID FROM @EquipByTrack );
	--DELETE FROM AllEquipments WHERE Code IN (SELECT ID FROM @EquipByTrack);
	DELETE FROM AllEquipments WHERE [Path] = @num;
	DELETE FROM Pickets WHERE [path] = @num;
	DELETE FROM Tracks WHERE ID = @num; 
COMMIT TRANSACTION
END TRY
BEGIN CATCH
ROLLBACK TRANSACTION
	
	SET @error = ERROR_NUMBER();
	
END CATCH

RETURN @error;
END
GO
/****** Object:  StoredProcedure [dbo].[delGroup]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[delGroup]( @ClassCode int,
								  @GroupNum	  int,
								  @error_msg nvarchar(1000) OUT	  
								) 
	
AS
BEGIN
	declare @aaa int
	--BEGIN TRY
	--	BEGIN TRAN;

	--		DECLARE @line_id int
	--		DECLARE cursor_line_id CURSOR READ_ONLY FOR 
	--		SELECT LineNum FROM Main WHERE GroupNum=@GroupNum 
	
	--		OPEN cursor_line_id
	--		FETCH NEXT FROM cursor_line_id INTO @line_id
	
	--		WHILE @@FETCH_STATUS = 0
	--		BEGIN 
	--			exec [dbo].[delLine]  @ClassCode,@GroupNum,@line_id, @error_msg
	--			FETCH NEXT FROM cursor_line_id INTO @line_id
	--		END

	--		CLOSE cursor_line_id
	--		DEALLOCATE cursor_line_id

	--		DECLARE @group_count int
	--		SELECT @group_count = count(Main.LineNum) FROM Main WHERE GroupNum = @GroupNum AND ClassNum = @ClassCode

	--		IF(@group_count=1)
	--			UPDATE Main SET [Main].[GroupNum] = 0 WHERE  ClassNum=@ClassCode AND [GroupNum]=@GroupNum

	--		SELECT  @group_count = count(GroupNum) From Main where [GroupNum]=@GroupNum AND ClassNum!=@ClassCode
	
	--		IF (@group_count = 0)
	--		BEGIN
	--			DELETE FROM [Group] WHERE Code = @GroupNum 
	--		END

	--	COMMIT TRAN;
	--	RETURN (0);
	--END TRY
	--BEGIN CATCH
	--	SELECT XACT_STATE() as 'XACT_STATE', @@TRANCOUNT as '@@TRANCOUNT';
	--	IF @@TRANCOUNT > 1
	--	BEGIN
	--		ROLLBACK TRANSACTION;
	--		THROW;
	--	END

	--	SET @error_msg = ERROR_MESSAGE();
	--END CATCH;
END
GO
/****** Object:  StoredProcedure [dbo].[delLine]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[delLine](/*---CLASS-----*/@ClassCode int,
								/*---GROUP-----*/@GroupCode int,
								/*---LINE------*/@Line	  int,
								 @error_msg nvarchar(1000) OUT	
							   ) 
AS
BEGIN
	declare @aaa int
	--BEGIN TRY
	--	BEGIN TRAN;

	--	DECLARE @track_id int
	--	DECLARE cursor_track_id CURSOR READ_ONLY FOR 
	--	SELECT Track FROM Main WHERE LineNum=@Line 
	
	--	OPEN cursor_track_id
	--	FETCH NEXT FROM cursor_track_id INTO @track_id
	
	--	WHILE @@FETCH_STATUS = 0
	--	BEGIN 
	--		exec [dbo].[delPath] @track_id, @ClassCode, @GroupCode, @Line;
	--		FETCH NEXT FROM cursor_track_id INTO @track_id
	--	END

	--	CLOSE cursor_track_id
	--	DEALLOCATE cursor_track_id

	--	DECLARE @lines_count int
	--	SELECT @lines_count = count(Main.LineNum) FROM Main WHERE LineNum = @Line AND GroupNum = @GroupCode AND ClassNum = @ClassCode

	--	IF(@lines_count=1)
	--		UPDATE Main SET [Main].[LineNum] = 0 WHERE  ClassNum=@ClassCode AND [GroupNum]=@GroupCode AND LineNum=@Line
	--	ELSE
	--		DELETE FROM Main WHERE ClassNum=@ClassCode AND [GroupNum]=@GroupCode AND LineNum=@Line
	
	--	SELECT  @lines_count = count(LineNum) From Main where LineNum=@Line AND [GroupNum]!=@GroupCode AND ClassNum=@ClassCode
	
	--	IF (@lines_count = 0)
	--	BEGIN
	--		DELETE FROM Lines WHERE LineNum = @Line 
	--	END

	--	COMMIT TRAN;
	--	RETURN 0;
	--END TRY
	--BEGIN CATCH
	--	SELECT XACT_STATE() as 'XACT_STATE', @@TRANCOUNT as '@@TRANCOUNT';
	--	IF @@TRANCOUNT > 1
	--	BEGIN
	--		ROLLBACK TRANSACTION;
	--		THROW;
	--	END

	--	SET @error_msg = ERROR_MESSAGE();
		
		
	--	THROW;
	--END CATCH;
END
GO
/****** Object:  StoredProcedure [dbo].[delPath]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- =============================================
CREATE PROCEDURE [dbo].[delPath]( @num	    int,								  
								 @ClassID   int,
								 @GroupID   int,
								 @LineID    int)
	
AS
BEGIN
	SET NOCOUNT ON;
		
	--BEGIN TRY
	--	BEGIN TRAN;

	--	DECLARE @track_count int
	--	SELECT @track_count = count(Track) FROM Main WHERE Track=@num AND [GroupNum]=@GroupID AND LineNum=@LineID


	--	IF @track_count = 1
	--		UPDATE Main SET Main.Track = 0 WHERE  Track=@num AND [GroupNum]=@GroupID AND LineNum=@LineID
	--	ELSE
	--	BEGIN
	--		DELETE FROM Main WHERE  Track = @num AND [GroupNum]=@GroupID AND LineNum=@LineID
	--		INSERT INTO Main VALUES(0,0,0,0,0,NULL,0,0,0,0,0,@ClassID,@GroupID,@LineID)
	--	END

	--	DELETE FROM [Objects] WHERE [Path] = @num AND [Group]=@GroupID AND Line=@LineID
	--	DELETE FROM Pickets	  WHERE [path] = @num AND [Group]=@GroupID AND Line=@LineID

	--	DECLARE @path int
	--	SELECT  @path = count(Track) From Main where Track=@num AND [GroupNum]!=@GroupID AND LineNum!=@LineID
	
	--	IF (@path=0)
	--	BEGIN
	--		DELETE FROM Track WHERE ID = @num 
	--	END
	--	COMMIT TRAN;
	--	RETURN 0;
	--END  TRY
	--BEGIN CATCH
	--	SELECT XACT_STATE() as 'XACT_STATE', @@TRANCOUNT as '@@TRANCOUNT';
	--	IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION;
	--	THROW;
	--END CATCH;
	
END
GO
/****** Object:  StoredProcedure [dbo].[delPathFromMainTable]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[delPathFromMainTable](	@Track	  int,								  
												@lastOrNot int
											) 
	
AS
BEGIN

	SET NOCOUNT ON;
	--IF @LastOrNot = 1
	--	UPDATE Main SET Main.Track = 0 WHERE  Track=@Track
	--ELSE
	--	DELETE FROM Main WHERE  Track=@Track 

	--DELETE FROM Track WHERE ID=@Track
	
END
GO
/****** Object:  StoredProcedure [dbo].[delPeregon]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[delPeregon](/*---CLASS-----*/@ClassCode int,
								  /*---GROUP-----*/@GroupCode int,
								  /*---LINE------*/@Line	  int,
								  /*---TRACK-----*/@Track	  int,
								  /*---PEREGON---*/@Peregon   int,
								  --/*---PICKET----*/@Npicket   int,
								  @LastOrNot int) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;
	--IF @LastOrNot = 1
	--BEGIN
	--	DELETE FROM Main WHERE  ClassNum=@ClassCode AND GroupNum=@GroupCode AND LineNum=@Line AND Track=@Track AND Layout=@Peregon --AND Npicket=@Npicket
	--	INSERT INTO Main VALUES(0,/*@Npicket*/0,@Track,0,0,null,0,0,0,0,/*@Peregon*/0,@ClassCode,@GroupCode,@Line)
	--	--UPDATE Main SET Main.Code = 0, Main.Npicket = 0 WHERE ClassNum=@ClassCode AND GroupNum=@GroupCode AND LineNum=@Line AND Track=@Track AND Layout=@Peregon AND Npicket=@Npicket
	--END
	--ELSE
	--BEGIN
	--	DELETE FROM Main WHERE  ClassNum=@ClassCode AND GroupNum=@GroupCode AND LineNum=@Line AND Track=@Track AND Layout=@Peregon --AND Npicket=@Npicket
	--	--INSERT INTO Main VALUES(0,/*@Npicket*/0,@Track,0,0,null,0,0,0,0,@Peregon,@ClassCode,@GroupCode,@Line)
	--END
END
GO
/****** Object:  StoredProcedure [dbo].[delPeregonFromDB]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[delPeregonFromDB](--/*---CLASS-----*/@ClassCode int,
								  --/*---GROUP-----*/@GroupCode int,
								  --/*---LINE------*/@Line	  int,
								  --/*---TRACK-----*/@Track	  int,
								  --/*---PEREGON---*/@Peregon   int,
								  /*---PICKET----*/@Nperegon   int
								  --@LastOrNot int
								  ) 
	
AS
BEGIN

	declare @before int
	declare @after int

	SELECT @before = NperegonBefore, @after= NperegonAfter FROM Layout WHERE Code = @Nperegon
	DELETE FROM Layout WHERE Code = @Nperegon
	IF @before!=0
	BEGIN
			UPDATE Layout SET NperegonAfter = @after WHERE Code = @before
		IF @after!=0
			UPDATE Layout  SET NperegonBefore = @before WHERE Code = @after
		ELSE
			UPDATE Layout SET NperegonAfter = 0 WHERE Code = @before
	END
	ELSE
		UPDATE Layout SET NperegonBefore = 0 WHERE Code = @after

END
GO
/****** Object:  StoredProcedure [dbo].[delPicketFromDB]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[delPicketFromDB](@Npicket   int  ) 
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	
	DECLARE @equipsByPicket table(ID int NOT NULL);
	INSERT INTO @equipsByPicket SELECT Code FROM AllEquipments WHERE Picket = @Npicket;

	--DELETE FROM Main WHERE EquipmentId IN (SELECT ID FROM @equipsByPicket)
	DELETE FROM AllEquipments WHERE Picket = @Npicket;

	declare @deletingBefore int;
	declare @deletingAfter int;

	SELECT @deletingBefore = NpicketBefore, @deletingAfter= NpicketAfter FROM Pickets WHERE number = @Npicket;
	DELETE FROM Pickets WHERE number = @Npicket;
	
	IF @deletingBefore!=0
	BEGIN
			UPDATE Pickets SET NpicketAfter = @deletingAfter WHERE number = @deletingBefore;
		IF @deletingAfter!=0
			UPDATE Pickets  SET NpicketBefore = @deletingBefore WHERE number = @deletingAfter;	
	END
	ELSE
	BEGIN
		IF @deletingAfter!=0
			UPDATE Pickets SET NpicketBefore = 0 WHERE number = @deletingAfter;
	END
END
GO
/****** Object:  StoredProcedure [dbo].[editFilters]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[editFilters]( /*---CLASS-----*/@ClassCode  int,
								/*---GROUP-----*/@GroupCode  int,
								/*---LINE------*/@Line		 int,
												 @newLineOrNot int) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

	--declare @firstOrNot int
	--SET @firstOrNot = -1
	--SELECT @firstOrNot = LineNum FROM Main WHERE ClassNum = @ClassCode AND GroupNum = @GroupCode

	--declare @maxLayout int

	----IF @newLineOrNot=0
	--	--INSERT INTO Layout VALUES(0,'notExist','NE',@Line,0,0)
	--IF @firstOrNot = 0
	--	UPDATE Main SET Main.LineNum = @Line WHERE ClassNum = @ClassCode AND GroupNum = @GroupCode
	--ELSE
	--	INSERT INTO Main      VALUES(/*equip*/0,/*Npicket*/0,/*Track*/0,0,0,null,0,0,0,0,/*Peregon*/0,@ClassCode,@GroupCode,@Line)
END
GO
/****** Object:  StoredProcedure [dbo].[insertEquipTemperature]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================

CREATE PROCEDURE [dbo].[insertEquipTemperature] (/*-----EQUIPMENT------*/ @EquipCode int,
												 /*---CurTemperature---*/@curTemperature int
												) 
	
AS
BEGIN
	SET NOCOUNT ON;
	--UPDATE [Objects] SET [Objects].curTemperature=@curTemperature WHERE [Objects].Code = @EquipCode
	UPDATE AllEquipments SET curTemperature = @curTemperature WHERE Code = @EquipCode
END
GO
/****** Object:  StoredProcedure [dbo].[insertFilter]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[insertFilter](	/*---FILTER_ID--*/@Filter_ID int,
										/*---CLASS------*/@ClassCode  int, @ClassName nvarchar(50),
										/*---GROUPS-----*/@GroupsCodes  nvarchar(MAX), @GroupsNames nvarchar(MAX),
										/*---STATUS-----*/@Status int
									) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

	/*declare @firstOrNot int
	SET @firstOrNot = -1
	SELECT @firstOrNot = LineNum FROM Main WHERE ClassNum = @ClassCode AND GroupNum = @GroupCode

	declare @maxLayout int

	--IF @newLineOrNot=0
		--INSERT INTO Layout VALUES(0,'notExist','NE',@Line,0,0)
	IF @firstOrNot = 0
		UPDATE Main SET Main.LineNum = @Line WHERE ClassNum = @ClassCode AND GroupNum = @GroupCode
	ELSE
		INSERT INTO Main      VALUES(/*equip*/0,/*Npicket*/0,/*Track*/0,0,0,null,0,0,0,0,/*Peregon*/0,@ClassCode,@GroupCode,@Line)*/

	--DELETE  FROM EquipmentFilter_Tbl
	INSERT INTO EquipmentFilter_Tbl VALUES(@Filter_ID, @ClassCode,@ClassName,@GroupsNames,@GroupsCodes,@Status)
	

END
GO
/****** Object:  StoredProcedure [dbo].[insertOrder1]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
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
GO
/****** Object:  StoredProcedure [dbo].[insertRowInPassageTable]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
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
GO
/****** Object:  StoredProcedure [dbo].[insertStoredProcedure1]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[insertStoredProcedure1] 
	-- Add the parameters for the stored procedure here
	(@ClassCode int ,
	@ClassName char(20))
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    -- Insert statements for procedure here
	INSERT INTO   [Class]  VALUES(@ClassCode,@ClassName); 
END
GO
/****** Object:  StoredProcedure [dbo].[insertStoredProcedure2]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[insertStoredProcedure2] (/*---CLASS-----*//*@ClassCode int, @ClassName char(50),*/
												/*---GROUP-----*/@GroupCode int, @GroupName char(50),
												/*---LINE------*/@Line int,
												/*---TRACK-----*/@Track int,
												/*---PEREGON---*/@newPeregonRow BIT,@typeOfPeregonCreation int,@peregonNumber int, @beforePeregon1 int, @afterPeregon1 int, @beforePeregon2 int,@afterPeregon2 int, @beforePeregon3 int,@afterPeregon3 int, @peregonName char(50), @ShortPeregonName char(30),
												/*---PICKET----*/@newPicketRow int,@typeOfPicketCreation int, @Npicket int,@DlinaPicket int,@beforePicket1 int,@afterPicket1 int,@beforePicket2 int,@afterPicket2 int,
												/*--EQUIPMENT--*/@newEquipmentRow int, @EquipmentNumber int,/*@GroupNumber = @GroupCode*/ @EquipmentName char(50)) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

	--INSERT INTO  [Class]						VALUES(@ClassCode,@ClassName); 
	--INSERT INTO [Group]([Code],[Class],[Group]) VALUES(@GroupCode,@ClassCode,@GroupName); 
	--declare @eqipmentNumberTMP int
	--declare @eqipmentNumber1 int
	--SET @eqipmentNumber1=0
	--SELECT @eqipmentNumberTMP = Code FROM [Objects] WHERE [Group] = @GroupCode
	--SELECT @eqipmentNumber1 = Code FROM Main WHERE Code = @eqipmentNumberTMP AND Track=0 AND Layout=0

	--declare @newPeregonNumber int

	--IF (@newPeregonRow = 1)	
	--BEGIN
	--   IF @typeOfPeregonCreation = 7 /*---- выбрано создание перегона между крайними перегодами на пути -----*/
	--   BEGIN
	--	UPDATE Layout SET NperegonAfter = @beforePeregon3 WHERE Code = @beforePeregon2
	--	INSERT INTO Layout VALUES(@beforePeregon3,@peregonName,@ShortPeregonName,@Line,@beforePeregon2,@afterPeregon2)
	--	UPDATE Layout SET NperegonBefore = @afterPeregon1 WHERE Code = @afterPeregon2;
	--   END

	--   IF @typeOfPeregonCreation = 6 /*---- выбрано создание перегона между первыми перегонами на пути -----*/
	--   BEGIN
	--    UPDATE Layout SET NperegonAfter = @beforePeregon3 WHERE Code = @beforePeregon2
	--	INSERT INTO Layout VALUES(@beforePeregon3,@peregonName,@ShortPeregonName,@Line,@beforePeregon2,@afterPeregon2)
	--	UPDATE Layout SET NperegonBefore = @afterPeregon1 WHERE Code = @afterPeregon2
	--   END

	--   IF @typeOfPeregonCreation = 2 /*---- выбрано создание нового последнего перегона в линии ----*/
	--   BEGIN
	--    UPDATE Layout SET NperegonAfter = @afterPeregon1 WHERE Code = @beforePeregon2
	--	INSERT INTO Layout VALUES(@afterPeregon1,@peregonName,@ShortPeregonName,@Line,@beforePeregon2,0)
	--   END

	--   IF @typeOfPeregonCreation = 1 /*---- выбрано создание нового первого перегона в линии ----- */
	--   BEGIN
	--    INSERT INTO Layout VALUES(@beforePeregon2,@peregonName,@ShortPeregonName,@Line,0,@afterPeregon1)
	--    UPDATE Layout SET NperegonBefore = @beforePeregon2 WHERE Code = @afterPeregon1
	--   END
	--END
	
	--IF @newPicketRow = 1
	--BEGIN
	--	IF @typeOfPicketCreation=0 
	--	BEGIN
	--		INSERT INTO Pickets VALUES(@beforePicket2,@DlinaPicket,@peregonNumber,0,@afterPicket1)
	--		UPDATE Pickets SET NpicketBefore = @beforePicket2 WHERE Npiketa = @afterPicket1 
	--	END
	--	IF @typeOfPicketCreation=1
	--	BEGIN
	--		INSERT INTO Pickets VALUES(@afterPicket1,@DlinaPicket,@peregonNumber,@beforePicket2,0)
	--	END
	--END
	--IF @newEquipmentRow != 1 -- Equipment
	--BEGIN
	--	IF @eqipmentNumber1>0
	--		UPDATE [Objects] SET Code = @eqipmentNumber1, [Group] = @GroupCode, Object = @EquipmentName
	--	ELSE
	--		INSERT INTO [Objects] VALUES (@EquipmentNumber,@GroupCode,@EquipmentName) 
	--END
	--/*   MAIN   */
	--IF @eqipmentNumber1 = 0 -- 
	--	INSERT INTO Main VALUES(@EquipmentNumber,@Npicket,@Track,0,0,null,0,0,0,0,@peregonNumber) -- IF equipment not created it value 0, also with picket
	--ELSE
	--	BEGIN
	--	IF @typeOfPicketCreation > 0
	--		UPDATE Main SET Layout = @peregonNumber, Track = @Track, Npicket = @Npicket WHERE Code = @EquipmentNumber AND Track = 0 AND Npicket = 0 AND Layout = 0  -- Picket Update
	--	ELSE
	--		UPDATE Main SET Layout = @peregonNumber, Track = @Track WHERE Code = @EquipmentNumber AND Track = 0 AND Npicket = 0 AND Layout = 0						-- Picket not Update
	--	END
END
GO
/****** Object:  StoredProcedure [dbo].[insertStoredProcedure3]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[insertStoredProcedure3](/*---CLASS-----*/@ClassCode int,
												/*---GROUP-----*/@GroupCode int,
												/*---LINE------*/@Line int,
												/*---TRACK-----*/@Track int,
												/*---PEREGON---*/@peregonNumber int) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

	declare @firstOrNorPeregon int
	SET @firstOrNorPeregon=0
	--SELECT @firstOrNorPeregon=Layout FROM Main  WHERE Track=@Track AND ClassNum = @ClassCode AND GroupNum=@GroupCode AND LineNum=@Line

	/*IF (@newPeregonRow = 1)	
	BEGIN
	   IF @typeOfPeregonCreation = 8/*---- выбрано создание нового перегона линии -----*/
	   BEGIN
		IF @firstOrNorPeregon=0
			INSERT INTO Layout VALUES(@peregonNumber,@peregonName,@ShortPeregonName,@Line,0,0)
		ELSE
			UPDATE Layout SET Code = @peregonNumber, Layout=@peregonName,SName = @ShortPeregonName,NperegonBefore= 0,NperegonAfter=0 WHERE Line=@Line
	   END
	   IF @typeOfPeregonCreation = 7 /*---- выбрано создание перегона между крайними перегодами на пути -----*/
	   BEGIN
		UPDATE Layout SET NperegonAfter = @beforePeregon3 WHERE Code = @beforePeregon2
		INSERT INTO Layout VALUES(@beforePeregon3,@peregonName,@ShortPeregonName,@Line,@beforePeregon2,@afterPeregon2)
		UPDATE Layout SET NperegonBefore = @afterPeregon1 WHERE Code = @afterPeregon2;
	   END

	   IF @typeOfPeregonCreation = 6 /*---- выбрано создание перегона между первыми перегонами на пути -----*/
	   BEGIN
	    UPDATE Layout SET NperegonAfter = @beforePeregon3 WHERE Code = @beforePeregon2
		INSERT INTO Layout VALUES(@beforePeregon3,@peregonName,@ShortPeregonName,@Line,@beforePeregon2,@afterPeregon2)
		UPDATE Layout SET NperegonBefore = @afterPeregon1 WHERE Code = @afterPeregon2
	   END

	   IF @typeOfPeregonCreation = 2 /*---- выбрано создание нового последнего перегона в линии ----*/
	   BEGIN
	    UPDATE Layout SET NperegonAfter = @afterPeregon1 WHERE Code = @beforePeregon2
		INSERT INTO Layout VALUES(@afterPeregon1,@peregonName,@ShortPeregonName,@Line,@beforePeregon2,0)
	   END

	   IF @typeOfPeregonCreation = 1 /*---- выбрано создание нового первого перегона в линии ----- */
	   BEGIN
	    INSERT INTO Layout VALUES(@beforePeregon2,@peregonName,@ShortPeregonName,@Line,0,@afterPeregon1)
	    UPDATE Layout SET NperegonBefore = @beforePeregon2 WHERE Code = @afterPeregon1
	   END
	END*/

	--IF @firstOrNorPeregon!=0
	--	INSERT INTO Main VALUES(0,0,@Track,0,0,null,0,0,0,0,@peregonNumber,@ClassCode,@GroupCode,@Line)
	--ELSE
	--	UPDATE Main SET Layout=@peregonNumber WHERE Track=@Track AND ClassNum = @ClassCode AND GroupNum=@GroupCode AND LineNum=@Line
END
GO
/****** Object:  StoredProcedure [dbo].[line]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[line] (/*---CLASS-----*/@ClassCode int, @ClassName char(50),
												/*---GROUP-----*/@GroupCode int, @GroupName char(50),
												/*---LINE------*/@Line int,
												/*---TRACK-----*/@Track int,
												/*---PEREGON---*/@newPeregonRow BIT,@typeOfPeregonCreation int,@peregonNumber int, @beforePeregon1 int, @afterPeregon1 int, @beforePeregon2 int,@afterPeregon2 int, @beforePeregon3 int,@afterPeregon3 int, @peregonName char(50), @ShortPeregonName char(30),
												/*---PICKET----*/@newPicketRow int,@typeOfPicketCreation int, @Npicket int,@DlinaPicket int,@beforePicket1 int,@afterPicket1 int,@beforePicket2 int,@afterPicket2 int,
												/*--EQUIPMENT--*/@newEquipmentRow int, @EquipmentNumber int,/*@GroupNumber = @GroupCode*/ @EquipmentName char(50)) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

	--INSERT INTO  [Class]						VALUES(@ClassCode,@ClassName); 
	--INSERT INTO [Group]([Code],[Class],[Group]) VALUES(@GroupCode,@ClassCode,@GroupName);
	--UPDATE Layout SE 
	--

	--declare @newPeregonNumber int

	--IF (@newPeregonRow = 1)	
	--BEGIN
	--   IF @typeOfPeregonCreation = 7 /*---- выбрано создание перегона между крайними перегодами на пути -----*/
	--   BEGIN
	--	UPDATE Layout SET NperegonAfter = @beforePeregon3 WHERE Code = @beforePeregon2
	--	INSERT INTO Layout VALUES(@beforePeregon3,@peregonName,@ShortPeregonName,@Line,@beforePeregon2,@afterPeregon2)
	--	UPDATE Layout SET NperegonBefore = @afterPeregon1 WHERE Code = @afterPeregon2;
	--   END

	--   IF @typeOfPeregonCreation = 6 /*---- выбрано создание перегона между первыми перегонами на пути -----*/
	--   BEGIN
	--    UPDATE Layout SET NperegonAfter = @beforePeregon3 WHERE Code = @beforePeregon2
	--	INSERT INTO Layout VALUES(@beforePeregon3,@peregonName,@ShortPeregonName,@Line,@beforePeregon2,@afterPeregon2)
	--	UPDATE Layout SET NperegonBefore = @afterPeregon1 WHERE Code = @afterPeregon2
	--   END

	--   IF @typeOfPeregonCreation = 2 /*---- выбрано создание нового последнего перегона в линии ----*/
	--   BEGIN
	--    UPDATE Layout SET NperegonAfter = @afterPeregon1 WHERE Code = @beforePeregon2
	--	INSERT INTO Layout VALUES(@afterPeregon1,@peregonName,@ShortPeregonName,@Line,@beforePeregon2,0)
	--   END

	--   IF @typeOfPeregonCreation = 1 /*---- выбрано создание нового первого перегона в линии ----- */
	--   BEGIN
	--    INSERT INTO Layout VALUES(@beforePeregon2,@peregonName,@ShortPeregonName,@Line,0,@afterPeregon1)
	--    UPDATE Layout SET NperegonBefore = @beforePeregon2 WHERE Code = @afterPeregon1
	--   END
	--END
	
	--IF @newPicketRow = 1
	--BEGIN
	--	IF @typeOfPicketCreation=0 
	--	BEGIN
	--		INSERT INTO Pickets VALUES(@beforePicket2,@DlinaPicket,@peregonNumber,0,@afterPicket1)
	--		UPDATE Pickets SET NpicketBefore = @beforePicket2 WHERE Npiketa = @afterPicket1 
	--	END
	--	IF @typeOfPicketCreation=1
	--	BEGIN
	--		INSERT INTO Pickets VALUES(@afterPicket1,@DlinaPicket,@peregonNumber,@beforePicket2,0)
	--	END
	--END
	--IF @newEquipmentRow = 1
	--	INSERT INTO [Objects] VALUES (@EquipmentNumber,@GroupCode,@EquipmentName) 
  
	----INSERT INTO Main VALUES(@EquipmentNumber,@Npicket,@Track,0,0,null,0,0,0,0,@peregonNumber)
	--UPDATE Main SET Layout = @peregonNumber WHERE Code = @beforePeregon2
END
GO
/****** Object:  StoredProcedure [dbo].[peregonCreate]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[peregonCreate](/*---LINE------*/@Line int,
									  /*---PEREGON---*/@typeOfPeregonCreation int,@peregonNumber int, @afterPeregon1 int, @beforePeregon2 int,@afterPeregon2 int, @beforePeregon3 int, @peregonName nvarchar(100), @ShortPeregonName nvarchar(20), @PeregonLength int) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;
	
	   IF @typeOfPeregonCreation = 9 /*----- создание первого перегона линии ------*/
		INSERT INTO Layout VALUES(@peregonNumber,@peregonName,@ShortPeregonName,@Line,0,0,@PeregonLength)
	   
	   IF @typeOfPeregonCreation = 7 OR @typeOfPeregonCreation = 6 /*---- выбрано создание перегона между перегонами на пути -----*/
	   BEGIN
		UPDATE Layout SET NperegonAfter = @beforePeregon3 WHERE Code = @beforePeregon2
		INSERT INTO Layout VALUES(@beforePeregon3,@peregonName,@ShortPeregonName,@Line,@beforePeregon2,@afterPeregon2,@PeregonLength)
		UPDATE Layout SET NperegonBefore = @afterPeregon1 WHERE Code = @afterPeregon2;
	   END

	   IF @typeOfPeregonCreation = 2 /*---- выбрано создание нового последнего(по порядку следования) перегона в линии ----*/
	   BEGIN
	    UPDATE Layout SET NperegonAfter = @afterPeregon1 WHERE Code = @beforePeregon2
		INSERT INTO Layout VALUES(@afterPeregon1,@peregonName,@ShortPeregonName,@Line,@beforePeregon2,0,@PeregonLength)
	   END

	   IF @typeOfPeregonCreation = 1 /*---- выбрано создание нового первого по (порядку следования) перегона в линии ----- */
	   BEGIN
	    INSERT INTO Layout VALUES(@beforePeregon2,@peregonName,@ShortPeregonName,@Line,0,@afterPeregon1,@PeregonLength)
	    UPDATE Layout SET NperegonBefore = @beforePeregon2 WHERE Code = @afterPeregon1
	   END
END
GO
/****** Object:  StoredProcedure [dbo].[PicketAdd]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[PicketAdd] (	@ClassCode int,
									@GroupCode int,
									@Line int,
									@Track int,
									@peregonNumber int,
									@Npicket int) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;


	declare @firstOrNorPicket int
	SET @firstOrNorPicket=0
	--SELECT @firstOrNorPicket=Npicket FROM Main WHERE  Track=@Track AND ClassNum = @ClassCode AND GroupNum=@GroupCode AND LineNum=@Line AND Layout = @peregonNumber

	--/*   MAIN   */
	--IF @firstOrNorPicket!=0
	--	INSERT INTO Main VALUES(0, @Npicket,@Track,0,0,null,0,0,0,0,@peregonNumber,@ClassCode,@GroupCode,@Line)
	--ELSE
	--	UPDATE Main SET Npicket = @Npicket WHERE  Track=@Track AND ClassNum = @ClassCode AND GroupNum=@GroupCode AND LineNum=@Line AND Layout = @peregonNumber
END
GO
/****** Object:  StoredProcedure [dbo].[PicketCreate]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[PicketCreate] (	 @NpicketaDisplay nvarchar(50), @line int,@Path int,
										 @peregonNumber int,
										 @typeOfPicketCreation int, @Npicket int, @DlinaPicket int,@afterPicket1 int,@beforePicket2 int, @LeftShiftLine int, @RightShiftLine int) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;
	
	IF @typeOfPicketCreation=0
	BEGIN
		--INSERT INTO Pickets VALUES(@NpicketaDisplay,@DlinaPicket,@peregonNumber,0,@afterPicket1,@beforePicket2,@line,@Path,@LeftShiftLine,@RightShiftLine)
		UPDATE Pickets SET NpicketBefore = @Npicket WHERE number = @afterPicket1 
	END
	IF @typeOfPicketCreation=1
	BEGIN
		--INSERT INTO Pickets(Dlina) VALUES('1');
		--INSERT INTO Pickets VALUES(@NpicketaDisplay,@DlinaPicket,@peregonNumber,@beforePicket2,0,@afterPicket1,@line,@Path,@LeftShiftLine,@RightShiftLine)
		UPDATE Pickets SET NpicketAfter = @Npicket WHERE number = @beforePicket2 ;
	END
	IF @typeOfPicketCreation=3
		--INSERT INTO dbo.Pickets VALUES( @NpicketaDisplay,@DlinaPicket,@peregonNumber,0,0,@Npicket,@line,@Path,@LeftShiftLine,@RightShiftLine);
		INSERT INTO dbo.Pickets(Dlina) VALUES(1);
END
GO
/****** Object:  StoredProcedure [dbo].[PicketCreateFirst]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[PicketCreateFirst] ( 
	@NpicketaDisplay nvarchar(50),
	@Path int,
	@Npicket int, 
	@DlinaPicket int, 
	@afterPicket1 int, 
	@beforePicket2 int, 
	@LeftShiftLine int, 
	@RightShiftLine int) 
	
AS
BEGIN

	SET NOCOUNT ON;
	
	INSERT INTO Pickets VALUES( @NpicketaDisplay, @DlinaPicket, 0, 0, @Npicket, @Path, @LeftShiftLine, @RightShiftLine)
END
GO
/****** Object:  StoredProcedure [dbo].[PicketCreateLeft]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[PicketCreateLeft] (	@NpicketaDisplay nvarchar(50),
											@Path int,
											@DlinaPicket int,
											@AddingPicketID int,
											@LeftShiftLine int,
											@RightShiftLine int,
											@afterPicketID int) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;
	
	INSERT INTO Pickets VALUES(@NpicketaDisplay,@DlinaPicket,0, @afterPicketID, @AddingPicketID, @Path,@LeftShiftLine,@RightShiftLine);
	UPDATE Pickets SET NpicketBefore = @AddingPicketID WHERE number = @afterPicketID;
END
GO
/****** Object:  StoredProcedure [dbo].[PicketCreateRight]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[PicketCreateRight] ( @NpicketaDisplay nvarchar(50),@Path int,
											@DlinaPicket int,@addingPicketNumber int,@before int, @LeftShiftLine int, @RightShiftLine int) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

	INSERT INTO Pickets VALUES(@NpicketaDisplay,@DlinaPicket,@before,0,@addingPicketNumber,@Path,@LeftShiftLine,@RightShiftLine)
	UPDATE Pickets SET NpicketAfter = @addingPicketNumber WHERE number = @before 
END
GO
/****** Object:  StoredProcedure [dbo].[setPeregonLength]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
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
GO
/****** Object:  StoredProcedure [dbo].[updateEquipState]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================

CREATE PROCEDURE [dbo].[updateEquipState]( @EquipCode  int, @state int) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

	UPDATE [AllEquipments] SET EquipWorkState=@state WHERE Code = @EquipCode
END
GO
/****** Object:  StoredProcedure [dbo].[UpdateLineNum]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[UpdateLineNum] (
										@lineNumNew int,
										@lineNum int
									) 	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    -- Insert statements for procedure here
	--UPDATE [dbo].[Main] SET LineNum = @lineNumNew  WHERE LineNum=@lineNum
END
GO
/****** Object:  StoredProcedure [dbo].[UpdateNegativePickets]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[UpdateNegativePickets](@pNum int, @delta int, @lenght int)
AS
BEGIN
	SET NOCOUNT ON;
	BEGIN TRANSACTION
	
		DECLARE @RShift int, @LShift int
		DECLARE @cur_picket int
	
		SET @cur_picket = @pNum

		SELECT @LShift = StartShiftLine FROM [dbo].[Pickets] WHERE number = @pNum
		UPDATE [dbo].[Pickets] SET StartShiftLine = (@LShift - @delta) , Dlina = @lenght WHERE number = @cur_picket

		SELECT @cur_picket = NpicketBefore FROM Pickets WHERE number = @cur_picket
	
		IF @cur_picket=0 RETURN

		WHILE 1=1
		BEGIN 
			SELECT @LShift = StartShiftLine, @RShift  = EndShiftLine FROM [dbo].[Pickets] WHERE number = @cur_picket 
			UPDATE [dbo].[Pickets] SET StartShiftLine = (@LShift-@delta), EndShiftLine = (@RShift-@delta) WHERE number = @cur_picket

			exec updateObjectsOffsets @cur_picket,@delta
		
			SELECT @cur_picket = NpicketBefore FROM Pickets WHERE number = @cur_picket

			IF @cur_picket=0 BREAK
		END

	COMMIT TRANSACTION
END
GO
/****** Object:  StoredProcedure [dbo].[UpdateObject]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[UpdateObject] (	@EquipNum int,
										@Shift int
									  ) 	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    -- Insert statements for procedure here
	UPDATE [dbo].[Objects] SET shiftFromPicket = @Shift  WHERE Code = @EquipNum
END
GO
/****** Object:  StoredProcedure [dbo].[UpdateObjectArea]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[UpdateObjectArea] (	@EquipNum int,
											@X int,
											@Y int,
											@Height int,
											@Width  int,
											@AreaType int
									  ) 	
AS
BEGIN
	
	SET NOCOUNT ON;
	  
	UPDATE [dbo].[AllEquipments] SET Area_X = @X, Area_Y = @Y, Area_Height = @Height, Area_Width = @Width, Area_Type = @AreaType  WHERE Code = @EquipNum
END
GO
/****** Object:  StoredProcedure [dbo].[updateObjectCoordinate]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[updateObjectCoordinate]( @EquipCode  int, @X int, @Y int) 
	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

	UPDATE [AllEquipments] SET Area_X=@X, Area_Y=@Y WHERE Code = @EquipCode
END
GO
/****** Object:  StoredProcedure [dbo].[UpdateObjectFramesArea]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[UpdateObjectFramesArea] (	@EquipNum int,
											@Time datetime,
											@X int,
											@Y int,
											@Height int,
											@Width  int,
											@AreaType int
									  ) 	
AS
BEGIN
	
	SET NOCOUNT ON;
	  
	UPDATE [dbo].[ObjectsFrames] SET Area_X = @X, Area_Y = @Y, Area_Height = @Height, Area_Width = @Width, Area_Type = @AreaType  WHERE [ObjID] = @EquipNum AND [Time]=@Time
END
GO
/****** Object:  StoredProcedure [dbo].[updateObjectsOffsets]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
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
	SELECT Code, shiftLine FROM [AllEquipments] WHERE Picket = @pNum
	
	OPEN  cursor_objectID
	FETCH NEXT FROM cursor_objectID INTO @obj_num, @offset
	
	WHILE @@FETCH_STATUS = 0
	BEGIN 
		UPDATE [AllEquipments] SET shiftLine = (@offset - @delta) WHERE Code = @obj_num
		FETCH NEXT FROM cursor_objectID INTO @obj_num, @offset
	END

	CLOSE cursor_objectID
	DEALLOCATE cursor_objectID
END
GO
/****** Object:  StoredProcedure [dbo].[updateOrder]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[updateOrder] (
										@ID int,
										@Person nvarchar(MAX),
										@Desc   nvarchar(MAX),
										@CreationDate DATE,
										@FirstDate DATE,
										@FinishDate DATE,
										@State int,
										@result int OUT
									) 	
AS
BEGIN
	SET NOCOUNT ON;
	BEGIN TRAN
		BEGIN TRY
			UPDATE [dbo].[Orders] SET [Orders].Person = @Person, [Orders].[Desc] = @Desc, [Orders].CreationDate = @CreationDate, [Orders].FirstDate = @FirstDate, [Orders].FinishDate=@FinishDate,[Orders].[State] = @State   WHERE [Orders].ID = @ID
		END TRY
		BEGIN CATCH
			SELECT 
			ERROR_NUMBER() AS ErrorNumber
			,ERROR_SEVERITY() AS ErrorSeverity
			,ERROR_STATE() AS ErrorState
			,ERROR_PROCEDURE() AS ErrorProcedure
			,ERROR_LINE() AS ErrorLine
			,ERROR_MESSAGE() AS ErrorMessage;

			IF @@TRANCOUNT > 0
			BEGIN
				ROLLBACK TRANSACTION;
				SET	@result = 0
			END
		END CATCH;

	IF @@TRANCOUNT > 0
	BEGIN
		COMMIT TRANSACTION;
	    SET	@result = 1
	END
END
GO
/****** Object:  StoredProcedure [dbo].[UpdatePicket]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[UpdatePicket] (	@EquipNum int,
										@leftShift int,
										@rightShift int,
										@lenght int
									  ) 	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    -- Insert statements for procedure here
	UPDATE [dbo].[Pickets] SET StartShiftLine = @leftShift, EndShiftLine =@rightShift, Dlina = @lenght  WHERE number = @EquipNum
END
GO
/****** Object:  StoredProcedure [dbo].[UpdatePickets]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[UpdatePickets](@pNum int, @delta int)
AS
BEGIN
	SET NOCOUNT ON;
	
	DECLARE @next_picket int
	DECLARE @while_stop bit
	SET @while_stop = 1	
	
	WHILE @while_stop=1
	BEGIN 
		SELECT @next_picket = number FROM Pickets WHERE NpicketAfter = @next_picket
	END
END
GO
/****** Object:  StoredProcedure [dbo].[UpdatePositivePickets]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>	

-- =============================================
CREATE PROCEDURE [dbo].[UpdatePositivePickets](@pNum int, @delta int, @lenght int)
AS
BEGIN
	SET NOCOUNT ON;
	BEGIN TRANSACTION
	
		DECLARE @RShift int, @LShift int
		DECLARE @cur_picket int
	
		SET @cur_picket = @pNum
		
		-- Set first for change picket 
		SELECT @RShift = EndShiftLine FROM [dbo].[Pickets] WHERE number = @pNum
		UPDATE [dbo].[Pickets] SET EndShiftLine = (@RShift - @delta) , Dlina = @lenght WHERE number = @cur_picket

		-- Get next picket
		SELECT @cur_picket = NpicketAfter FROM Pickets WHERE number = @cur_picket
	
		-- If we change last picket when return
		IF @cur_picket=0 RETURN

		WHILE 1=1
		BEGIN 
			SELECT @LShift = StartShiftLine, @RShift = EndShiftLine FROM [dbo].[Pickets] WHERE number = @cur_picket 
			UPDATE [dbo].[Pickets] SET StartShiftLine = (@LShift-@delta) , EndShiftLine = (@RShift-@delta) WHERE number = @cur_picket

			exec updateObjectsOffsets @cur_picket,@delta
		
			SELECT @cur_picket = NpicketAfter FROM Pickets WHERE number = @cur_picket

			IF @cur_picket=0 BREAK
		END

	COMMIT TRANSACTION
END
GO
/****** Object:  StoredProcedure [dbo].[UpdateShiftBeginEquip]    Script Date: 4/8/2018 1:01:18 PM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[UpdateShiftBeginEquip] (	@EquipNum int,
													@Shift int
											   ) 	
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    -- Insert statements for procedure here
	UPDATE [dbo].[AllEquipments] SET shiftFromPicket = @Shift  WHERE Code = @EquipNum
END
GO
