CREATE TABLE [dbo].[EquipmentFilter_Tbl] (
    [filter_id]      INT            NOT NULL,
    [class_id]       INT            NOT NULL,
    [class_Name]     NVARCHAR (50)  NOT NULL,
    [groups_Names]   NVARCHAR (MAX) NOT NULL,
    [groups_Numbers] NVARCHAR (MAX) NOT NULL,
    [apply]          INT            NOT NULL
);

