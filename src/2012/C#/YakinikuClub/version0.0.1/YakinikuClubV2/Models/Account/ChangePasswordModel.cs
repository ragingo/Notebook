using System.ComponentModel.DataAnnotations;
using System.Web.Mvc;
using YakinikuClubV2.Common;

namespace YakinikuClubV2.Models.Account
{
	public class ChangePasswordModel
	{
		[Required]
		[DataType(DataType.Password)]
		[Display(Name = "現在のパスワード")]
		public string OldPassword { get; set; }

		[Required]
		[StringLength(Constants.Model.PasswordMaxLength, MinimumLength = Constants.Model.PasswordMinLength, ErrorMessage = "{0}は{2}～{1}文字で入力してください。")]
		[DataType(DataType.Password)]
		[Display(Name = "新しいパスワード")]
		public string NewPassword { get; set; }

		[DataType(DataType.Password)]
		[Display(Name = "確認用パスワード")]
		[Compare("NewPassword", ErrorMessage = "入力した新しいパスワードと異なります。")]
		public string ConfirmPassword { get; set; }
	}
}