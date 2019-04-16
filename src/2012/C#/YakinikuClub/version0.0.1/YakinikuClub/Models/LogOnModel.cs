using System.ComponentModel.DataAnnotations;

namespace YakinikuClub.Models
{
	public class LogOnModel : ModelBase
	{
		[Required]
		[Display(Name = "ユーザー名")]
		public string UserName { get; set; }

		[Required]
		[DataType(DataType.Password)]
		[Display(Name = "パスワード")]
		public string Password { get; set; }

		[Display(Name = "Remember me?")]
		public bool RememberMe { get; set; }
	}
}