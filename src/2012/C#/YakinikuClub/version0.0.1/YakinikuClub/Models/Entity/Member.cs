using System.ComponentModel.DataAnnotations;

namespace YakinikuClub.Models.Entity
{
	public class Member
	{
		[Key, Column(Order = 0)]
		public int MemberId { get; set; }

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