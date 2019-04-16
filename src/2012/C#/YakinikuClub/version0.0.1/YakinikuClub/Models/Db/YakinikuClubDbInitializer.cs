using System.Data.Entity.Migrations;

namespace YakinikuClub.Models.Db
{
	class YakinikuClubDbInitializer : DbMigrationsConfiguration<YakinikuClubDbContext>
	{
		public YakinikuClubDbInitializer()
		{
			AutomaticMigrationsEnabled = true;
			AutomaticMigrationDataLossAllowed = true;
		}
	}
}