using System.ComponentModel.DataAnnotations;
using System.Web.Mvc;
using YakinikuClubV2.Common;
using YakinikuClubV2.Validation;

namespace YakinikuClubV2.Models.Account
{
	public class RegisterModel
	{
		[Required]
		[Display(Name = "ユーザ名")]
		public string UserName { get; set; }

		[Required]
		[DataType(DataType.EmailAddress)]
		[Display(Name = "メールアドレス")]
		public string Email { get; set; }

		[Required]
		[StringLength(Constants.Model.PasswordMaxLength, MinimumLength = Constants.Model.PasswordMinLength, ErrorMessage = "{0}は{2}～{1}文字で入力してください。")]
		[DataType(DataType.Password)]
		[Display(Name = "パスワード")]
		public string Password { get; set; }

		[DataType(DataType.Password)]
		[Display(Name = "確認用パスワード")]
		[Compare("Password", ErrorMessage = "入力したパスワードと異なります。")]
		public string ConfirmPassword { get; set; }

		[Required]
		[DataType(DataType.Password)]
		[Display(Name = "合言葉")]
		[WatchwordValidation(ErrorMessage = "合言葉が違います。")]
		public string Watchword { get; set; }
	}
}