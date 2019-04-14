using System;
using System.Collections.Generic;
using System.DirectoryServices;

/*
http://uchukamen.com/Programming1/ADSI/index.htm
DirectoryEntry の Path の例
ドメインに参加している場合
	WinNT://MyDomain/YourComputerName
	WinNT://MyDomain/Group 
	WinNT://MyDomain/MyComputer/aPrinter/
ワークグループの場合
	WinNT://YourComputerName
LDAPの場合
	LDAP://MyDomain.microsoft.com/CN=TopH,DC=DEV,DC=MSFT,DC=COM,O=Internet
	LDAP://CN=TopHat,DC=DEV,DC=MSFT,DC=COM,O=Internet
NDSの場合
	NDS://ndsServer/O=Internet/DC=COM/DC=MSFT/DC=DEV/CN=TopHat
	NWCOMPAT://binderyServer/TopHat
*/

namespace UserInfoTest
{
	class Program
	{
		public static void Main(string[] args)
		{
			string path = "WinNT://localhost";
			foreach (var ui in ADUserInfo.GetList(path))
			{
				Console.WriteLine(
					"ユーザ名：{0, -20} 最終パスワード変更日時：{1:yyyy/MM/dd HH:mm:ss}",
					ui.Name,
					DateTime.Now.AddSeconds(-ui.PasswordAge));
			}
		}
	}

	class ADUserInfo
	{
		public int UserFlags{ get; set; }
		public int MaxStorage{ get; set; }
		public int PasswordAge{ get; set; }
		public int PasswordExpired{ get; set; }
		public byte[] LoginHours{ get; set; }
		public string FullName{ get; set; }
		public string Description{ get; set; }
		public int BadPasswordAttempts{ get; set; }
		public DateTime LastLogin{ get; set; }
		public string HomeDirectory{ get; set; }
		public string LoginScript{ get; set; }
		public string Profile{ get; set; }
		public string HomeDirDrive{ get; set; }
		public string Parameters{ get; set; }
		public int PrimaryGroupID{ get; set; }
		public string Name{ get; set; }
		public int MinPasswordLength{ get; set; }
		public int MaxPasswordAge{ get; set; }
		public int MinPasswordAge{ get; set; }
		public int PasswordHistoryLength{ get; set; }
		public int AutoUnlockInterval{ get; set; }
		public int LockoutObservationInterval{ get; set; }
		public int MaxBadPasswordsAllowed{ get; set; }
		public byte[] objectSid{ get; set; }

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