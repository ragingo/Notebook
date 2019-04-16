using System.Collections.Generic;
using System.Linq;
using AutoMapper;
using YakinikuClub.Models.Db;
using YakinikuClub.Models.Repository;
using Entity = YakinikuClub.Models.Entity;

namespace YakinikuClub.ViewModels.Member
{
	public class MemberListViewModel
	{
		public IEnumerable<MemberViewModel> Members { get; set; }
		public MemberListViewModel()
		{
			using (var ctx = new YakinikuClubDbContext())
			{
				var repos = new MemberRepository(ctx);
				var list = repos.GetAll().ToList();
				var vmList = new List<MemberViewModel>();
				for (int i = 0; i < list.Count; i++)
				{
					vmList.Add(new MemberViewModel());
				}
				Mapper.CreateMap<Entity.Member, MemberViewModel>();
				var result = Mapper.Map<IEnumerable<Entity.Member>, IEnumerable<MemberViewModel>>(list);
				Members = result;
			}
		}
	}
}