using System.IO;
using System.Linq;
using System.Windows;

namespace WpfApplication1
{
	public partial class MainWindow : Window
	{
		public MainWindow()
		{
			InitializeComponent();

			this.DataContext = new DirectoryInfo(@"D:\images").EnumerateFiles().Select(x => x.FullName);
		}
	}
}
