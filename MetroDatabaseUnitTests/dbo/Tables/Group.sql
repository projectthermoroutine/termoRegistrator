CREATE TABLE [dbo].[Group] (
    [Code]   SMALLINT       NOT NULL,
    [Class]  TINYINT        NOT NULL,
    [Group]  NVARCHAR (20)  COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
    [ObjSrc] NVARCHAR (MAX) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
    [PtSrc]  NVARCHAR (MAX) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
    [Color]  NVARCHAR (10)  NULL
);

