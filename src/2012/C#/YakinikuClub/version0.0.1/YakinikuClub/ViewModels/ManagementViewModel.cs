using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using YakinikuClub.ViewModels.ClubActivity;

namespace YakinikuClub.ViewModels
{
	public class ManagementViewModel
	{
		public ClubActivityRegisterViewModel ClubActivityRegister { get; set; }
		public ClubActivityListViewModel ClubActivityList { get; set; }

		public ManagementViewModel()
		{
			ClubActivityRegister = new ClubActivityRegisterViewModel();
			ClubActivityList = new ClubActivityListViewModel();
		}
	}
}