using System.ComponentModel.DataAnnotations;

namespace YakinikuClub.ViewModels
{
	public class ShopViewModel
	{
		[Display(Name = "店コード")]
		public int ShopCode { get; set; }

		[Display(Name = "場所コード")]
		public int LocationCode { get; set; }

		[Display(Name = "店名")]
		[StringLength(1000), Required]
		public string ShopName { get; set; }

		[Display(Name = "住所")]
		[Required]
		public string Address { get; set; }

		[Display(Name = "電話番号")]
		[DataType(DataType.PhoneNumber)]
		public string PhoneNumber { get; set; }

		[Display(Name = "緯度")]
		public double? Latitude { get; set; }

		[Display(Name = "経度")]
		public double? Longitude { get; set; }

		public string Memo { get; set; }

		public string Key
		{
			get
			{
				return string.Join("-", ShopCode, LocationCode);
			}
		}
	}
}