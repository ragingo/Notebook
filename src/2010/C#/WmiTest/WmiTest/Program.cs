using System;
using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.Diagnostics;
using System.Diagnostics.Eventing.Reader;
using System.DirectoryServices;
using System.IO;
using System.Linq;
using System.Linq.Expressions;
using System.Management;
using System.Net;
using System.Reflection;
using System.Text;
using System.Text.RegularExpressions;
using System.Xml.Linq;
using Microsoft.CSharp;

namespace WmiTest
{
	//class WmiDynamicObject : DynamicObject
	//{
	//    private ManagementBaseObject _instance;
	//    public WmiDynamicObject(ManagementBaseObject instance)
	//    {
	//        _instance = instance;
	//    }
	//    public override bool TryGetMember(GetMemberBinder binder, out object result)
	//    {
	//        result = _instance.Properties[binder.Name].Value;
	//        return true;
	//    }
	//    public override IEnumerable<string> GetDynamicMemberNames()
	//    {
	//        return _instance.Properties.OfType<PropertyData>().Select(x => x.Name);
	//    }
	//}

	class Program
	{
		static void Main(string[] args)
		{
			//ShowUserList();
			//ShowLoginLogoutList1();
			//ShowLoginLogoutList2();
		}

		//static void ShowLoginLogoutList1()
		//{
		//    var scope = new ManagementScope(new ManagementPath(@"\\.\root\cimv2"));
		//    var searcher = new ManagementObjectSearcher(scope, new SelectQuery("Win32_NTLogEvent"));

		//    var result =
		//        from ManagementBaseObject obj in searcher.Get()
		//        let inst = WmiBase.CreateInstance<WmiWin32NTLogEvent>(obj)
		//        where inst.EventCode == 4624 || inst.EventCode == 4634
		//        select new
		//        {
		//            inst.ComputerName,
		//            User = "",
		//            inst.EventCode,
		//            inst.TimeWritten,
		//        };

		//    foreach (var i in result)
		//    {
		//        Console.WriteLine(i);
		//    }
		//}

		//static void ShowLoginLogoutList2()
		//{
		//    var result =
		//        from log in EventLog.GetEventLogs()
		//        from EventLogEntry ent in log.Entries
		//        let eventId = ent.InstanceId & 0x3fffffff
		//        where eventId == 4624 || eventId == 4634
		//        select new
		//        {
		//            User = ent.UserName,
		//            Host = ent.MachineName,
		//        };

		//    foreach (var i in result)
		//    {
		//        Console.WriteLine(i);
		//    }
		//}

		static void ShowUserList()
		{
			var scope = new ManagementScope(new ManagementPath(@"\\.\root\cimv2"));
			var searcher = new ManagementObjectSearcher(scope, new SelectQuery("Win32_UserAccount"));

			var result =
				from ManagementBaseObject obj in searcher.Get()
				let inst = WmiBase.CreateInstance<WmiWin32UserAccount>(obj)
				join user in ADUserInfo.GetList("WinNT://localhost") on inst.Name equals user.Name
				select new
				{
					UserName = inst.Name,
					PasswordLastChanged = DateTime.Now.AddSeconds(-user.PasswordAge),
					Lockout = inst.Lockout,
					PasswordExpires = inst.PasswordExpires
				};

			foreach (var i in result)
			{
				Console.WriteLine(i);
			}
		}
	}
}
