CREATE TABLE [dbo].[Layout] (
    [Code]           INT            NOT NULL,
    [Layout]         NVARCHAR (100) NOT NULL,
    [SName]          NVARCHAR (20)  NULL,
    [Line]           INT            NULL,
    [NperegonBefore] INT            NULL,
    [NperegonAfter]  INT            NULL,
    [PeregonLength]  INT            NULL
);

