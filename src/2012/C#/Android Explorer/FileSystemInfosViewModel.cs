using System.Collections.Generic;
using System.Collections.ObjectModel;
using GalaSoft.MvvmLight;

namespace Android_Explorer
{
	class FileSystemInfosViewModel : ViewModelBase
	{
		public ObservableCollection<AndroidFileInfo> Root { get; private set; }
		public FileSystemInfosViewModel()
		{
			Root = new ObservableCollection<AndroidFileInfo>();
			Root.Add(new AndroidFileInfo { FileName = "/", Path = "/", IsDirectory = true });
		}
	}
}
