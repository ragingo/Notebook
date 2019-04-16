using System.ComponentModel.DataAnnotations;

namespace YakinikuClub.ViewModels.Member
{
	public class MemberViewModel
	{
		[StringLength(50)]
		public string NickName { get; set; }

		[StringLength(50)]
		public string FirstName { get; set; }

		[StringLength(50)]
		public string LastName { get; set; }

		[DataType(DataType.EmailAddress)]
		public string Email { get; set; }

		[DataType(DataType.Password)]
		public string Password { get; set; }

		public int? Age { get; set; }

		public string Memo { get; set; }

		public bool IsAdministrator { get; set; }
	}
}