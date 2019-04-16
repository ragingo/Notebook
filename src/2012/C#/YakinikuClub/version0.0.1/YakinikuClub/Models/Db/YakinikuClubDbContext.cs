using System.Data.Entity;
using YakinikuClub.Models.Entity;

namespace YakinikuClub.Models.Db
{
	public class YakinikuClubDbContext : DbContext
	{
		public DbSet<Member> Members { get; set; }
		public DbSet<Shop> Shops { get; set; }
		public DbSet<ClubActivity> ClubActivities { get; set; }
		public DbSet<Participant> Participants { get; set; }

		static YakinikuClubDbContext()
		{
		}

		public YakinikuClubDbContext()
		{
		}
	}
}