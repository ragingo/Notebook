using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;
using System.Collections;

namespace AviDumper
{
	class Program
	{
		static void Main(string[] args)
		{
			const string fileName = @"F:\Downloads\avi_h264_mp3.avi";

			var container = AviContainer.Load(fileName);

			foreach (var item in container)
			{
				if (item is AviListChunk)
				{
					Console.Write("".PadLeft(container.Level, '\t'));
					Console.WriteLine((item as AviListChunk).ListChunk.fcc);
				}
				if (item is AviSubChunk)
				{
					Console.Write("".PadLeft(container.Level, '\t'));
					Console.WriteLine((item as AviSubChunk).Chunk.fcc);
				}
			}

			//byte[] data = File.ReadAllBytes(fileName);

			//unsafe
			//{
			//    int offset;
			//    int length;

			//    offset = 0;
			//    length = Marshal.SizeOf(typeof(RIFFLIST));

			//    byte[] riffListData =
			//        data.Skip(offset)
			//            .Take(length)
			//            .ToArray();

			//    fixed (byte* p = riffListData)
			//    {
			//        var riffChunk = (RIFFLIST*)p;
			//    }

			//    offset += length;
			//    length = Marshal.SizeOf(typeof(RIFFLIST));

			//    byte[] listData =
			//        data.Skip(offset)
			//            .Take(length)
			//            .ToArray();

			//    fixed (byte* p = listData)
			//    {
			//        var riffChunk = (RIFFLIST*)p;
			//    }

			//    offset += length;
			//    length = Marshal.SizeOf(typeof(AVIMAINHEADER));

			//    byte[] mainHeaderData =
			//        data.Skip(offset)
			//            .Take(length)
			//            .ToArray();

			//    fixed (byte* p = mainHeaderData)
			//    {
			//        var mainHeader = (AVIMAINHEADER*)p;
			//    }
			//}
		}
	}

	public sealed class AviContainer : IEnumerable<AviItem>, IDisposable
	{
		private string _fileName;
		private BinaryReader _reader;
		private bool _isDisposed;
		private bool _isHeaderRead;
		public int Level { get; set; }

		private AviContainer()
		{
		}

		~AviContainer()
		{
			if (!_isDisposed)
			{
				_reader.Dispose();
			}
		}

		public static AviContainer Load(string fileName)
		{
			var container = new AviContainer();
			container._fileName = fileName;
			container._reader = new BinaryReader(new FileStream(fileName, FileMode.Open, FileAccess.ReadWrite, FileShare.Read));
			return container;
		}


		public IEnumerator<AviItem> GetEnumerator()
		{
			while (_reader.BaseStream.Position < _reader.BaseStream.Length)
			{
				var fourCC = new FourCC(_reader.ReadBytes(4));

				if (fourCC.ToString() == "RIFF" ||
					fourCC.ToString() == "LIST")
				{
					var item = new AviListChunk();
					item.ListChunk.fcc = fourCC;
					item.ListChunk.cb = _reader.ReadUInt32();
					item.ListChunk.fccListType = new FourCC(_reader.ReadBytes(4));
					yield return item;
				}
				else
				{
					if (!_isHeaderRead)
					{
						_isHeaderRead = true;
						var item = new AviSubChunk();
						item.Chunk.fcc = fourCC;
						item.Chunk.cb = _reader.ReadUInt32();
						item.Data = new AviMainHeader();
						item.Data.fcc = item.Chunk.fcc;
						item.Data.cb = item.Chunk.cb;
						item.Data.dwMicroSecPerFrame = _reader.ReadUInt32();
						item.Data.dwMaxBytesPerSec = _reader.ReadUInt32();
						item.Data.dwPaddingGranularity = _reader.ReadUInt32();
						item.Data.dwFlags = _reader.ReadUInt32();
						item.Data.dwTotalFrames = _reader.ReadUInt32();
						item.Data.dwInitialFrames = _reader.ReadUInt32();
						item.Data.dwStreams = _reader.ReadUInt32();
						item.Data.dwSuggestedBufferSize = _reader.ReadUInt32();
						item.Data.dwWidth = _reader.ReadUInt32();
						item.Data.dwHeight = _reader.ReadUInt32();
						item.Data.dwReserved = _reader.ReadBytes(4 * 4).Select(x => (uint)x).ToArray();
						yield return item;
					}
					else
					{
					}
				}
			}
		}

		IEnumerator IEnumerable.GetEnumerator()
		{
			return this.GetEnumerator();
		}

		public void Dispose()
		{
			_isDisposed = true;
			_reader.Dispose();
			GC.SuppressFinalize(this);
		}
	}

	public class AviItem
	{
	}

	public class AviListChunk : AviItem
	{
		public RiffList ListChunk { get; set; }

		public AviListChunk()
		{
			ListChunk = new RiffList();
		}
	}

	public class AviSubChunk : AviItem
	{
		public RiffChunk Chunk { get; set; }

		public dynamic Data { get; set; }

		public AviSubChunk()
		{
			Chunk = new RiffChunk();
		}
	}

	public class FourCC
	{
		private string _fourCC;
		public FourCC(byte[] bytes)
		{
			_fourCC = Encoding.ASCII.GetString(bytes);
		}

		public override string ToString()
		{
			return _fourCC;
		}
	}

	public class RiffChunk
	{
		public FourCC fcc;
		public uint cb;
	}

	public class RiffList : RiffChunk
	{
		public FourCC fccListType;
	}

	public class AviMainHeader
	{
		public FourCC fcc;
		public uint cb;
		public uint dwMicroSecPerFrame;
		public uint dwMaxBytesPerSec;
		public uint dwPaddingGranularity;
		public uint dwFlags;
		public uint dwTotalFrames;
		public uint dwInitialFrames;
		public uint dwStreams;
		public uint dwSuggestedBufferSize;
		public uint dwWidth;
		public uint dwHeight;
		public uint[] dwReserved;
	}

	[StructLayout(LayoutKind.Explicit)]
	public unsafe struct RIFFCHUNK
	{
		[FieldOffset(0)]
		public fixed byte fcc[4];
		[FieldOffset(4)]
		public uint cb;
	}

	[StructLayout(LayoutKind.Explicit)]
	public unsafe struct RIFFLIST
	{
		[FieldOffset(0)]
		public fixed byte fcc[4];
		[FieldOffset(4)]
		public uint cb;
		[FieldOffset(8)]
		public fixed byte fccListType[4];
	}

	[StructLayout(LayoutKind.Explicit)]
	public unsafe struct AVIMAINHEADER
	{
		[FieldOffset(0)]  public fixed byte fcc[4];
		[FieldOffset(4)]  public uint cb;
		[FieldOffset(8)]  public uint dwMicroSecPerFrame;
		[FieldOffset(12)] public uint dwMaxBytesPerSec;
		[FieldOffset(16)] public uint dwPaddingGranularity;
		[FieldOffset(20)] public uint dwFlags;
		[FieldOffset(24)] public uint dwTotalFrames;
		[FieldOffset(28)] public uint dwInitialFrames;
		[FieldOffset(32)] public uint dwStreams;
		[FieldOffset(36)] public uint dwSuggestedBufferSize;
		[FieldOffset(40)] public uint dwWidth;
		[FieldOffset(44)] public uint dwHeight;
		[FieldOffset(48)] public fixed uint dwReserved[4];
	}
}
