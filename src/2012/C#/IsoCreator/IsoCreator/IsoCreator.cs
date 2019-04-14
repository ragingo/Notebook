using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.ComTypes;
using System.Text;
using System.Text.RegularExpressions;

namespace Sakura.Iso
{
	sealed class IsoCreator : IDisposable
	{
		private static readonly Type IMAPI2FS_MsftFileSystemImage;
		private dynamic _fileSystemImage;
		private bool _isDisposed;
		private SortedDictionary<string, FileSystemInfo> _items;

		public string RootDirectory
		{
			get;
			set;
		}

		public FsiFileSystems FileSystems
		{
			get { return _fileSystemImage.FileSystemsToCreate; }
			set { _fileSystemImage.FileSystemsToCreate = value; }
		}

		public string VolumeName
		{
			get { return _fileSystemImage.VolumeName; }
			set { _fileSystemImage.VolumeName = value; }
		}

		private IMAPI_MEDIA_PHYSICAL_TYPE _mediaType;
		public IMAPI_MEDIA_PHYSICAL_TYPE MediaType
		{
			get
			{
				return _mediaType;
			}
			set
			{
				_mediaType = value;
				_fileSystemImage.ChooseImageDefaultsForMediaType(_mediaType);
			}
		}

		static IsoCreator()
		{
			IMAPI2FS_MsftFileSystemImage = Type.GetTypeFromProgID("IMAPI2FS.MsftFileSystemImage");
		}

		public IsoCreator()
		{
			_isDisposed = false;
			_fileSystemImage = Activator.CreateInstance(IMAPI2FS_MsftFileSystemImage);
			_items = new SortedDictionary<string, FileSystemInfo>();
			FileSystems = FsiFileSystems.FsiFileSystemISO9660 | FsiFileSystems.FsiFileSystemJoliet;
			VolumeName = DateTime.Now.ToString("yyyyMMddHHmmss");
			MediaType = IMAPI_MEDIA_PHYSICAL_TYPE.IMAPI_MEDIA_TYPE_DISK;
		}

		~IsoCreator()
		{
			Dispose();
		}

		public void AddItems(IEnumerable<string> items)
		{
			AddItems(
				items.Select(
					x => Directory.Exists(x) ?
						(FileSystemInfo)new DirectoryInfo(x) :
						(FileSystemInfo)new FileInfo(x)
				)
			);
		}

		public void AddItems(IEnumerable<FileSystemInfo> items)
		{
			foreach (var item in items)
			{
				_items[item.FullName] = item;
			}
		}

		public void AddItem(string item)
		{
			AddItem(
				Directory.Exists(item) ?
					(FileSystemInfo)new DirectoryInfo(item) :
					(FileSystemInfo)new FileInfo(item)
			);
		}

		public void AddItem(FileSystemInfo item)
		{
			_items[item.FullName] = item;
		}

		public void ClearItems()
		{
			_items.Clear();
		}

		private string ConvertItemName(FileSystemInfo item)
		{
			return item.FullName.Replace(RootDirectory, "").Trim('\\');
		}

		private static string CreateImapiErrorMessage(COMException e, string path)
		{
			var msg = new StringBuilder();
			msg.AppendFormat("Error Path : {0}, ", path);
			msg.AppendFormat("Error Code : 0x{0:X}", (uint)e.ErrorCode);
			msg.AppendLine();
			return msg.ToString();
		}

		private void CreateTree()
		{
			foreach (var item in _items)
			{
				string name = ConvertItemName(item.Value);
				if (item.Value is DirectoryInfo)
				{
					try
					{
						_fileSystemImage.Root.AddDirectory(name);
					}
					catch (COMException e)
					{
						Debug.Write(CreateImapiErrorMessage(e, item.Value.FullName));
					}
				}
				else
				{
					try
					{
						IStream stream;
						SHCreateStreamOnFile(item.Value.FullName, STGM.READWRITE, out stream);
						_fileSystemImage.Root.AddFile(name, stream);
					}
					catch (COMException e)
					{
						Debug.Write(CreateImapiErrorMessage(e, item.Value.FullName));
					}
				}
				Console.WriteLine("add : " + item.Value.FullName);
			}
		}

