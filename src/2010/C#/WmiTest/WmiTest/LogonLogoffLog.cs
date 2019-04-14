using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WmiTest
{
	class LogonLogoffLog
	{
		public int EventId
		{
			get;
			set;
		}
		public string TargetDomainName
		{
			get;
			set;
		}
		public string Computer
		{
			get;
			set;
		}
		public string TargetUserSid
		{
			get;
			set;
		}
		public string TargetUserName
		{
			get;
			set;
		}
		public ulong TargetLogonId
		{
			get;
			set;
		}
		public int LogonType
		{
			get;
			set;
		}
		public DateTime? LogonTime
		{
			get;
			set;
		}
		public DateTime? LogoffTime
		{
			get;
			set;
		}
		public string Status
		{
			get;
			set;
		}
	}
}
