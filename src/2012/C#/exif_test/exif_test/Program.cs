using System;
using System.Linq;
using System.IO;
using System.Collections.Generic;
using System.Diagnostics;

namespace exif_test
{
	class TagInfo
	{
		public ushort Tag { get; set; }
		public ushort Type { get; set; }
		public uint Count { get; set; }
		public uint ValueOrOffset { get; set; }
		public dynamic Value { get; set; }
		public override string ToString()
		{
			return string.Format("tag:{0}(0x{0:X2}), type:{1}, count:{2}, value or offset:{3}, value:{4}", Tag, Type, Count, ValueOrOffset, Value);
		}
	}
	class Program
	{
		static dynamic Convert(byte[] data, TypeCode type, bool isLittleEndian = true)
		{
			dynamic val;
			switch (type)
			{
			case TypeCode.String:
				var tmp = data.Where(x => char.IsLetterOrDigit((char)x));
				if (!isLittleEndian)
				{
					tmp = tmp.Reverse();
				}
				val = string.Join("", tmp.Select(x => ((char)x).ToString()));
				break;
			case TypeCode.UInt16:
				val = BitConverter.ToUInt16(isLittleEndian ? data : data.Reverse().ToArray(), 0);
				break;
			case TypeCode.UInt32:
				val = BitConverter.ToUInt32(isLittleEndian ? data : data.Reverse().ToArray(), 0);
				break;
			case TypeCode.UInt64:
				val = BitConverter.ToUInt64(isLittleEndian ? data : data.Reverse().ToArray(), 0);
				break;
			default:
				val = BitConverter.ToString(data);
				break;
			}
			return val;
		}
		static dynamic ReadData(BinaryReader br, int byteCount, TypeCode type, bool isLittleEndian = true)
		{
			byte[] data = br.ReadBytes(byteCount);
			return Convert(data, type, isLittleEndian);
		}
		const int GPS_IFD_POINTER = 34853;
		static void Main(string[] args)
		{
			string path = @"DSC_0003.JPG";
			using (var fs = File.OpenRead(path))
			using (var br = new BinaryReader(fs))
			{
				string soi = ReadData(br, 2, TypeCode.Empty);
				string marker = ReadData(br, 2, TypeCode.Empty);
				int size = ReadData(br, 2, TypeCode.UInt16);
				string exifCode = ReadData(br, 6, TypeCode.String);

				string order = ReadData(br, 2, TypeCode.String);
				bool isLE = order == "II";
				string tiffCode = ReadData(br, 2, TypeCode.Empty, isLE);
				uint ifdOffset = ReadData(br, 4, TypeCode.UInt32, isLE); // 6
				int entryNumber = ReadData(br, 2, TypeCode.UInt16, isLE);

				long headerSize = 12;//fs.Position;
				Console.WriteLine("headerSize:{0}", headerSize);

				Console.WriteLine("soi:{0}", soi);
				Console.WriteLine("marker:{0}", marker);
				Console.WriteLine("size:{0}", size);
				Console.WriteLine("exifCode:{0}", exifCode);
				Console.WriteLine("order:{0}", order);
				Console.WriteLine("isLE:{0}", isLE);
				Console.WriteLine("tiffCode:{0}", tiffCode);
				Console.WriteLine("ifdOffset:{0}", ifdOffset);
				Console.WriteLine("entryNumber:{0}", entryNumber);

				var tags = new Dictionary<int, TagInfo>();
				for (int i = 0; i < entryNumber; i++)
				{
					var tag = ReadTag(br, false);
					tags[tag.Tag] = tag;
					Console.WriteLine(tag);
				}
				if(tags.ContainsKey(GPS_IFD_POINTER))
				{
					Console.WriteLine("---gps----------");
					var tag = tags[GPS_IFD_POINTER];
					//fs.Seek(headerSize + tag.ValueOrOffset - 2, SeekOrigin.Begin); // 2bytesずれてる？
					//fs.Seek(tag.ValueOrOffset, SeekOrigin.Begin); //先頭からのoffset?
					fs.Seek(headerSize + tag.ValueOrOffset, SeekOrigin.Begin);

					int gpsEntryNumber = ReadData(br, 2, TypeCode.UInt16);
					Console.WriteLine(gpsEntryNumber);

					for (int i = 0; i < gpsEntryNumber; i++)
					{
						Console.WriteLine(ReadTag(br, false));
					}
				}
			}
		}
		private static TagInfo ReadTag(BinaryReader br, bool isLittleEndian = true, TypeCode valueType = TypeCode.String)
		{
			TagInfo tag = new TagInfo();
			tag.Tag = ReadData(br, 2, TypeCode.UInt16, isLittleEndian);
			tag.Type = ReadData(br, 2, TypeCode.UInt16, isLittleEndian);
			tag.Count = ReadData(br, 4, TypeCode.UInt32, isLittleEndian);

			byte[] val = br.ReadBytes(4);
			tag.ValueOrOffset = Convert(val, TypeCode.UInt32, isLittleEndian);
			tag.Value = Convert(val, valueType, isLittleEndian);
			return tag;
		}
	}
}

// http://diary.awm.jp/~yoya/data/2010/05/11/jpegdump3_gps.phps
// http://blogs.yahoo.co.jp/airmikan/15295063.html
// http://www2.airnet.ne.jp/~kenshi/exif.html