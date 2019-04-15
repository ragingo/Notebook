require 'win32ole'

msoShapeHeart = 21
msoShapeSmileyFace = 17
msoThreeD1 = 1
msoTrue = true

xlApp = WIN32OLE.new('Excel.Application')

xlApp.DisplayAlerts = false
xlApp.Visible = true
xlBook = xlApp.Workbooks.Add
xlSheet = xlBook.Worksheets.Item(1)

# Top
xlSheet.Shapes.AddShape(msoShapeHeart, 100, 50, 60, 60).Select
heart_top = xlApp.Selection.ShapeRange
heart_top.Name = "Heart_Top"
heart_top.Fill.ForeColor.SchemeColor = 45
heart_top.Fill.Visible = msoTrue
heart_top.Fill.Solid

# Right
xlApp.Selection.ShapeRange.Duplicate.Select
heart_right = xlApp.Selection.ShapeRange
heart_right.Name = "Heart_Right"
heart_right.Left = heart_top.Left + heart_top.Width / 2
heart_right.Top = heart_top.Top + heart_top.Height / 2
heart_right.Rotation = 90.0

# Left
xlApp.Selection.ShapeRange.Duplicate.Select
heart_left = xlApp.Selection.ShapeRange
heart_left.Name = "Heart_Left"
heart_left.Left = heart_top.Left - heart_top.Width / 2
heart_left.Top = heart_top.Top + heart_top.Height / 2
heart_left.Rotation = -90.0

# Bottom
xlApp.Selection.ShapeRange.Duplicate.Select
heart_bottom = xlApp.Selection.ShapeRange
heart_bottom.Name = "Heart_Bottom"
heart_bottom.Left = heart_top.Left
heart_bottom.Top = heart_top.Top + heart_top.Height
heart_bottom.Rotation = 180.0

# Center
#xlSheet.Shapes.AddShape(msoShapeSmileyFace, 100, 85, 60, 60).Select
#smile_center = xlApp.Selection.ShapeRange
#smile_center.Name = "Smile_Center"

# Flower
xlSheet.Shapes.SelectAll #xlSheet.Shapes.Range(["Heart_Top", "Heart_Right", "Heart_Left", "Heart_Bottom", "Smile_Center"]).Select # Range‚Å—Ž‚¿‚é
xlApp.Selection.Group.Select
flower = xlApp.Selection.ShapeRange
flower.Name = "Flower"
flower.ThreeD.SetThreeDFormat msoThreeD1
flower.ThreeD.Depth = 5

# rotation
count = 0
#while count < 360*5
#	count+=1
#	flower.Rotation = count % 360
#	sleep 0.001
#end
r = 0.0
(0..180*5).each do |i|
	puts r
	flower.ThreeD.RotationY = r
	flower.Left = flower.Left + 1
	r = r + 1
	if r == 90 then
		r = -90
	end
	sleep 0.001
end

sleep 3

xlBook.Close
xlApp.Quit
