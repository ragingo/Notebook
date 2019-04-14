using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.DirectoryServices;

namespace WmiTest
{
	class ADUserInfo
	{
		public int UserFlags
		{
			get;
			set;
		}
		public int MaxStorage
		{
			get;
			set;
		}
		public int PasswordAge
		{
			get;
			set;
		}
		public int PasswordExpired
		{
			get;
			set;
		}
		public byte[] LoginHours
		{
			get;
			set;
		}
		public string FullName
		{
			get;
			set;
		}
		public string Description
		{
			get;
			set;
		}
		public int BadPasswordAttempts
		{
			get;
			set;
		}
		public DateTime LastLogin
		{
			get;
			set;
		}
		public string HomeDirectory
		{
			get;
			set;
		}
		public string LoginScript
		{
			get;
			set;
		}
		public string Profile
		{
			get;
			set;
		}
		public string HomeDirDrive
		{
			get;
			set;
		}
		public string Parameters
		{
			get;
			set;
		}
		public int PrimaryGroupID
		{
			get;
			set;
		}
		public string Name
		{
			get;
			set;
		}
		public int MinPasswordLength
		{
			get;
			set;
		}
		public int MaxPasswordAge
		{
			get;
			set;
		}
		public int MinPasswordAge
		{
			get;
			set;
		}
		public int PasswordHistoryLength
		{
			get;
			set;
		}
		public int AutoUnlockInterval
		{
			get;
			set;
		}
		public int LockoutObservationInterval
		{
			get;
			set;
		}
		public int MaxBadPasswordsAllowed
		{
			get;
			set;
		}
		public byte[] objectSid
		{
			get;
			set;
		}

		public ADUserInfo(DirectoryEntry entry)
		{
			foreach (var pi in this.GetType().GetProperties())
			{
				var pvc = entry.Properties[pi.Name];
				if (pvc != null && pvc.Count > 0)
				{
					pi.SetValue(this, pvc.Value, null);
				}
			}
		}

		public static IEnumerable<ADUserInfo> GetList(string path)
		{
			var de = new DirectoryEntry(path);
			foreach (DirectoryEntry child in de.Children)
			{
				if (child.SchemaClassName == "User")
				{
					yield return new ADUserInfo(child);
				}
			}
		}
	}
}
