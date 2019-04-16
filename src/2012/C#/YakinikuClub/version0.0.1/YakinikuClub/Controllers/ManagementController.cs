using System.Collections;
using System.Web.Mvc;
using YakinikuClub.Binders;
using YakinikuClub.Helpers;
using YakinikuClub.Models.Db;
using YakinikuClub.Models.Repository;
using YakinikuClub.ViewModels;
using YakinikuClub.ViewModels.ClubActivity;

namespace YakinikuClub.Controllers
{
	public class ManagementController : Controller
	{
		[Authorize]
		public ActionResult Index()
		{
			var model = this.GetMemberInfo();
			ViewBag.IsAdministrator = model.IsAdministrator;
			ViewBag.DataKindList = GetDataKindList();
			return View(new ManagementViewModel());
		}

		private IEnumerable GetDataKindList()
		{
			var list = new[] {
				new { Value = "0", Text = "未選択" },
				new { Value = "1", Text = "部活動" },
			};
			return list;
		}
	}
}
