CREATE TABLE [dbo].[Lines] (
    [LineNum]         INT           NOT NULL,
    [LineName]        NVARCHAR (50) NULL,
    [StartCoordinate] BIGINT        NULL,
    [LineCode]        VARCHAR (100) NULL,
    PRIMARY KEY CLUSTERED ([LineNum] ASC)
);



