using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace Android_Explorer
{
	public partial class MainWindow : Window
	{
		public MainWindow()
		{
			InitializeComponent();

			var vm = new MainViewModel();
			DataContext = vm;
			dataGrid.DataContext = vm.FileSystemInfosViewModel.Root;
		}

		private void TreeNode_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
		{
			var txt = sender as TextBlock;
			var info = txt.DataContext as AndroidFileInfo;
			if (info.IsDirectory)
			{
				Task.Factory.StartNew((obj) =>
				{
					string device = obj as string;
					info.LoadChildren(device);
				}, cmbDevices.SelectedItem);
			}
			dataGrid.DataContext = info;
			cmbLocation.Items.Add(info.Path);
		}

		private void btnReload_Click(object sender, RoutedEventArgs e)
		{
			var info = dataGrid.DataContext as AndroidFileInfo;
			if (info == null)
			{
				e.Handled = true;
				return;
			}
			string device = cmbDevices.SelectedItem as string;
			info.ReloadChildren(device);
		}

		private void btnPull_Click(object sender, RoutedEventArgs e)
		{
			if (MessageBox.Show("取得しますか？", "確認", MessageBoxButton.YesNo, MessageBoxImage.Question) == MessageBoxResult.Yes)
			{
				var targets = dataGrid.SelectedItems.Cast<AndroidFileInfo>().ToList();
				foreach (var info in targets)
				{
					info.Download(cmbDevices.SelectedItem as string);
				}
			}
		}

		private void cmbLocation_TextInput(object sender, TextCompositionEventArgs e)
		{

		}

		private void dataGrid_MouseDoubleClick(object sender, MouseButtonEventArgs e)
		{
			var info = dataGrid.SelectedItem as AndroidFileInfo;
			if (MessageBox.Show("表示する為、一時フォルダに保存されますがよろしいですか？", "確認", MessageBoxButton.OKCancel, MessageBoxImage.Question) == MessageBoxResult.OK)
			{
				info.Download(cmbDevices.SelectedItem as string);
				Process.Start(
					Path.Combine(AndroidSettings.DefaultTemporaryDirectory.FullName, info.FileName)
				);
			}
		}

		private void btnNavigate_Click(object sender, RoutedEventArgs e)
		{
			string path = cmbLocation.Text;
		}
	}
}
