using System.Collections.Generic;
using System.Data.Entity;
using System.Linq;
using AutoMapper;
using YakinikuClub.Models.Db;
using YakinikuClub.Models.Repository;
using Entity = YakinikuClub.Models.Entity;

namespace YakinikuClub.ViewModels.ClubActivity
{
	public class ClubActivityListViewModel
	{
		public IEnumerable<ClubActivityViewModel> ClubActivities { get; private set; }

		public ClubActivityListViewModel()
		{
			using (var ctx = new YakinikuClubDbContext())
			{
				var repos = new ClubActivityRepository(ctx);
				var list =
					repos.GetAll()
						.Include(x => x.Shop)
						.OrderByDescending(x => x.Times)
						.ToList();
				var vmList = new List<ClubActivityViewModel>();
				for (int i = 0; i < list.Count; i++)
				{
					vmList.Add(new ClubActivityViewModel());
				}
				Mapper.CreateMap<Entity.ClubActivity, ClubActivityViewModel>();
				Mapper.CreateMap<Entity.Shop, ShopViewModel>();
				var result = Mapper.Map<IEnumerable<Entity.ClubActivity>, IEnumerable<ClubActivityViewModel>>(list);
				ClubActivities = result;
			}
		}
	}
}