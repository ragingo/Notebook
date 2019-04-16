using System;
using System.Linq;
using System.Collections.Generic;
using System.Data.Entity;
using System.ComponentModel.DataAnnotations;
using YakinikuClub.Models.Db;
using YakinikuClub.Models.Repository;
using YakinikuClub.Utils;

namespace YakinikuClub.ViewModels.ClubActivity
{
	public class ClubActivityViewModel
	{
		[Display(Name = "回数")]
		public int Times { get; set; }

		[Display(Name = "開始日時")]
		[DataType(DataType.DateTime)]
		public DateTime StartDate { get; set; }

		[Display(Name = "終了日時")]
		[DataType(DataType.DateTime)]
		public DateTime EndDate { get; set; }

		[Display(Name = "備考")]
		public string Memo { get; set; }

		public ShopViewModel Shop { get; set; }

		public ClubActivityViewModel()
		{
		}
	}
}