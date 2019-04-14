using Explorer.Model;
using Explorer.ViewModel.Common;

namespace Explorer.ViewModel.MainWindow
{
	class MainWindowViewModel : ViewModelBase
	{
		public FileSystemTree Tree
		{
			get;
			set;
		}

		public MainWindowViewModel()
		{
			Tree = new FileSystemTree();
		}
	}
}
