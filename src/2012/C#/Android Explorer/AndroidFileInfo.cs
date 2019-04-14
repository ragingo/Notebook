using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Threading;
using System.Windows.Data;

namespace Android_Explorer
{
	class AndroidFileInfo : IEquatable<AndroidFileInfo>
	{
		private static Adb adb = new Adb();
		public AndroidFileInfo Root { get; set; }
		public AndroidFileInfo Parent { get; set; }
		public string FileName { get; set; }
		public string Path { get; set; }
		public bool IsDirectory { get; set; }
		public int Length { get; set; }
		public string LastUpdate { get; set; }
		public ObservableCollection<AndroidFileInfo> Children { get; set; }
		private static Dictionary<string, AndroidFileInfo> allCaches = new Dictionary<string, AndroidFileInfo>();
		private Dictionary<string, AndroidFileInfo> caches = new Dictionary<string, AndroidFileInfo>();

		public AndroidFileInfo()
		{
			DispatcherHelper.UIDispatcher.Invoke(
				(Action)(() =>
				{
					Children = new ObservableCollection<AndroidFileInfo>();
				})
			);
		}

		public static void Traversal(string path)
		{
			string[] items = path.Split('/');

		}

		private IEnumerable<AndroidFileInfo> ParseLs(IEnumerable<string> lines)
		{
			foreach (string line in lines)
			{
				string[] columns = Regex.Replace(line, "[ ]+", " ").Split(' ');
				AndroidFileInfo info = new AndroidFileInfo();
				info.Parent = this;
				char type = columns[0][0];

				switch (type)
				{
					case '-':
						info.Length = int.Parse(columns[3]);
						info.LastUpdate = columns[4] + " " + columns[5];
						info.FileName = string.Join(" ", columns.Skip(6));
						break;
					case 'd':
						info.IsDirectory = true;
						info.LastUpdate = columns[3] + " " + columns[4];
						info.FileName = string.Join(" ", columns.Skip(5));
						break;
					default:
						continue;
				}

				if (Parent == null)
				{
					info.Path = "/" + info.FileName;
					info.Root = this;
				}
				else
				{
					info.Path = Path + "/" + info.FileName;
					info.Root = Root;
				}
				if (!caches.ContainsKey(info.Path))
				{
					caches.Add(info.Path, info);
					allCaches.Add(info.Path, info);
					yield return info;
				}
				else
				{
					continue;
				}
			}
		}

		private IEnumerable<AndroidFileInfo> GetList(string device)
		{
			var lines = adb.InvokeShell(device, "ls -l \"" + Path + "\"");
			return ParseLs(lines);
		}

		public void LoadChildren(string device)
		{
			foreach (var info in GetList(device).OrderBy(x => x.Path))
			{
				DispatcherHelper.UIDispatcher.BeginInvoke(
					(Action<AndroidFileInfo>)Children.Add,
					DispatcherPriority.Input,
					info
				);
			}
		}

		public void ClearChildren()
		{
			ClearChildrenTree(this);
		}

		private void ClearChildrenTree(AndroidFileInfo node)
		{
			foreach (AndroidFileInfo child in node.Children)
			{
				ClearChildrenTree(child);
				child.Parent = null;
				child.Root = null;
				child.ClearChildrenTree(child);

				allCaches.Remove(child.Path);
				child.caches.Clear();
			}
			node.caches.Clear();
			node.Children.Clear();
		}

		public void ReloadChildren(string device)
		{
			ClearChildren();
			Task.Factory.StartNew(() =>
			{
				LoadChildren(device);
			});
		}

		public void Remove()
		{
			adb.InvokeShell("rm", Path);
			this.Parent.Children.Remove(this);
		}

		public void Download(string device)
		{
			adb.Download(device, Path, AndroidSettings.DefaultTemporaryDirectory.FullName);
		}

		public bool Equals(AndroidFileInfo other)
		{
			return Path == other.Path;
		}
	}
}
