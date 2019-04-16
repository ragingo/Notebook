using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Diagnostics;

namespace ConsoleApplication1
{
	struct TagInfo
	{
		public ushort Tag;
		public ushort Type;
		public uint Count;
		public uint Offset;
		public uint TotalOffset;
		public override string ToString()
		{
			return string.Format("tag:{0}(0x{0:X2}), type:{1}, count:{2}, offset:{3}, total offset:{4}", Tag, Type, Count, Offset, TotalOffset);
		}
	}
	class Program
	{
		const int GPS_IFD_POINTER = 34853;
		static void Main(string[] args)
		{
			string filename = @"D:\geotag\DSCF0053.JPG";
			using (var fs = File.OpenRead(filename))
			using (var br = new BinaryReader(fs))
			{
				byte[] app1 = br.ReadBytes(2);
				byte[] seg_len = br.ReadBytes(4);
				byte[] exif_code = br.ReadBytes(6);
				byte[] tiff_header_order = br.ReadBytes(2);
				byte[] tiff_header_ifd_offset = br.ReadBytes(6);
				byte[] entry_number = br.ReadBytes(2);

				string order = tiff_header_order.ToString2();
				bool isLE = order == "II";
				uint ifd_offset = BitConverter.ToUInt32(tiff_header_ifd_offset, 0);
				int entryNum = BitConverter.ToInt16(entry_number, 0);

				Dump(app1);
				Dump(seg_len);
				Dump(exif_code, exif_code.ToString2());
				Dump(tiff_header_order, order);
				Dump(tiff_header_ifd_offset, "0x"+ifd_offset.ToString("X"));
				Dump(entry_number, entryNum);

				TagInfo[] entries = new TagInfo[entryNum];
				for (int i = 0; i < entryNum; i++)
				{
					ReadTag(br, entries, i);
					Console.WriteLine(entries[i]);

					if (entries[i].Tag == GPS_IFD_POINTER)
					{
						// �߂�
						fs.Seek(178, SeekOrigin.Begin);

						Console.WriteLine("---- gps ifd begin ----");
						br.ReadBytes((int)entries[i].Offset);
						byte[] gps_ifd_number = br.ReadBytes(2);
						int gpsTagNum = BitConverter.ToUInt16(gps_ifd_number, 0);
						Dump(gps_ifd_number, gpsTagNum);

						TagInfo[] gpsEntries = new TagInfo[gpsTagNum];
						for (int j = 0; j < gpsTagNum; j++)
						{
							ReadTag(br, gpsEntries, j);
							Console.WriteLine(gpsEntries[j]);
							Debug.Assert(gpsEntries[j].Type > 0 && gpsEntries[j].Type < 9, "tag type ��������");
						}

						Console.WriteLine("---- gps ifd end   ----");
						break;
					}
				}
			}
		}

		private static void ReadTag(BinaryReader br, TagInfo[] entries, int entryNo)
		{
			byte[] ent = br.ReadBytes(12);
			int pos = 0;
			entries[entryNo].Tag = BitConverter.ToUInt16(ent.Skip(pos).Take(2).ToArray(), 0);
			pos += 2;
			entries[entryNo].Type = BitConverter.ToUInt16(ent.Skip(pos).Take(2).ToArray(), 0);
			pos += 2;
			entries[entryNo].Count = BitConverter.ToUInt16(ent.Skip(pos).Take(4).ToArray(), 0);
			pos += 4;
			entries[entryNo].Offset = BitConverter.ToUInt16(ent.Skip(pos).Take(4).ToArray(), 0);
			if (entryNo > 0)
			{
				entries[entryNo].TotalOffset = entries[entryNo - 1].TotalOffset + entries[entryNo].Offset;
			}
			else
			{
				entries[entryNo].TotalOffset = entries[entryNo].Offset;
			}
		}
		static void Dump(IEnumerable<byte> data, params object[] args)
		{
			string s = string.Join(" ", data.Select(x=>x.ToString("X2")));
			Console.WriteLine("{0} ({1})", s, string.Join(", ", args));
		}
	}
	static class Util
	{
		public static string ToString2(this IEnumerable<byte> data)
		{
			return data.Where(x => char.IsLetterOrDigit((char)x)).Select(x => ((char)x).ToString()).Aggregate((x, y) => x + y);
		}
	}
}
