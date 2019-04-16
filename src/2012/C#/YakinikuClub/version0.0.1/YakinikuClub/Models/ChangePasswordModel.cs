using System.ComponentModel.DataAnnotations;
using System.Web.Mvc;

namespace YakinikuClub.Models
{
	public class ChangePasswordModel : ModelBase
	{
		[Required]
		[DataType(DataType.Password)]
		[Display(Name = "現在のパスワード")]
		public string OldPassword { get; set; }

		[Required]
		[StringLength(100, ErrorMessage = "The {0} must be at least {2} characters long.", MinimumLength = 6)]
		[DataType(DataType.Password)]
		[Display(Name = "新パスワード")]
		public string NewPassword { get; set; }

		[DataType(DataType.Password)]
		[Display(Name = "新パスワード再確認")]
		[Compare("NewPassword", ErrorMessage = "The new password and confirmation password do not match.")]
		public string ConfirmPassword { get; set; }
	}
}