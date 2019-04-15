using System;
using System.Threading;

namespace heart_rotation2
{
	class Program
	{
		static void Main(string[] args)
		{
			const int msoShapeHeart = 21;
			const int msoShapeSmileyFace = 17;

			dynamic xlApp = Activator.CreateInstance(Type.GetTypeFromProgID("Excel.Application"));
			xlApp.DisplayAlerts = false;
			xlApp.Visible = true;

			dynamic xlBook = xlApp.Workbooks.Add();
			dynamic xlSheet = xlBook.Worksheets[1];
			// Top
			xlSheet.Shapes.AddShape(msoShapeHeart, 100, 50, 60, 60).Select();
			dynamic heart_top = xlApp.Selection.ShapeRange;
			heart_top.Name = "Heart_Top";
			heart_top.Fill.ForeColor.SchemeColor = 45;
			heart_top.Fill.Visible = true;
			heart_top.Fill.Solid();

			// Right
			xlApp.Selection.ShapeRange.Duplicate.Select();
			dynamic heart_right = xlApp.Selection.ShapeRange;
			heart_right.Name = "Heart_Right";
			heart_right.Left = heart_top.Left + heart_top.Width / 2;
			heart_right.Top = heart_top.Top + heart_top.Height / 2;
			heart_right.Rotation = 90.0;

			// Left
			xlApp.Selection.ShapeRange.Duplicate.Select();
			dynamic heart_left = xlApp.Selection.ShapeRange;
			heart_left.Name = "Heart_Left";
			heart_left.Left = heart_top.Left - heart_top.Width / 2;
			heart_left.Top = heart_top.Top + heart_top.Height / 2;
			heart_left.Rotation = -90.0;

			// Bottom
			xlApp.Selection.ShapeRange.Duplicate.Select();
			dynamic heart_bottom = xlApp.Selection.ShapeRange;
			heart_bottom.Name = "Heart_Bottom";
			heart_bottom.Left = heart_top.Left;
			heart_bottom.Top = heart_top.Top + heart_top.Height;
			heart_bottom.Rotation = 180.0;

			// Center
			xlSheet.Shapes.AddShape(msoShapeSmileyFace, 100, 85, 60, 60).Select();
			dynamic smile_center = xlApp.Selection.ShapeRange;
			smile_center.Name = "Smile_Center";

			// Flower
			xlSheet.Shapes.SelectAll();
			xlApp.Selection.Group.Select();
			dynamic flower = xlApp.Selection.ShapeRange;
			flower.Name = "Flower";

			// rotation
			ulong count = 0;
			while (count < 360*5)
			{
				flower.Rotation = count++ % 360;
				flower.Left += 0.5;
				Thread.Sleep(1);
			}

			Thread.Sleep(3000);

			xlBook.Close();
			xlApp.Quit();
		}
	}
}
