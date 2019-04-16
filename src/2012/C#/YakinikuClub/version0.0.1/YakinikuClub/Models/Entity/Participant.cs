
using System.ComponentModel.DataAnnotations;
namespace YakinikuClub.Models.Entity
{
	public class Participant
	{
		[Key, Column(Order = 0)]
		public int Times { get; set; }

		[Key, Column(Order = 1)]
		public int MemberId { get; set; }

		[ForeignKey("Times")]
		public ClubActivity ClubActivity { get; set; }

		[ForeignKey("MemberId")]
		public Member Member { get; set; }
	}
}