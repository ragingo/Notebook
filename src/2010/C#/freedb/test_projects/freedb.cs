using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace freedb
{
	public class freedb
	{
		public static CdaInfo ReadCda(byte[] data)
		{
			if (data == null)
			{
				throw new ArgumentNullException();
			}
			if (data.Length != 44)
			{
				throw new ArgumentException();
			}

			CdaInfo ci = new CdaInfo();
			ci.Track  = data[0x17] <<  8 | data[0x16];
			ci.Serial = data[0x21] << 24 | data[0x20] << 16 | data[0x19] <<  8 | data[0x18];
			ci.HsgPosition = data[0x1F] << 24 | data[0x1E] << 16 | data[0x1D] << 8 | data[0x1C];
			ci.HsgLength = data[0x23] << 24 | data[0x22] << 16 | data[0x21] << 8 | data[0x20];
			ci.FramePosition = data[0x24];
			ci.TimePosition = new TimeSpan(data[0x27], data[0x26], data[0x25]);
			ci.TimeLength = new TimeSpan(data[0x2B], data[0x2A], data[0x29]);
			ci.FrameLength = (int)ci.TimeLength.TotalSeconds * 75;
			return ci;
		}

		public static string CalculateDiscId(IEnumerable<CdaInfo> cdaInfos)
		{
			int frames = 0;
			int seconds = 0;
			foreach (var ci in cdaInfos)
			{
				seconds += (int)ci.TimeLength.TotalSeconds;
				int len = ci.FrameLength;
				do
				{
					frames += (len - len / 10 * 10);
					len /= 10;
				} while (len > 0);
			}

			int mod = frames % 255;

			return string.Format("{0:X8}", mod << 24 | seconds << 8 | cdaInfos.Count());
		}
	}
}
