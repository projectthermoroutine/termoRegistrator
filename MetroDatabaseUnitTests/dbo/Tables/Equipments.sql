CREATE TABLE [dbo].[Equipments] (
    [Code]               INT          NOT NULL,
    [Group]              SMALLINT     NULL,
    [Name]               VARCHAR (50) NULL,
    [shiftLine]          BIGINT       NULL,
    [x]                  INT          NULL,
    [y]                  INT          NULL,
    [curTemperature]     INT          NULL,
    [maxTemperature]     INT          NULL,
    [regularly]          INT          NULL,
    [shiftFromPicket]    INT          NULL,
    [typeId]             INT          NULL,
    [typeEquip]          INT          NOT NULL,
    [strelkaLeftOrRight] INT          NULL,
    [Picket]             INT          NOT NULL,
    [Path]               INT          NOT NULL,
    [Line]               INT          NULL,
    [ObjectLenght]       INT          NULL,
    [Area_X]             INT          NULL,
    [Area_Y]             INT          NULL,
    [Area_Width]         INT          NULL,
    [Area_Height]        INT          NULL,
    [Area_Type]          INT          NULL,
    PRIMARY KEY CLUSTERED ([Code] ASC)
);





