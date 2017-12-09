CREATE TABLE [dbo].[Pickets] (
    [Npiketa]        NVARCHAR (50) NULL,
    [Dlina]          INT           NULL,
    [Peregon]        INT           NULL,
    [NpicketBefore]  INT           NULL,
    [NpicketAfter]   INT           NULL,
    [number]         INT           NOT NULL,
    [line]           INT           NULL,
    [path]           INT           NULL,
    [StartShiftLine] INT           NULL,
    [EndShiftLine]   INT           NULL,
    [Class]          INT           NULL,
    [Group]          INT           NULL
);

