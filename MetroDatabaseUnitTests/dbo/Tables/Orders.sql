CREATE TABLE [dbo].[Orders] (
    [ID]           INT            NULL,
    [Person]       NVARCHAR (MAX) NULL,
    [Desc]         NVARCHAR (MAX) NULL,
    [CreationDate] DATE           NULL,
    [FirstDate]    DATE           NULL,
    [FinishDate]   DATE           NULL,
    [State]        INT            NULL,
    [id_equipment] INT            NULL
);

