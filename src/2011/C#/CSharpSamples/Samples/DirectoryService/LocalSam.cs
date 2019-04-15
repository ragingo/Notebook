using System;
using System.Collections;
using System.Collections.Generic;
using System.DirectoryServices;
using System.DirectoryServices.AccountManagement;
using System.Linq;
using System.Text;

namespace Samples.DirectoryService
{
	// http://msdn.microsoft.com/ja-jp/magazine/cc135979.aspx
	class LocalSam
	{
		public static IEnumerable<UserPrincipal> GetUsers(PrincipalContext context)
		{
			using (var searcher = new PrincipalSearcher(new UserPrincipal(context)))
			{
				return searcher.FindAll().Cast<UserPrincipal>();
			}
		}

		public static IEnumerable<GroupPrincipal> GetGroups(PrincipalContext context)
		{
			using (var searcher = new PrincipalSearcher(new GroupPrincipal(context)))
			{
				return searcher.FindAll().Cast<GroupPrincipal>();
			}
		}

		public static string ToString(UserPrincipal principal)
		{
			StringBuilder sb = new StringBuilder();
			sb.Append("[");
			sb.AppendFormat("Name={0}, ", principal.Name);
			sb.AppendFormat("Enabled={0}, ", principal.Enabled);
			sb.AppendFormat("LockedOut={0}, ", principal.IsAccountLockedOut());
			sb.AppendFormat("LastPasswordSet={0}", TimeZone.CurrentTimeZone.ToLocalTime((DateTime)principal.LastPasswordSet));
			sb.Append("]");
			return sb.ToString();
		}

		public static string ToString(GroupPrincipal principal)
		{
			StringBuilder sb = new StringBuilder();
			sb.Append("[");
			sb.AppendFormat("Name={0}", principal.Name);
			sb.Append("]");
			return sb.ToString();
		}

		public static void ShowUsers1()
		{
			foreach (var user in GetUsers(new PrincipalContext(ContextType.Machine)))
			{
				var groups = user.GetGroups().Cast<GroupPrincipal>();
				if (groups.Count() == 0)
				{
					Console.WriteLine("[所属グループ無し]");
				}
				else
				{
					foreach (var group in groups)
					{
						Console.Write(ToString(group) + ", ");
					}
					Console.WriteLine();
				}
				Console.WriteLine("\t" + ToString(user));
			}
		}

		public static void ShowGroups1()
		{
			foreach (var group in GetGroups(new PrincipalContext(ContextType.Machine)))
			{
				Console.WriteLine(ToString(group));
			}
		}

		public static IEnumerable<DirectoryEntry> GetUsers(DirectoryEntry rootEntry)
		{
			rootEntry.Children.SchemaFilter.Add("user");
			return rootEntry.Children.Cast<DirectoryEntry>();
		}

		public static IEnumerable<DirectoryEntry> GetGroups(DirectoryEntry rootEntry)
		{
			rootEntry.Children.SchemaFilter.Add("group");
			return rootEntry.Children.Cast<DirectoryEntry>();
		}

		public static string ToString(DirectoryEntry entry)
		{
			StringBuilder sb = new StringBuilder();
			if (entry.SchemaClassName == "User")
			{
				sb.Append("[");
				sb.AppendFormat("Name={0}, ", entry.Name);
				sb.AppendFormat("Enabled={0}, ", entry.InvokeGet("AccountDisabled"));
				sb.AppendFormat("LockedOut={0}, ", entry.InvokeGet("IsAccountLocked"));
				sb.AppendFormat("LastPasswordSet={0}", 
					DateTime.Now.AddSeconds(-(int)entry.Properties["PasswordAge"].Value)
				);
				sb.Append("]");
			}
			else if (entry.SchemaClassName == "Group")
			{
				sb.AppendFormat("[Name={0}]", entry.Name);
			}
			return sb.ToString();
		}

		public static void ShowUsers2()
		{
			using (DirectoryEntry entry = new DirectoryEntry("WinNT://."))
			{
				foreach (var user in GetUsers(entry))
				{
					var groups =
						(user.Invoke("Groups") as IEnumerable)
							.Cast<object>()
							.Select(x => new DirectoryEntry(x));
					if (groups.Count() == 0)
					{
						Console.WriteLine("[所属グループ無し]");
					}
					else
					{
						foreach (var group in groups)
						{
							Console.Write(ToString(group) + ", ");
						}
						Console.WriteLine();
					}
					Console.WriteLine("\t" + ToString(user));
				}
			}
		}

		public static void ShowGroups2()
		{
			using (DirectoryEntry entry = new DirectoryEntry("WinNT://."))
			{
				foreach (var group in GetGroups(entry))
				{
					Console.WriteLine(ToString(group));
				}
			}
		}
	}
}
