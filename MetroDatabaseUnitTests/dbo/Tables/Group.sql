CREATE TABLE [dbo].[Group] (
    [Code]   INT            NOT NULL,
    [Class]  INT            NULL,
    [Group]  NVARCHAR (20)  NOT NULL,
    [ObjSrc] NVARCHAR (MAX) NULL,
    [PtSrc]  NVARCHAR (MAX) NULL,
    [Color]  NVARCHAR (10)  NULL,
    PRIMARY KEY CLUSTERED ([Code] ASC),
    FOREIGN KEY ([Class]) REFERENCES [dbo].[Class] ([Code])
);



