using System.Data.Entity;

namespace CarManagementSystem.Models
{
	public class CmsDbContext : DbContext
	{
		public DbSet<Customer> Customers
		{
			get;
			set;
		}

		public DbSet<Car> Cars
		{
			get;
			set;
		}

		public DbSet<WorkHistory> WorkHistories
		{
			get;
			set;
		}
	}
}
