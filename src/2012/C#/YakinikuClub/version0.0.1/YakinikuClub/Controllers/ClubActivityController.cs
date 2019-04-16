using System.Web.Mvc;
using YakinikuClub.Binders;
using YakinikuClub.Helpers;
using YakinikuClub.Models.Db;
using YakinikuClub.Models.Repository;
using YakinikuClub.ViewModels;
using YakinikuClub.ViewModels.ClubActivity;
using YakinikuClub.ViewModels.History;

namespace YakinikuClub.Controllers
{
	public class ClubActivityController : Controller
	{
		[Authorize]
		public ActionResult Plan()
		{
			var model = this.GetMemberInfo();
			ViewBag.IsAdministrator = model.IsAdministrator;
			return View(new PlanViewModel());
		}

		[Authorize]
		public ActionResult History()
		{
			var model = this.GetMemberInfo();
			ViewBag.IsAdministrator = model.IsAdministrator;
			return View(new HistoryListViewModel());
		}

		[HttpPost]
		public ActionResult Register(
			[ModelBinder(typeof(ManagementViewModelBinder))] ClubActivityViewModel data)
		{
			if (ModelState.IsValid)
			{
				using (var ctx = new YakinikuClubDbContext())
				{
					var repos = new ClubActivityRepository(ctx);
					repos.AddClubActivity(data);
					ctx.SaveChanges();
				}
				return RedirectToAction("Index", "Home");
			}
			return RedirectToAction("Index", "Management");
		}

	}
}
