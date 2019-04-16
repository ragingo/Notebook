using System.ComponentModel.DataAnnotations;

namespace YakinikuClubV2.Models.Account
{
	public class LoginModel
	{
		[Required]
		[Display(Name = "ユーザ名")]
		public string UserName { get; set; }

		[Required]
		[DataType(DataType.Password)]
		[Display(Name = "パスワード")]
		public string Password { get; set; }
	}
}