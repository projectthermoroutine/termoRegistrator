CREATE PROCEDURE [dbo].[NewSelectCommand]
AS
	SET NOCOUNT ON;
--SELECT     Main.Code, Main.Layout, Main.Track, Main.Border, Main.ObjData, Main.PtData, Main.Point, Main.Displacement, Main.Y, Objects.Object AS ObjName, 
--                      [Group].[Group] AS GrpName, Class.Class AS ClsName, Layout.Layout AS LtName, Layout.SName AS LtSName, Objects.[Group], [Group].Class, Main.Area, 
--                      CAST(ROUND(Layout.Code / 100000, 0) AS INT) AS Line
--FROM         Main INNER JOIN
--                      Objects ON Main.Code = Objects.Code INNER JOIN
--                      [Group] ON Objects.[Group] = [Group].Code INNER JOIN
--                      Class ON [Group].Class = Class.Code INNER JOIN
--                      Layout ON Main.Layout = Layout.Code
--ORDER BY [Group].Class, Objects.[Group], CAST(ROUND(Main.Layout / 100000, 0) AS INT), Main.Track, Main.Layout, Main.Code