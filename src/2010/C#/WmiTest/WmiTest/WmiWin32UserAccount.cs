using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WmiTest
{
	class WmiWin32UserAccount : WmiBase
	{
		public override string ClassName
		{
			get
			{
				return "Win32_UserAccount";
			}
		}
		[WmiProperty]
		public uint AccountType
		{
			get;
			set;
		}
		[WmiProperty]
		public string Caption
		{
			get;
			set;
		}
		[WmiProperty]
		public string Description
		{
			get;
			set;
		}
		[WmiProperty]
		public bool Disabled
		{
			get;
			set;
		}
		[WmiProperty]
		public string Domain
		{
			get;
			set;
		}
		[WmiProperty]
		public string FullName
		{
			get;
			set;
		}
		[WmiProperty]
		public DateTime InstallDate
		{
			get;
			set;
		}
		[WmiProperty]
		public bool LocalAccount
		{
			get;
			set;
		}
		[WmiProperty]
		public bool Lockout
		{
			get;
			set;
		}
		[WmiProperty]
		public string Name
		{
			get;
			set;
		}
		[WmiProperty]
		public bool PasswordChangeable
		{
			get;
			set;
		}
		[WmiProperty]
		public bool PasswordExpires
		{
			get;
			set;
		}
		[WmiProperty]
		public bool PasswordRequired
		{
			get;
			set;
		}
		[WmiProperty]
		public string SID
		{
			get;
			set;
		}
		[WmiProperty]
		public byte SIDType
		{
			get;
			set;
		}
		[WmiProperty]
		public string Status
		{
			get;
			set;
		}
	}
}
