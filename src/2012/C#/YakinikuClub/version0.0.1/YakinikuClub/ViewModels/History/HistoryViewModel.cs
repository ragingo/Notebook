using System;

namespace YakinikuClub.ViewModels.History
{
	public class HistoryViewModel
	{
		public int Times { get; set; }

		public DateTime StartDate { get; set; }

		public DateTime EndDate { get; set; }

		public ShopViewModel Shop { get; set; }

		public string Memo { get; set; }
	}
}