		private void ClearTree()
		{
			foreach(var item in _items)
			{
				string name = ConvertItemName(item.Value);
				try
				{
					_fileSystemImage.Root.Remove(name);
				}
				catch (COMException e)
				{
					Debug.Write(CreateImapiErrorMessage(e, item.Value.FullName));
				}
			}
		}

		public void Save(FileStream outputStream)
		{
			long pt = _fileSystemImage.ChangePoint();

			try
			{
				CreateTree();
			}
			catch(Exception e)
			{
				_fileSystemImage.RollbackToChangePoint(pt);
				Debug.WriteLine(e);
				return;
			}

			IStream imageStream = _fileSystemImage.CreateResultImage().ImageStream;
			using (var cs = new ComStream(imageStream))
			{
				cs.CopyTo(outputStream);
			}
			Marshal.ReleaseComObject(imageStream);
			imageStream = null;

			_fileSystemImage.RollbackToChangePoint(pt);
		}

		public void Dispose()
		{
			GC.SuppressFinalize(this);
			if (!_isDisposed)
			{
				_isDisposed = true;
				if (_fileSystemImage != null)
				{
					Marshal.FinalReleaseComObject(_fileSystemImage);
					_fileSystemImage = null;
				}
			}
		}

		public enum FsiFileSystems
		{
			FsiFileSystemNone = 0,
			FsiFileSystemISO9660 = 1,
			FsiFileSystemJoliet = 2,
			FsiFileSystemUDF = 4,
			FsiFileSystemUnknown = 0x40000000
		}

		public enum IMAPI_MEDIA_PHYSICAL_TYPE
		{
			IMAPI_MEDIA_TYPE_UNKNOWN = 0,
			IMAPI_MEDIA_TYPE_CDROM = 1,
			IMAPI_MEDIA_TYPE_CDR = 2,
			IMAPI_MEDIA_TYPE_CDRW = 3,
			IMAPI_MEDIA_TYPE_DVDROM = 4,
			IMAPI_MEDIA_TYPE_DVDRAM = 5,
			IMAPI_MEDIA_TYPE_DVDPLUSR = 6,
			IMAPI_MEDIA_TYPE_DVDPLUSRW = 7,
			IMAPI_MEDIA_TYPE_DVDPLUSR_DUALLAYER = 8,
			IMAPI_MEDIA_TYPE_DVDDASHR = 9,
			IMAPI_MEDIA_TYPE_DVDDASHRW = 10,
			IMAPI_MEDIA_TYPE_DVDDASHR_DUALLAYER = 11,
			IMAPI_MEDIA_TYPE_DISK = 12,
			IMAPI_MEDIA_TYPE_DVDPLUSRW_DUALLAYER = 13,
			IMAPI_MEDIA_TYPE_HDDVDROM = 14,
			IMAPI_MEDIA_TYPE_HDDVDR = 15,
			IMAPI_MEDIA_TYPE_HDDVDRAM = 16,
			IMAPI_MEDIA_TYPE_BDROM = 17,
			IMAPI_MEDIA_TYPE_BDR = 18,
			IMAPI_MEDIA_TYPE_BDRE = 19,
			IMAPI_MEDIA_TYPE_MAX = 19,
		}

		[DllImport("shlwapi.dll", CharSet = CharSet.Unicode, ExactSpelling = true, PreserveSig = false, EntryPoint = "SHCreateStreamOnFileW")]
		private static extern void SHCreateStreamOnFile(string pszFile, STGM grfMode, out IStream ppstm);

		[Flags]
		private enum STGM : int
		{
			DIRECT = 0x00000000,
			TRANSACTED = 0x00010000,
			SIMPLE = 0x08000000,
			READ = 0x00000000,
			WRITE = 0x00000001,
			READWRITE = 0x00000002,
			SHARE_DENY_NONE = 0x00000040,
			SHARE_DENY_READ = 0x00000030,
			SHARE_DENY_WRITE = 0x00000020,
			SHARE_EXCLUSIVE = 0x00000010,
			PRIORITY = 0x00040000,
			DELETEONRELEASE = 0x04000000,
			NOSCRATCH = 0x00100000,
			CREATE = 0x00001000,
			CONVERT = 0x00020000,
			FAILIFTHERE = 0x00000000,
			NOSNAPSHOT = 0x00200000,
			DIRECT_SWMR = 0x00400000,
		}
	}
}
