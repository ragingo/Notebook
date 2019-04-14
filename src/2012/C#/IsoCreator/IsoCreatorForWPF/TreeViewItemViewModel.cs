using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Windows;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Runtime.InteropServices;
using System;

namespace IsoCreatorForWPF
{
	public class TreeViewItemViewModel : ViewModelBase
	{
		public WeakReference<TreeViewItemViewModel> Parent
		{
			get;
			set;
		}

		public ICollection<TreeViewItemViewModel> Children
		{
			get;
			set;
		}

		private bool? _isSelected;
		public bool? IsSelected
		{
			get { return _isSelected; }
			set
			{
				Util.RaisePropertyChanged(this, nameof(IsSelected), ref _isSelected, ref value);
			}
		}

		private FileSystemInfo _item;
		public FileSystemInfo Item
		{
			get { return _item; }
			set
			{
				Util.RaisePropertyChanged(this, nameof(Item), ref _item, ref value);
				if (IsDirectory)
				{
					Children = new ObservableCollection<TreeViewItemViewModel>();
				}
				RaisePropertyChanged(nameof(IsDirectory));
			}
		}

		public bool IsDirectory
		{
			get
			{
				return Item is DirectoryInfo;
			}
		}

		private static Dictionary<Int64, ImageSource> _iconCache = new Dictionary<Int64, ImageSource>();

		private ImageSource _icon;
		public ImageSource Icon
		{
			get
			{
				if (_icon == null && Item != null)
				{
					var shinfo = new SHFILEINFO();
					var result = SHGetFileInfo(Item.FullName, 0, ref shinfo, (uint)Marshal.SizeOf(shinfo), SHGFI_ICON | SHGFI_SMALLICON | SHGFI_LARGEICON);
					if (result != IntPtr.Zero)
					{
						var bs = Imaging.CreateBitmapSourceFromHIcon(shinfo.hIcon, Int32Rect.Empty, BitmapSizeOptions.FromEmptyOptions());
						if (!_iconCache.ContainsKey(shinfo.iIcon.ToInt64()))
						{
							_iconCache[shinfo.iIcon.ToInt64()] = bs;
							bs.Freeze();
						}
						_icon = _iconCache[shinfo.iIcon.ToInt64()];
						DestroyIcon(shinfo.hIcon);
						RaisePropertyChanged(nameof(Icon));
					}
				}
				return _icon;
			}
		}

		public int Level
		{
			get
			{
				if (Parent == null)
				{
					return 0;
				}
				TreeViewItemViewModel item;
				if (Parent.TryGetTarget(out item))
				{
					return item.Level + 1;
				}
				return 0;
			}
		}

		public TreeViewItemViewModel()
		{
			IsSelected = false;
		}

		public override void Cleanup()
		{
			base.Cleanup();
			_iconCache.Clear();
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

		[DllImport("shell32.dll")]
		private static extern IntPtr SHGetFileInfo(string pszPath, uint dwFileAttributes, ref SHFILEINFO psfi, uint cbSizeFileInfo, uint uFlags);
		[DllImport("user32.dll")]
		private extern static bool DestroyIcon(IntPtr handle);
	}
}
