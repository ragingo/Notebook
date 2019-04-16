using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Linq;
using System.Web;
using YakinikuClub.Models.Db;
using YakinikuClub.Models.Repository;
using Entity = YakinikuClub.Models.Entity;
using AutoMapper;

namespace YakinikuClub.ViewModels.History
{
	public class HistoryListViewModel
	{
		public IEnumerable<HistoryViewModel> Histories { get; private set; }

		public HistoryListViewModel()
		{
			using (var ctx = new YakinikuClubDbContext())
			{
				var reposClubActivity = new ClubActivityRepository(ctx);
				var reposParticipant = new ParticipantRepository(ctx);
				var reposMember = new MemberRepository(ctx);

				Mapper.CreateMap<Entity.Shop, ShopViewModel>();

				var list =
					(from ca in reposClubActivity.GetAll().Include(x => x.Shop)
					 join p in reposParticipant.GetAll() on ca.Times equals p.Times
					 join m in reposMember.GetAll() on p.MemberId equals m.MemberId
					 where ca.StartDate < DateTime.Now
					 orderby ca.Times descending
					 group new { p, m } by new { ca.Times, ca.StartDate, ca.EndDate, ca.Shop, ca.Memo }
					).ToList();

				var result =
					from item in list
					let memo = 
						(item.Key.Memo + Environment.NewLine +
						 string.Join(Environment.NewLine, item.Select(x => x.m.NickName))
						).Trim('\r', '\n')
					select new HistoryViewModel
					{
						Times = item.Key.Times,
						StartDate = item.Key.StartDate,
						EndDate = item.Key.EndDate,
						Shop = Mapper.Map<Entity.Shop, ShopViewModel>(item.Key.Shop),
						Memo = memo
					};

				Histories = result;
			}
		}
	}
}