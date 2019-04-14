using System;
using System.Collections.Generic;
using System.DirectoryServices;
using System.Dynamic;
using System.Linq;
using System.Management;
using System.Reflection;
using System.Reflection.Emit;

namespace ConsoleApplication1
{
	class Program
	{
		static void Main(string[] args)
		{
			//var sessions1 =
			//    from session in ExecuteQuery(@"\\.\root\cimv2", "Win32_LogonSession")
			//    let x = session.ToDynamicObject()
			//    where x.LogonType == 2
			//    select x;
			//sessions1.ToList().ForEach(x => Console.WriteLine(x));
		}

		static IEnumerable<ManagementObject> ExecuteQuery(string scope, string className)
		{
			var searcher = new ManagementObjectSearcher(new ManagementScope(scope), new SelectQuery(className));
			return searcher.Get().Cast<ManagementObject>();
		}

		static void Main1(string[] args)
		{
			var result =
				from u1 in GetUsers1()
				join u2 in GetUsers2() on u1.Name equals u2.Name
				select new
				{
					u1 = DumpProperties(u1),
					u2 = DumpProperties(u2),
					end = "-".PadRight(100, '-'),
				};
			result.ToList().ForEach(x => Console.WriteLine(x));
		}

		static string DumpProperties(object obj)
		{
			return
				string.Join("\r\n",
					obj.GetType()
						.GetProperties()
						.Select(x => x.Name + " : " + x.GetValue(obj, null))
						.ToArray()
				) + "\r\n";
		}

		static IEnumerable<dynamic> GetUsers1()
		{
			var container = new DirectoryEntry("WinNT://.");
			container.Children.SchemaFilter.Add("user");
			var result =
				from user in container.Children.OfType<DirectoryEntry>()
				let u = user.ToDynamicObject()
				select new
				{
					u.Name,
					u.AccountDisabled,
					u.BadPasswordAttempts,
					u.IsAccountLocked,
					u.PasswordAge,
					u.PasswordExpired,
				};
			return result;
		}
		static IEnumerable<dynamic> GetUsers2()
		{
			var scope = new ManagementScope(new ManagementPath("\\root\\cimv2"));
			var searcher = new ManagementObjectSearcher(scope, new SelectQuery("Win32_UserAccount"));
			var result =
				from user in searcher.Get().OfType<ManagementBaseObject>()
				let u = user.ToDynamic()
				select new
				{
					u.Name,
					u.Disabled,
					u.Lockout,
					u.PasswordChangeable,
					u.PasswordExpires,
					u.PasswordRequired,
				};
			return result;
		}
	}
}
