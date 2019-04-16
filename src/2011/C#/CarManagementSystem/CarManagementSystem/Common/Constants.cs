
namespace CarManagementSystem.Common
{
	public static class Constants
	{
		public static class Path
		{
			public const string GoogleMapJs = "CarManagementSystem.Resources.map.js.template";
		}
		public static class RegexPattern
		{
			public const string PhoneNumber = @"(\d{2}-?\d{4}-?\d{4})|(\d{3}-?\d{3}-?\d{4})|(\d{4}-?\d{2}-?\d{4})|(\d{3}-?\d{4}-?\d{4})";
			public const string ZipCode = @"\d{3}-\d{4}";
		}
	}
}
