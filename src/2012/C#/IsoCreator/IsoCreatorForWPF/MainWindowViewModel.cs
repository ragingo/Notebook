using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows.Threading;
using GalaSoft.MvvmLight.Command;
using Microsoft.Win32;

namespace IsoCreatorForWPF
{
	public class MainWindowViewModel : ViewModelBase
	{
		private readonly Dispatcher UIDispatcher = null;

		private TreeViewViewModel _tree;
		public TreeViewViewModel Tree
		{
			get { return _tree; }
			set
			{
				Util.RaisePropertyChanged(this, nameof(Tree), ref _tree, ref value);
			}
		}

		private TreeViewViewModel _exportTree;
		public TreeViewViewModel ExportTree
		{
			get { return _exportTree; }
			set
			{
				Util.RaisePropertyChanged(this, nameof(ExportTree), ref _exportTree, ref value);
			}
		}

		private string _inputDirectoryPath;
		public string InputDirectoryPath
		{
			get { return _inputDirectoryPath; }
			set
			{
				Util.RaisePropertyChanged(this, nameof(InputDirectoryPath), ref _inputDirectoryPath, ref value);
			}
		}

		private string _outputFilePath;
		public string OutputFilePath
		{
			get { return _outputFilePath; }
			set
			{
				Util.RaisePropertyChanged(this, nameof(OutputFilePath), ref _outputFilePath, ref value);
			}
		}

		private ICommand _openDirectoryCommand;
		public ICommand OpenDirectoryCommand
		{
			get
			{
				return _openDirectoryCommand ?? (_openDirectoryCommand = new RelayCommand(OpenDirectory));
			}
		}

		private ICommand _saveFileCommand;
		public ICommand SaveFileCommand
		{
			get
			{
				return _saveFileCommand ?? (_saveFileCommand = new RelayCommand(SaveFile));
			}
		}

		private ICommand _executeCommand;
		public ICommand ExecuteCommand
		{
			get
			{
				return _executeCommand ?? (_executeCommand = new RelayCommand(Execute));
			}
		}

		private ICommand _changeSelectionCommand;
		public ICommand ChangeSelectionCommand
		{
			get
			{
				return _changeSelectionCommand ?? (_changeSelectionCommand = new RelayCommand<TreeViewItemViewModel>(ChangeSelection));
			}
		}

		public MainWindowViewModel()
		{
			UIDispatcher = Dispatcher.CurrentDispatcher;
			Tree = new TreeViewViewModel();
			ExportTree = new TreeViewViewModel();
		}

		private void ChangeSelection(TreeViewItemViewModel selectedNode)
		{
			Task.Factory.StartNew(() =>
			{
				var target =
					_nodes
						.Where(x => x.Value.Item.FullName.StartsWith(selectedNode.Item.FullName))
						.Where(x => x.Value.Level != selectedNode.Level);

				foreach (var child in target)
				{
					child.Value.IsSelected = selectedNode.IsSelected;
				}

				Action<TreeViewItemViewModel> changeParentsCheckState = null;
				changeParentsCheckState = (TreeViewItemViewModel n) =>
				{
					if (n.Parent == null)
					{
						return;
					}
					TreeViewItemViewModel parent;
					if (!n.Parent.TryGetTarget(out parent))
					{
						return;
					}
					if (parent.Children == null)
					{
						return;
					}
					int count = parent.Children.Count;
					int falseCount = parent.Children.Count(x => x.IsSelected.HasValue && !x.IsSelected.Value);
					int trueCount = parent.Children.Count(x => x.IsSelected.HasValue && x.IsSelected.Value);
					if (count == trueCount)
					{
						parent.IsSelected = true;
					}
					else if (count == falseCount)
					{
						parent.IsSelected = false;
					}
					else
					{
						parent.IsSelected = null;
					}

					changeParentsCheckState(parent);
				};
				changeParentsCheckState(selectedNode);
			});
		}


