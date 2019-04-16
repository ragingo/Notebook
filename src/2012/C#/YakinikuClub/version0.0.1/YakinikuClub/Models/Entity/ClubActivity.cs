using System;
using System.ComponentModel.DataAnnotations;

namespace YakinikuClub.Models.Entity
{
	public class ClubActivity
	{
		[Display(Name = "回数")]
		[Key, Column(Order = 0), DatabaseGenerated(DatabaseGeneratedOption.None)]
		public int Times { get; set; }

		[Display(Name = "開始日時")]
		[DataType(DataType.DateTime), Required]
		public DateTime StartDate { get; set; }

		[Display(Name = "終了日時")]
		[DataType(DataType.DateTime), Required]
		public DateTime EndDate { get; set; }

		[Display(Name = "備考")]
		public string Memo { get; set; }

		public Shop Shop { get; set; }

		public ClubActivity()
		{
			StartDate = DateTime.Today;
			EndDate = DateTime.Today;
		}
	}
}