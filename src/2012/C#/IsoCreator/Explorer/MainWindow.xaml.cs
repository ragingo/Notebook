using System.Windows;
using System.Windows.Controls;
using Explorer.Model;

namespace Explorer
{
	public partial class MainWindow : Window
	{
		public MainWindow()
		{
			InitializeComponent();
		}

		private void TreeView_Expanded(object sender, RoutedEventArgs e)
		{
			var item = e.OriginalSource as TreeViewItem;
			if (item.HasHeader)
			{
				var node = item.Header as FileSystemNode;
				node.ExpandChildren();
			}
		}
	}
}
