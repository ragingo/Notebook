using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using YakinikuClub.Models.Db;
using YakinikuClub.Models.Repository;
using YakinikuClub.Utils;

namespace YakinikuClub.ViewModels.ClubActivity
{
	public class ClubActivityRegisterViewModel
	{
		[Display(Name = "回数")]
		[Required]
		public int Times { get; set; }

		[Display(Name = "開始日時")]
		[DataType(DataType.DateTime), Required]
		public DateTime StartDate { get; set; }

		[Display(Name = "終了日時")]
		[DataType(DataType.DateTime), Required]
		public DateTime EndDate { get; set; }

		[Display(Name = "備考")]
		public string Memo { get; set; }

		public List<ShopViewModel> Shops { get; set; }

		public ClubActivityRegisterViewModel()
		{
			StartDate = DateTime.Today;
			EndDate = DateTime.Today;

			using (var ctx = new YakinikuClubDbContext())
			{
				var reposShop = new ShopRepository(ctx);
				var shops = reposShop.GetAll().ToList();
				var shopViewModels = new List<ShopViewModel>();
				for (int i = 0; i < shops.Count; i++)
				{
					shopViewModels.Add(new ShopViewModel());
				}
				ObjectUtil.CopyTo(shops, shopViewModels);
				Shops = shopViewModels;
			}
		}
	}
}