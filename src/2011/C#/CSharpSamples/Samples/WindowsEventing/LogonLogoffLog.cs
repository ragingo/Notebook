using System;
using System.Security.Principal;
using System.Text;

namespace Samples.WindowsEventing
{
	class LogonLogoffLog
	{
		public int EventId
		{
			get;
			set;
		}
		public string Computer
		{
			get;
			set;
		}
		public uint LogonType
		{
			get;
			set;
		}
		public DateTime? LogonTime
		{
			get;
			set;
		}
		public DateTime? LogoffTime
		{
			get;
			set;
		}
		public string TargetDomainName
		{
			get;
			set;
		}
		public ulong TargetLogonId
		{
			get;
			set;
		}
		public SecurityIdentifier TargetUserSid
		{
			get;
			set;
		}
		public string TargetUserName
		{
			get;
			set;
		}
		public override string ToString()
		{
			StringBuilder sb = new StringBuilder();
			foreach (var prop in this.GetType().GetProperties())
			{
				sb.AppendFormat("\t\t{0} : {1}\n", prop.Name, prop.GetValue(this, null));
			}
			return sb.ToString();
		}
	}
}
