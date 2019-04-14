using System;
using System.Collections.Generic;
using System.Windows;

namespace chart_test2
{
	public partial class MainWindow : Window
	{
		public MainWindow()
		{
			InitializeComponent();

			chartbox.DataContext = GetData();
		}

		public IEnumerable<SampleData> GetData()
		{
			var data = new[] {
				new SampleData{ Title = "A", From = DateTime.Parse("2011/04/01"), To = DateTime.Parse("2011/04/05"), Star = DateTime.Parse("2011/03/31") },
				new SampleData{ Title = "B", From = DateTime.Parse("2011/04/10"), To = DateTime.Parse("2011/04/11"), Star = DateTime.Parse("2011/04/04") },
				new SampleData{ Title = "C", From = DateTime.Parse("2011/04/15"), To = DateTime.Parse("2011/04/16"), Star = DateTime.Parse("2011/04/13") },
				new SampleData{ Title = "D", From = DateTime.Parse("2011/04/01"), To = DateTime.Parse("2011/04/05"), Star = DateTime.Parse("2011/03/31") },
				new SampleData{ Title = "E", From = DateTime.Parse("2011/04/10"), To = DateTime.Parse("2011/04/11"), Star = DateTime.Parse("2011/04/04") },
				new SampleData{ Title = "F", From = DateTime.Parse("2011/04/15"), To = DateTime.Parse("2011/04/16"), Star = DateTime.Parse("2011/04/13") },
				new SampleData{ Title = "G", From = DateTime.Parse("2011/04/01"), To = DateTime.Parse("2011/04/05"), Star = DateTime.Parse("2011/03/31") },
				new SampleData{ Title = "H", From = DateTime.Parse("2011/04/10"), To = DateTime.Parse("2011/04/11"), Star = DateTime.Parse("2011/04/04") },
				new SampleData{ Title = "I", From = DateTime.Parse("2011/04/15"), To = DateTime.Parse("2011/04/16"), Star = DateTime.Parse("2011/04/13") },
			};
			return data;
		}
	}
}
