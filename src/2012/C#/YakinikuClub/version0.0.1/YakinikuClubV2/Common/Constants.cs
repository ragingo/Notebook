using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Configuration;

namespace YakinikuClubV2.Common
{
	static class Constants
	{
		public static class Model
		{
			public const int PasswordMinLength = 6;
			public const int PasswordMaxLength = 100;
			public static readonly string Watchword = ConfigurationManager.AppSettings["Watchword"];
		}
		public static class ViewModel
		{
		}
	}
}