using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Media3D;

namespace WpfTest2
{
	public partial class Window1 : Window
	{
		public static readonly DependencyProperty DraggedProperty =
			DependencyProperty.RegisterAttached("Dragged", typeof(bool), typeof(Window1),
			new PropertyMetadata(false));

		public static void SetDragged(DependencyObject target, bool value)
		{
			target.SetValue(DraggedProperty, value);
		}
		public static bool GetDragged(DependencyObject target)
		{
			return (bool)target.GetValue(DraggedProperty);
		}


		public static readonly DependencyProperty StartPointProperty =
			DependencyProperty.RegisterAttached("StartPoint",
			typeof(Point),
			typeof(Window1),
			new UIPropertyMetadata(new Point()));

		public static Point GetStartPoint(DependencyObject obj)
		{
			return (Point)obj.GetValue(StartPointProperty);
		}

		public static void SetStartPoint(DependencyObject obj, Point value)
		{
			obj.SetValue(StartPointProperty, value);
		}
		public Window1()
		{
			InitializeComponent();
		}

		private void Window_Loaded(object sender, RoutedEventArgs e)
		{
		}

		private void viewport_MouseDown(object sender, MouseButtonEventArgs e)
		{
		}

		private void viewport_MouseUp(object sender, MouseButtonEventArgs e)
		{
		}

		private void viewport_MouseMove(object sender, MouseEventArgs e)
		{
		}
	}
}
