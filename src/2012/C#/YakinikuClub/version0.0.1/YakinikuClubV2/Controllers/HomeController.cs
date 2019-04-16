using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;

namespace YakinikuClubV2.Controllers
{
	public class HomeController : Controller
	{
		public ActionResult Index()
		{
			ViewBag.Message = "ようこそ！";
			return View();
		}

		public ActionResult About()
		{
			ViewBag.Message = "焼肉部について";
			return View();
		}

		public ActionResult Contact()
		{
			ViewBag.Message = "焼肉部情報";
			return View();
		}
	}
}
