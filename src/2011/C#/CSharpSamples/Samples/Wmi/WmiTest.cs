using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Management;

namespace Samples.Wmi
{
	class WmiTest
	{
		public static void BackupEventLog()
		{
			const string EXPORT_FILE_NAME_FORMAT = @"d:\logs\{0}_{1:yyyyMMddHHmmss}.evtx";
			var targets = new string[] {
				"Application",
				"Security"
			};
			var scope = new ManagementScope(new ManagementPath(@"\\.\root\cimv2"));
			scope.Options.EnablePrivileges = true;
			var searcher = new ManagementObjectSearcher(scope, new SelectQuery("Win32_NTEventLogFile"));
			var logs =
				from ManagementObject log in searcher.Get()
				join target in targets on log["LogFilename"].ToString() equals target
				select log;

			foreach (var log in logs)
			{
				log.InvokeMethod("BackupEventLog",
					new[] { 
						string.Format(EXPORT_FILE_NAME_FORMAT, log["LogFilename"], DateTime.Now)
					}
				);
			}
		}
	}
}
