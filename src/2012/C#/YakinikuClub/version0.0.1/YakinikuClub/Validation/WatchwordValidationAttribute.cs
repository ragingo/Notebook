using System.ComponentModel.DataAnnotations;
using System.Configuration;

namespace YakinikuClub.Validation
{
	public class WatchwordValidationAttribute : ValidationAttribute
	{
		public override bool IsValid(object value)
		{
			if (value == null)
			{
				return false;
			}
			if (value.GetType() != typeof(string))
			{
				return false;
			}
			string inputWord = (string)value;
			string configWord = ConfigurationManager.AppSettings["Watchword"];
			if (inputWord == configWord)
			{
				return true;
			}
			return false;
		}
	}
}