
var msoShapeHeart = 21;
var msoShapeSmileyFace = 17;
var msoThreeD1 = 1;

var xlApp = WScript.CreateObject("Excel.Application");
xlApp.DisplayAlerts = false;
xlApp.Visible = true;

var xlBook = xlApp.Workbooks.Add();
var xlSheet = xlBook.Worksheets.Item(1);

// Top
xlSheet.Shapes.AddShape(msoShapeHeart, 100, 50, 60, 60).Select();
var heart_top = xlApp.Selection.ShapeRange;
heart_top.Name = "Heart_Top";
heart_top.Fill.ForeColor.SchemeColor = 45;
heart_top.Fill.Visible = true;
heart_top.Fill.Solid()

// Right
xlApp.Selection.ShapeRange.Duplicate().Select();
var heart_right = xlApp.Selection.ShapeRange;
heart_right.Name = "Heart_Right";
heart_right.Left = heart_top.Left + heart_top.Width / 2;
heart_right.Top = heart_top.Top + heart_top.Height / 2;
heart_right.Rotation = 90.0;

// Left
xlApp.Selection.ShapeRange.Duplicate().Select();
var heart_left = xlApp.Selection.ShapeRange;
heart_left.Name = "Heart_Left";
heart_left.Left = heart_top.Left - heart_top.Width / 2;
heart_left.Top = heart_top.Top + heart_top.Height / 2;
heart_left.Rotation = -90.0;

// Bottom
xlApp.Selection.ShapeRange.Duplicate().Select();
var heart_bottom = xlApp.Selection.ShapeRange;
heart_bottom.Name = "Heart_Bottom";
heart_bottom.Left = heart_top.Left;
heart_bottom.Top = heart_top.Top + heart_top.Height;
heart_bottom.Rotation = 180.0;

// Center
//xlSheet.Shapes.AddShape(msoShapeSmileyFace, 100, 85, 60, 60).Select();
//smile_center = xlApp.Selection.ShapeRange;
//smile_center.Name = "Smile_Center";

// Flower
xlSheet.Shapes.SelectAll();
xlApp.Selection.Group.Select();
var flower = xlApp.Selection.ShapeRange;
flower.Name = "Flower";
flower.ThreeD.SetThreeDFormat(msoThreeD1);
flower.ThreeD.Depth = 5;

// rotation
var r = 0.0
var i = 0
while(i++ < 360*5)
{
	flower.ThreeD.RotationY = r++;
	if (r == 90) {
		r = -90
	}
	WScript.Sleep(1);
}

WScript.Sleep(3000);

xlBook.Close();
xlApp.Quit();
