using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Security.AccessControl;
using System.Security.Principal;
using System.Text;
using System.Text.RegularExpressions;

namespace Samples.Security
{
	class Security
	{
		public static void TestGetPermissionValue()
		{
			string[] permissions = new[] {
				"-rwxrwxrwx",
				"-r-xrw---x",
				"-rwSrwsrwT",
				"-rwsrwSrwt"
			};
			foreach (string p in permissions)
			{
				Console.WriteLine(GetPermissionValue(p));
			}
		}
		public static int GetPermissionValue(string permission)
		{
			const int LEN = 10;
			if (permission.Length != LEN)
			{
				throw new ArgumentException();
			}
			if (!Regex.IsMatch(permission, "^[-d]([-r][-w][-xSs]){2}[-r][-w][-xTt]$"))
			{
				throw new ArgumentException();
			}

			int[] nums = new int[4];
			for (int i = 1; i < LEN; i++)
			{
				char c = permission[i];
				int n = -((i + 2) % 3 - 3);
				int idx = (i - 1) / 3 + 1;

				if (c == '-' || c == 'S' || c == 'T')
				{
					nums[idx] = 0;
				}
				else
				{
					nums[idx] = (int)Math.Pow(2.0, n);
				}
				if (char.ToUpper(c) == 'S' || char.ToUpper(c) == 'T')
				{
					nums[0] += (int)Math.Pow(2.0, n);
				}
			}
			StringBuilder result = new StringBuilder();
			foreach (int i in nums)
			{
				result.Append(i);
			}
			return int.Parse(result.ToString());
		}

		private static void SetAclTest()
		{
			string path = "c:\\a.txt";
			FileSecurity fs = File.GetAccessControl(path, AccessControlSections.Access);
			NTAccount account = new NTAccount("user1");
			fs.SetAccessRule(
				new FileSystemAccessRule(
					account.Translate(typeof(SecurityIdentifier)),
					FileSystemRights.FullControl,
					InheritanceFlags.None,
					PropagationFlags.NoPropagateInherit,
					AccessControlType.Allow
				)
			);
			File.SetAccessControl(path, fs);
		}
	}
}
