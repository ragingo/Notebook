using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace Explorer.Model
{
	class FileSystemNode : NodeBase<FileSystemNode, IFileSystemInfo>
	{
		public new string Name
		{
			get
			{
				if (Content == null)
				{
					return string.Empty;
				}
				else if (Content.IsDrive)
				{
					var drive = Content.Get<DriveInfo>();
					string name = drive.Name.TrimEnd('\\');
					if (drive.IsReady)
					{
						return string.Format("{0} ({1})", drive.VolumeLabel, name);
					}
					else
					{
						return string.Format("{0}", name);
					}
				}
				else if (Content.IsDirectory)
				{
					return Content.Get<DirectoryInfo>().Name;
				}
				else if (Content.IsFile)
				{
					return Content.Get<FileInfo>().Name;
				}
				else
				{
					return string.Empty;
				}
			}
		}

		public override string Identity
		{
			get
			{
				return Content.Identity;
			}
		}

		private ImageSource _icon;
		public ImageSource Icon
		{
			get
			{
				if (_icon == null && Content != null)
				{
					var shinfo = new SHFILEINFO();
					var result = SHGetFileInfo(Content.Identity, 0, ref shinfo, (uint)Marshal.SizeOf(shinfo), SHGFI_ICON | SHGFI_SMALLICON | SHGFI_LARGEICON);
					if (result != IntPtr.Zero)
					{
						var bs = Imaging.CreateBitmapSourceFromHIcon(shinfo.hIcon, Int32Rect.Empty, BitmapSizeOptions.FromEmptyOptions());
						_icon = bs;
						DestroyIcon(shinfo.hIcon);
					}
				}
				return _icon;
			}
		}

		protected override ICollection<FileSystemNode> CreateChildren()
		{
			var coll = new ObservableCollection<FileSystemNode>();
			coll.Add(new FileSystemNode());
			return coll;
		}

		public IEnumerable<FileSystemNode> EnumerateAllChildren()
		{
			if (Content.IsDrive || Content.IsDirectory)
			{
				foreach (var child in CacheNodes)
				{
					if (child.Key.StartsWith(Content.Identity) &&
						child.Value.Level != Level)
					{
						yield return child.Value;
					}
				}
			}
		}

		public IEnumerable<FileSystemNode> EnumerateAllParents(FileSystemNode node)
		{
			if (node.Parent != null)
			{
				yield return CacheNodes[node.Parent.Content.Identity];
				foreach (var parent in EnumerateAllParents(node.Parent))
				{
					yield return parent;
				}
			}
		}

		public void ExpandChildren(bool needsReflesh = false)
		{
			if (Content.IsFile)
			{
				return;
			}
			if (Children.Count > 1 && !needsReflesh)
			{
				return;
			}
			DirectoryInfo dir = null;
			if (Content.IsDrive)
			{
				var drive = Content.Get<DriveInfo>();
				if (!drive.IsReady)
				{
					return;
				}
				dir = drive.RootDirectory;
			}
			else if (Content.IsDirectory)
			{
				dir = Content.Get<DirectoryInfo>();
			}
			foreach (var item in dir.EnumerateFileSystemInfos())
			{
				//if ((item.Attributes & (FileAttributes.Hidden | FileAttributes.System)) > 0)
				if ((item.Attributes & FileAttributes.Hidden) > 0)
				{
					continue;
				}
				var child = new FileSystemNode();
				if (item is DirectoryInfo)
				{
					child.Content = new FileSystemInfo<DirectoryInfo>(item as DirectoryInfo);
				}
				else if (item is FileInfo)
				{
					child.Content = new FileSystemInfo<FileInfo>(item as FileInfo);
					child.Children.Clear();
				}
				child.Parent = this;
				AddChild(child);
			}
		}

		private struct SHFILEINFO
		{
			public IntPtr hIcon;
			public IntPtr iIcon;
			public uint dwAttributes;
			[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 260)]
			public string szDisplayName;
			[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 80)]
			public string szTypeName;
		};

		private const uint SHGFI_ICON = 0x100;
		private const uint SHGFI_LARGEICON = 0x0;
		private const uint SHGFI_SMALLICON = 0x1;

		[DllImport("shell32.dll", CharSet = CharSet.Unicode)]
		private static extern IntPtr SHGetFileInfo(string pszPath, uint dwFileAttributes, ref SHFILEINFO psfi, uint cbSizeFileInfo, uint uFlags);
		[DllImport("user32.dll")]
		private extern static bool DestroyIcon(IntPtr hIcon);
	}
}
