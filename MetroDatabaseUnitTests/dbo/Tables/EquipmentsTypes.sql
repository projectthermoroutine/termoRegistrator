CREATE TABLE [dbo].[EquipmentsTypes] (
    [Id]             INT        NOT NULL,
    [Name]           NCHAR (10) NULL,
    [MinTemperature] INT        NULL,
    [MaxTemperature] INT        NULL,
    [Width]          INT        NULL,
    [Height]         INT        NULL,
    PRIMARY KEY CLUSTERED ([Id] ASC)
);

