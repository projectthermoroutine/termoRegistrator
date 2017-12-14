CREATE TABLE [dbo].[Main] (
    [GroupId]     INT NULL,
    [EquipmentId] INT NULL,
    FOREIGN KEY ([EquipmentId]) REFERENCES [dbo].[Equipments] ([Code]),
    FOREIGN KEY ([GroupId]) REFERENCES [dbo].[Group] ([Code])
);



