using System;
using System.Collections.Concurrent;
using System.IO;
using System.Threading.Tasks;

namespace Explorer.Model
{
	class FileSystemInfo<T> : IFileSystemInfo
		where T : class
	{
		private static readonly ConcurrentBag<DriveInfo> Drives;
		private T _item;

		public bool IsDrive
		{
			get
			{
				return typeof(T) == typeof(DriveInfo);
			}
		}

		public bool IsDirectory
		{
			get
			{
				return typeof(T) == typeof(DirectoryInfo);
			}
		}

		public bool IsFile
		{
			get
			{
				return typeof(T) == typeof(FileInfo);
			}
		}

		public string Identity
		{
			get
			{
				if (IsDrive)
				{
					return Get<DriveInfo>().Name;
				}
				else if (IsDirectory)
				{
					return Get<DirectoryInfo>().FullName;
				}
				else if(IsFile)
				{
					return Get<FileInfo>().FullName;
				}
				return Guid.NewGuid().ToString();
			}
		}

		static FileSystemInfo()
		{
			Drives = new ConcurrentBag<DriveInfo>();
			Task.Factory.StartNew(() =>
			{
				foreach (var d in DriveInfo.GetDrives())
				{
					Drives.Add(d);
				}
			});
		}

		public TResult Get<TResult>()
			where TResult : class
		{
			return _item as TResult;
		}

		public void Set(T item)
		{
			if (typeof(T) == typeof(DriveInfo) ||
				typeof(T) == typeof(DirectoryInfo) ||
				typeof(T) == typeof(FileInfo))
			{
				_item = item;
			}
			else
			{
				throw new Exception();
			}
		}

		public FileSystemInfo(T item)
		{
			Set(item);
		}
	}
}
