using System;
using System.Data.Entity;
using System.Windows;
using CarManagementSystem.Models;
using CarManagementSystem.ViewModels;
using CarManagementSystem.Views;

namespace CarManagementSystem
{
	public partial class App : Application
	{
		protected override void OnStartup(StartupEventArgs e)
		{
			CreateData();

			base.OnStartup(e);
			var view = new MainView();
			view.DataContext = new MainViewModel(){ View = view };
			view.Show();
		}

		private void CreateData()
		{
			Database.SetInitializer(new CmsDbInitializer());
			using(var ctx = new CmsDbContext())
			{
				ctx.Database.Initialize(true);
			}
		}
	}

	class CmsDbInitializer : DropCreateDatabaseAlways<CmsDbContext>
	{
		protected override void Seed(CmsDbContext context)
		{
			var customers = new Customer[] {
				new Customer{ Name = "東京太郎", Address = "東京都" },
				new Customer{ Name = "埼玉太郎", Address = "埼玉県" },
				new Customer{ Name = "千葉太郎", Address = "千葉県" },
			};
			foreach (var c in customers)
			{
				context.Customers.Add(c);
			}
			context.SaveChanges();
			var cars = new Car[] {
				new Car{ Customer = customers[0], Number = "東京50 あ 46-49", Name = "クラウン1", Color = "黒", StoredDate = DateTime.Today },
				new Car{ Customer = customers[0], Number = "東京50 あ 46-50", Name = "クラウン2", Color = "黒", StoredDate = DateTime.Today },
				new Car{ Customer = customers[0], Number = "東京50 あ 46-51", Name = "クラウン3", Color = "黒", StoredDate = DateTime.Today },
				new Car{ Customer = customers[1], Number = "埼玉50 あ 46-49", Name = "クラウン4", Color = "赤", StoredDate = DateTime.Today },
				new Car{ Customer = customers[2], Number = "千葉50 あ 46-49", Name = "クラウン5", Color = "白", StoredDate = DateTime.Today },
			};
			foreach (var c in cars)
			{
				context.Cars.Add(c);
			}
			context.SaveChanges();
			var histories = new WorkHistory[] {
				new WorkHistory { Car = cars[0], DistanceTraveled = 123, Quantity = 4, UnitCost = 5000, Size = "10", Work = "タイヤ交換" },
				new WorkHistory { Car = cars[0], DistanceTraveled = 123, Quantity = 4, UnitCost = 1000, Size = "-", Work = "掃除" },
			};
			foreach (var h in histories)
			{
				context.WorkHistories.Add(h);
			}
			context.SaveChanges();
		}
	}
}
