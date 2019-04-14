using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GalaSoft.MvvmLight;

namespace ExcelGrepForWPF.ViewModel
{
	class MainWindowViewModel : ViewModelBase
	{
		public string DirectoryName
		{
			get;
			set;
		}

		public class HitItem
		{
			public string FileName { get; set; }

			public string Row { get; set; }

			public string Column { get; set; }

			public string Value { get; set; }
		}

		public ObservableCollection<HitItem> HitItems { get; set; }
	}
}
