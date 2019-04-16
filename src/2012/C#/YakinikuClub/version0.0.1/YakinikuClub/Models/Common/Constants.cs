using System.Configuration;

namespace YakinikuClub.Models.Common
{
	internal static class Constants
	{
		internal static class Database
		{
			public static readonly string DefaultSchema;
			public static readonly string EmptyShopKey;
			static Database()
			{
				DefaultSchema = ConfigurationManager.AppSettings["DatabaseDefaultSchema"];
				EmptyShopKey = ConfigurationManager.AppSettings["EmptyShopKey"];
			}
		}
	}
}