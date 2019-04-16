using System.Web.Mvc;
using YakinikuClub.Models.Db;
using YakinikuClub.Models.Entity;
using YakinikuClub.Models.Repository;

namespace YakinikuClub.Helpers
{
	static class ControllerHelper
	{
		public static Member GetMemberInfo(this Controller controller)
		{
			if (!controller.Request.IsAuthenticated)
			{
				return null;
			}
			using (var ctx = new YakinikuClubDbContext())
			{
				var repos = new MemberRepository(ctx);
				var model = repos.Get(x => x.NickName == controller.User.Identity.Name);
				return model;
			}
		}
	}
}