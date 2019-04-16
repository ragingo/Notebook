using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Linq;
using YakinikuClub.Models.Db;
using Entity = YakinikuClub.Models.Entity;
using YakinikuClub.Models.Repository;

namespace YakinikuClub.ViewModels
{
	//public class HistoryViewModel
	//{
	//    public IEnumerable<Entity.ClubActivity> ClubActivities { get; private set; }

	//    public HistoryViewModel()
	//    {
	//        using (var ctx = new YakinikuClubDbContext())
	//        {
	//            var repos = new ClubActivityRepository(ctx);

	//            ClubActivities =
	//                repos.GetAll()
	//                    .Include(x => x.Shop)
	//                    .Where(x => x.StartDate < DateTime.Now)
	//                    .OrderByDescending(x => x.Times)
	//                    .ToList();
	//        }
	//    }
	//}
}