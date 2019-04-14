using System.Collections.ObjectModel;
using System.Linq;
using System.Text.RegularExpressions;
using GalaSoft.MvvmLight;
using GalaSoft.MvvmLight.Command;
using System.Collections.Generic;
using System;
using System.Windows;

namespace Android_Explorer
{
	class MainViewModel : ViewModelBase
	{
		private string _selectedDevice;
		public string SelectedDevice
		{
			get { return _selectedDevice; }
			set
			{
				if (_selectedDevice != value)
				{
					_selectedDevice = value;
					RaisePropertyChanged("SelectedDevice");
					FileSystemInfosViewModel.Root.First().ClearChildren();
				}
			}
		}

		public ObservableCollection<string> Devices { get; private set; }
		public FileSystemInfosViewModel FileSystemInfosViewModel { get; private set; }

		private RelayCommand<IEnumerable<object>> _deleteFilesCommand;
		public RelayCommand<IEnumerable<object>> DeleteFilesCommand
		{
			get
			{
				return _deleteFilesCommand ?? (_deleteFilesCommand = new RelayCommand<IEnumerable<object>>(DeleteFiles));
			}
		}

		public MainViewModel()
		{
			FileSystemInfosViewModel = new FileSystemInfosViewModel();
			Devices = new ObservableCollection<string>();

			LoadDevices();
		}

		private void LoadDevices()
		{
			Devices.Clear();
			Devices.Add(string.Empty);
			foreach (string line in new Adb().Execute("devices").Skip(1))
			{
				string[] items = Regex.Replace(line, "[ ]+", "\t").Split('\t');
				string serial = items[0];
				string type = items[1];
				Devices.Add(serial);
			}
		}

		private void DeleteFiles(IEnumerable<object> items)
		{
			var targets = items.Cast<AndroidFileInfo>().ToList();
			string names = targets.Select(x => x.FileName).Aggregate((x, y) => x += Environment.NewLine + y);
			string msg = "以下のファイルを削除します。よろしいですか？" + Environment.NewLine + names;

			if (MessageBox.Show(msg, "確認", MessageBoxButton.OKCancel, MessageBoxImage.Question) == MessageBoxResult.OK)
			{
				foreach (var info in targets)
				{
					info.Remove();
				}
			}
		}
	}
}
