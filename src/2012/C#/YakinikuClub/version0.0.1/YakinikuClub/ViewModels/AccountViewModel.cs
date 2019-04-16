using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using YakinikuClub.Models;
using System.Web.Security;
using YakinikuClub.Models.Db;
using YakinikuClub.Models.Repository;

namespace YakinikuClub.ViewModels
{
	class AccountViewModel : ViewModelBase
	{
		public AccountViewModel()
		{
		}

		public MembershipCreateStatus Register(RegisterModel model)
		{
			MembershipCreateStatus createStatus;
			Membership.CreateUser(model.UserName, model.Password, model.Email, null, null, true, null, out createStatus);
			using (var ctx = new YakinikuClubDbContext())
			{
				var memberRepository = new MemberRepository(ctx);
				memberRepository.AddMember(model);
				ctx.SaveChanges();
			}
			return createStatus;
		}
	}
}