using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Samples.WindowsEventing
{
	static class EventType
	{
		public enum Security
		{
			Logon = 4624,
			Logoff = 4634,
		}
		public enum Application
		{
		}
	}
}
