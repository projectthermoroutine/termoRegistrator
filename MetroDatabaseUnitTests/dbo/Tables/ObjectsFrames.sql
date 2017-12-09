CREATE TABLE [dbo].[ObjectsFrames] (
    [ObjID]           INT            NOT NULL,
    [FilePath]        NVARCHAR (MAX) NOT NULL,
    [FrameCoordinate] BIGINT         NOT NULL,
    [Time]            DATETIME       NOT NULL,
    [Area_X]          INT            NULL,
    [Area_Y]          INT            NULL,
    [Area_Height]     INT            NULL,
    [Area_Width]      INT            NULL,
    [Area_Type]       INT            NULL
);

