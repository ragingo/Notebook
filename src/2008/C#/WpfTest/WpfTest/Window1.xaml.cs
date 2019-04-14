using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Media3D;

namespace WpfTest
{
	/// <summary>
	/// Window1.xaml の相互作用ロジック
	/// </summary>
	public partial class Window1 : Window
	{
		public Window1()
		{
			InitializeComponent();

			//DoubleAnimation da = new DoubleAnimation(1,0.2, new Duration(new TimeSpan(0,0,1)), FillBehavior.HoldEnd);
			//da.AutoReverse = true;
			//da.RepeatBehavior = RepeatBehavior.Forever;
			//mediaElement1.BeginAnimation(UIElement.OpacityProperty, da);

			Viewport3D vp = new Viewport3D();

			Model3DGroup group = new Model3DGroup();
			GeometryModel3D model = new GeometryModel3D();
			model.Geometry = CreateTriangle();
			model.Material = new DiffuseMaterial(Brushes.Red);
			group.Children.Add(model);

			PerspectiveCamera camera = new PerspectiveCamera();
			camera.LookDirection = new Vector3D(0, 0, -1);
			camera.Position = new Point3D(0, 0, 12);
			camera.FieldOfView = 45;
			camera.UpDirection = new Vector3D(0, 1, 0);
			vp.Camera = camera;

			PointLight light = new PointLight();
			light.Position = new Point3D(12, 12, 12);
			light.Color = Colors.White;
			light.Range = 150;
			light.ConstantAttenuation = 1.0;
			group.Children.Add(light);

			ModelVisual3D scene = new ModelVisual3D();
			scene.Content = group;
			vp.Children.Add(scene);

			//this.Content = vp;
		}

		private void Button_Click(object sender, RoutedEventArgs e)
		{
			Button btn = (Button)sender;
			//MessageBox.Show(btn.Content.ToString());
			new Window2().ShowDialog();
		}

		private void inkCanvas_StrokeCollected(object sender, InkCanvasStrokeCollectedEventArgs e)
		{
			overlay.Children.Clear();
		}

		private void inkCanvas_Gesture(object sender, InkCanvasGestureEventArgs e)
		{
			textBox2.Text = e.GetGestureRecognitionResults()[0].ApplicationGesture.ToString();
			inkCanvas.Strokes.Clear();
		}

		private MeshGeometry3D CreateTriangle()
		{
			// エッセンシャルWPF p.238参照
			MeshGeometry3D mesh = new MeshGeometry3D();
			mesh.Positions.Add(new Point3D(0, 0, 0));
			mesh.Positions.Add(new Point3D(3, 0, 0));
			mesh.Positions.Add(new Point3D(1.5, 2.5, 0));
			mesh.TriangleIndices.Add(0);
			mesh.TriangleIndices.Add(1);
			mesh.TriangleIndices.Add(2);
			return mesh;
		}
	}
}
