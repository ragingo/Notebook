using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace Explorer.Model
{
	class FileSystemTree
	{
		public IEnumerable<FileSystemNode> Drives
		{
			get;
			set;
		}

		public FileSystemTree()
		{
			Load();
		}

		private void Load()
		{
			Drives =
				from drive in DriveInfo.GetDrives()
				select new FileSystemNode
				{
					Content = new FileSystemInfo<DriveInfo>(drive),
				};
		}
	}
}
