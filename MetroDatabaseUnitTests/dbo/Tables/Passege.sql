CREATE TABLE [dbo].[Passege] (
    [ID]              INT            IDENTITY (1, 1) NOT NULL,
    [ObjID]           INT            NOT NULL,
    [FilePath]        NVARCHAR (MAX) NOT NULL,
    [FrameIndex]      INT            NOT NULL,
    [FrameCoordinate] BIGINT         NOT NULL,
    [Time]            DATETIME       NOT NULL
);

