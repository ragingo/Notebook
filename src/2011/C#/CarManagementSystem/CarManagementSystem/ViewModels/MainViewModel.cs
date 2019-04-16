using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Windows;
using System.Windows.Input;
using CarManagementSystem.Models;
using CarManagementSystem.Views;
using Microsoft.Practices.Prism.Commands;

namespace CarManagementSystem.ViewModels
{
	public class MainViewModel : ViewModelBase, IViewModel
	{
		private Dictionary<Type, Window> _windows = new Dictionary<Type, Window>();

		private IView _view;
		public IView View
		{
			get
			{
				return _view;
			}
			set
			{
				_view = value;
				(_view as Window).Closing += MainView_Closing;
			}
		}

		public ICommand ShowCustomerManagementCommand
		{
			get;
			private set;
		}

		public ICommand ShowCarManagementCommand
		{
			get;
			private set;
		}

		public ICommand ShowWorkHistoryCommand
		{
			get;
			private set;
		}

		public ICommand ShowSearchCommand
		{
			get;
			private set;
		}

		public ICommand BackupCommand
		{
			get;
			private set;
		}

		public MainViewModel()
		{
			ShowCustomerManagementCommand = new DelegateCommand(ShowCustomerManagement);
			ShowCarManagementCommand = new DelegateCommand(ShowCarManagement);
			ShowWorkHistoryCommand = new DelegateCommand(ShowWorkHistory);
			ShowSearchCommand = new DelegateCommand(ShowSearch);
			BackupCommand = new DelegateCommand(Backup);
		}

		private void ShowWindow(Type windowType)
		{
			if (!_windows.ContainsKey(windowType) || _windows[windowType] == null)
			{
				_windows[windowType] = Activator.CreateInstance(windowType) as Window;
				_windows[windowType].Show();
			}
			else
			{
				_windows[windowType].Activate();
			}
			_windows[windowType].Closed += delegate
			{
				_windows[windowType] = null;
			};
		}

		private void ShowCustomerManagement()
		{
			ShowWindow(typeof(CustomerManagementView));
		}

		private void ShowCarManagement()
		{
			ShowWindow(typeof(CarManagementView));
		}

		private void ShowWorkHistory()
		{
			ShowWindow(typeof(WorkHistoryView));
		}

		private void ShowSearch()
		{
			ShowWindow(typeof(SearchView));
		}

		private void Backup()
		{
			using (var ctx = new CmsDbContext())
			{
				string dbFileName = ctx.Database.Connection.DataSource;
				string backupFileName = 
					string.Format(
						"{0}_{1:yyyyMMddHHmmss}.sdf",
						Path.ChangeExtension(dbFileName, null),
						DateTime.Now
					);
				File.Copy(dbFileName, backupFileName);
			}
			MessageBox.Show("完了しました。");
		}

		private void CloseWindows()
		{
			_windows.Where(x => x.Value != null)
				.ToList()
				.ForEach(x => x.Value.Close());
		}

		private void MainView_Closing(object sender, CancelEventArgs e)
		{
			var result =
				MessageBox.Show("バックアップしましたか？", "確認", MessageBoxButton.YesNo, MessageBoxImage.Question);
			if (result == MessageBoxResult.Yes)
			{
				CloseWindows();
				return;
			}
			else
			{
				result =
					MessageBox.Show("バックアップします。よろしいですか？", "確認", MessageBoxButton.YesNo, MessageBoxImage.Question);
				if (result == MessageBoxResult.Yes)
				{
					Backup();

					result =
						MessageBox.Show("終了しますか？", "確認", MessageBoxButton.YesNo, MessageBoxImage.Question);
					if (result == MessageBoxResult.No)
					{
						e.Cancel = true;
						return;
					}
				}
				else
				{
					CloseWindows();
				}
			}
		}
	}
}
