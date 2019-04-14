using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WmiTest
{
	sealed class WmiWin32NTLogEvent : WmiBase
	{
		public override string ClassName
		{
			get
			{
				return "Win32_NTLogEvent";
			}
		}
		[WmiProperty]
		public UInt16 Category
		{
			get;
			set;
		}
		[WmiProperty]
		public string CategoryString
		{
			get;
			set;
		}
		[WmiProperty]
		public string ComputerName
		{
			get;
			set;
		}
		[WmiProperty]
		public byte[] Data
		{
			get;
			set;
		}
		[WmiProperty]
		public UInt16 EventCode
		{
			get;
			set;
		}
		[WmiProperty]
		public UInt32 EventIdentifier
		{
			get;
			set;
		}
		[WmiProperty]
		public byte EventType
		{
			get;
			set;
		}
		[WmiProperty]
		public string[] InsertionStrings
		{
			get;
			set;
		}
		[WmiProperty]
		public string Logfile
		{
			get;
			set;
		}
		[WmiProperty]
		public string Message
		{
			get;
			set;
		}
		[WmiProperty]
		public UInt32 RecordNumber
		{
			get;
			set;
		}
		[WmiProperty]
		public string SourceName
		{
			get;
			set;
		}
		[WmiProperty]
		public DateTime TimeGenerated
		{
			get;
			set;
		}
		[WmiProperty]
		public DateTime TimeWritten
		{
			get;
			set;
		}
		[WmiProperty]
		public string Type
		{
			get;
			set;
		}
		[WmiProperty]
		public string User
		{
			get;
			set;
		}
	}
}
