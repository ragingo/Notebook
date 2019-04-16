using System.ComponentModel.DataAnnotations;
using System.Web.Mvc;
using YakinikuClub.Validation;

namespace YakinikuClub.Models
{
	public class RegisterModel
	{
		[Display(Name = "ユーザー名")]
		[Required]
		public string UserName { get; set; }

		[Display(Name = "メールアドレス")]
		[Required]
		[DataType(DataType.EmailAddress)]
		public string Email { get; set; }

		[Display(Name = "パスワード")]
		[Required]
		[StringLength(100, ErrorMessage = "The {0} must be at least {2} characters long.", MinimumLength = 6)]
		[DataType(DataType.Password)]
		public string Password { get; set; }

		[Display(Name = "パスワード再確認")]
		[DataType(DataType.Password)]
		[Compare("Password", ErrorMessage = "The password and confirmation password do not match.")]
		public string ConfirmPassword { get; set; }

		[Display(Name = "合言葉")]
		[Required]
		[WatchwordValidation(ErrorMessage = "合言葉が違います。")]
		public string Watchword { get; set; }
	}
}