using System.Linq;
using YakinikuClub.Models.Db;
using YakinikuClub.Models.Entity;
using YakinikuClub.ViewModels.ClubActivity;

namespace YakinikuClub.Models.Repository
{
	public class MemberRepository : RepositoryBase<YakinikuClubDbContext, Member>
	{
		public MemberRepository(YakinikuClubDbContext ctx)
			: base(ctx)
		{
		}
		public void AddMember(RegisterModel model)
		{
			var member = Create();
			member.MemberId = GetNextId();
			member.NickName = model.UserName;
			member.Email = model.Email;
			Add(member);
		}
		private int GetNextId()
		{
			int nextId = (_ctx.Members.Max(x => (int?)x.MemberId) ?? 0) + 1;
			return nextId;
		}
	}
	public class ShopRepository : RepositoryBase<YakinikuClubDbContext, Shop>
	{
		public ShopRepository(YakinikuClubDbContext ctx)
			: base(ctx)
		{
		}
	}
	public class ClubActivityRepository : RepositoryBase<YakinikuClubDbContext, ClubActivity>
	{
		public ClubActivityRepository(YakinikuClubDbContext ctx)
			: base(ctx)
		{
		}

		public void AddClubActivity(ClubActivityViewModel viewModel)
		{
			var reposShop = new ShopRepository(_ctx);
			var activity = Create();
			activity.Times = viewModel.Times;
			activity.StartDate = viewModel.StartDate;
			activity.EndDate = viewModel.EndDate;
			activity.Memo = viewModel.Memo;
			activity.Shop = reposShop.Get(x => x.Key == viewModel.Shop.Key);
			Add(activity);
		}
	}
	public class ParticipantRepository : RepositoryBase<YakinikuClubDbContext, Participant>
	{
		public ParticipantRepository(YakinikuClubDbContext ctx)
			: base(ctx)
		{
		}
	}
}