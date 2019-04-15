using System;
using System.Collections.Generic;
using System.Diagnostics.Eventing.Reader;
using System.IO;
using System.Security.Principal;

namespace Samples.WindowsEventing
{
	class WindowsEventingTest
	{
		public static IEnumerable<LogonLogoffLog> GetLogonLogoffList()
		{
			string[] xPathRefs = new string[] {
				"Event/System/TimeCreated/@SystemTime",
				"Event/System/Computer",
				"Event/EventData/Data[@Name='LogonType']",
				"Event/EventData/Data[@Name='TargetDomainName']",
				"Event/EventData/Data[@Name='TargetLogonId']",
				"Event/EventData/Data[@Name='TargetUserName']",
				"Event/EventData/Data[@Name='TargetUserSid']",
			};
			var query = new EventLogQuery("Security", PathType.LogName, File.ReadAllText(@"WindowsEventing\QueryList.xml"));
			using (var reader = new EventLogReader(query))
			{
				for (var evt = reader.ReadEvent(); evt != null; evt = reader.ReadEvent())
				{
					using (evt)
					{
						var props = (evt as EventLogRecord).GetPropertyValues(new EventLogPropertySelector(xPathRefs));

						yield return
							new LogonLogoffLog
							{
								Computer = (string)props[1],
								EventId = evt.Id,
								LogoffTime = evt.Id == (int)EventType.Security.Logoff ? (DateTime?)props[0] : null,
								LogonTime = evt.Id == (int)EventType.Security.Logon ? (DateTime?)props[0] : null,
								LogonType = (uint)props[2],
								TargetDomainName = (string)props[3],
								TargetLogonId = (ulong)props[4],
								TargetUserName = (string)props[5],
								TargetUserSid = (SecurityIdentifier)props[6]
							};
					}
				}
			}
		}
	}
}
