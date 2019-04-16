using System.Web.Mvc;
using YakinikuClub.Helpers;
using YakinikuClub.ViewModels.Member;

namespace YakinikuClub.Controllers
{
	public class MemberController : Controller
	{
		[Authorize]
		public ActionResult Index()
		{
			var model = this.GetMemberInfo();
			ViewBag.IsAdministrator = model.IsAdministrator;
			return View(new MemberListViewModel());//test
		}
	}
}