		private Dictionary<string, TreeViewItemViewModel> _nodes = new Dictionary<string, TreeViewItemViewModel>();

		private void OpenDirectory()
		{
			foreach (var node in _nodes)
			{
				node.Value.Cleanup();
			}
			_nodes.Clear();

			var dialog = new BrowseForFolder();
			dialog.Options = 1;
			//dialog.RootFolder = Environment.GetFolderPath(Environment.SpecialFolder.MyComputer);
			dialog.Title = "フォルダを選択して下さい。";
			dialog.ShowDialog();
			if (string.IsNullOrEmpty(dialog.SelectedPath) ||
				!Directory.Exists(dialog.SelectedPath))
			{
				return;
			}

			InputDirectoryPath = dialog.SelectedPath;

			var root = new TreeViewItemViewModel();
			var parent = new DirectoryInfo(InputDirectoryPath);
			root.Item = parent;
			_nodes[root.Item.FullName] = root;
			Tree.Root = CollectionViewSource.GetDefaultView(new [] { root });
			ExportTree.Root = Tree.Root;

			Task.Factory.StartNew(() =>
			{
				foreach (var child in parent.EnumerateFileSystemInfos())
				{
					if ((child.Attributes & (FileAttributes.Hidden | FileAttributes.System)) > 0)
					{
						continue;
					}
					try
					{
						CreateTree(_nodes, child, parent);
					}
					catch (UnauthorizedAccessException)
					{
						_nodes.Remove(child.FullName);
					}
				}
			});
		}

		private void SaveFile()
		{
			var dialog = new SaveFileDialog();
			dialog.Filter = "*.iso|*.iso";
			dialog.AddExtension = true;
			var result = dialog.ShowDialog();
			if (result.HasValue && result.Value)
			{
				OutputFilePath = dialog.FileName;
			}
		}

		private void Execute()
		{
			if (string.IsNullOrEmpty(OutputFilePath))
			{
				return;
			}

			string tempFile = Path.GetTempFileName();
			File.WriteAllLines(
				tempFile,
				_nodes
					.Where(x => !x.Value.IsSelected.HasValue || x.Value.IsSelected.Value)
					.Select(x => x.Value.Item.FullName).OrderBy(x => x)
			);

			var args = new string[] {
				"-l", "\"" + tempFile + "\"",
				"-o", "\"" + OutputFilePath + "\"",
				"-v", "\"" + DateTime.Now.ToString("yyyy-MM-dd HH-mm-ss") + "\""
			};
			var psi = new ProcessStartInfo("IsoCreatorCLI.exe");
			psi.Arguments = string.Join(" ", args);
			psi.CreateNoWindow = false;
			psi.WindowStyle = ProcessWindowStyle.Normal;
			var p = Process.Start(psi);
		}

		private void CreateTree(Dictionary<string, TreeViewItemViewModel> nodes, FileSystemInfo current, FileSystemInfo parent)
		{
			var node = new TreeViewItemViewModel();
			nodes[current.FullName] = node;
			node.Item = current;

			if (parent != null)
			{
				var parentNode = nodes[parent.FullName];
				node.Parent = new WeakReference<TreeViewItemViewModel>(parentNode);

				Action<TreeViewItemViewModel, TreeViewItemViewModel> add = (TreeViewItemViewModel p, TreeViewItemViewModel c) => p.Children.Add(c);
				UIDispatcher.Invoke(add, DispatcherPriority.Normal, parentNode, node);
			}

			if (!(current is DirectoryInfo))
			{
				return;
			}

			foreach (var child in (current as DirectoryInfo).EnumerateFileSystemInfos())
			{
				if ((child.Attributes & (FileAttributes.Hidden | FileAttributes.System)) > 0)
				{
					continue;
				}
				try
				{
					CreateTree(nodes, child, current);
				}
				catch (UnauthorizedAccessException)
				{
					nodes.Remove(child.FullName);
				}
			}
		}
	}
}
