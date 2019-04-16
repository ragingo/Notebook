using System.Web.Mvc;
using YakinikuClub.Helpers;
using YakinikuClub.ViewModels;

namespace YakinikuClub.Controllers
{
	public class HomeController : Controller
	{
		public HomeController()
		{
			ViewBag.IsAdministrator = false;
		}

		public ActionResult Index()
		{
			ViewBag.Message = "焼肉部 へ ようこそ！";
			if (Request.IsAuthenticated)
			{
				var model = this.GetMemberInfo();
				ViewBag.IsAdministrator = model.IsAdministrator;
			}
			return View();
		}

		public ActionResult About()
		{
			if (Request.IsAuthenticated)
			{
				var model = this.GetMemberInfo();
				ViewBag.IsAdministrator = model.IsAdministrator;
			}
			return View();
		}
	}
}
