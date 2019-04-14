using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WmiTest
{
	public static class Constants
	{
		public const string EVENTLOG_NAMESPACE = "http://schemas.microsoft.com/win/2004/08/events/event";
		public const int EVENT_SECURITY_LOGON = 4624;
		public const int EVENT_SECURITY_LOGOFF = 4634;
	}
}
