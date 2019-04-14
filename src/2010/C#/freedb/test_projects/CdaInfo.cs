using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace freedb
{
	public class CdaInfo
	{
		public int Track
		{
			get;
			set;
		}
		public int Serial
		{
			get;
			set;
		}
		public int HsgPosition
		{
			get;
			set;
		}
		public int HsgLength
		{
			get;
			set;
		}
		public int FramePosition
		{
			get;
			set;
		}
		public TimeSpan TimePosition
		{
			get;
			set;
		}
		public int FrameLength
		{
			get;
			set;
		}
		public TimeSpan TimeLength
		{
			get;
			set;
		}
	}
}
