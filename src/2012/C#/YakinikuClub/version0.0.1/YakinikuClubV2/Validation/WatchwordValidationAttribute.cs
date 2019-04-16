using System.ComponentModel.DataAnnotations;
using System.Configuration;
using YakinikuClubV2.Common;

namespace YakinikuClubV2.Validation
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
			if (inputWord == Constants.Model.Watchword)
			{
				return true;
			}
			return false;
		}
	}
}