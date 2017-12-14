CREATE TABLE [dbo].[Pickets] (
    [Npiketa]        NVARCHAR (50) NULL,
    [Dlina]          INT           NULL,
    [NpicketBefore]  INT           NULL,
    [NpicketAfter]   INT           NULL,
    [number]         INT           NOT NULL,
    [path]           INT           NOT NULL,
    [StartShiftLine] INT           NULL,
    [EndShiftLine]   INT           NULL,
    PRIMARY KEY CLUSTERED ([number] ASC),
    FOREIGN KEY ([path]) REFERENCES [dbo].[Tracks] ([ID])
);



