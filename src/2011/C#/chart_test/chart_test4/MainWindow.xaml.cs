using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Collections;

namespace chart_test4
{
	public partial class MainWindow : Window
	{
		public MainWindow()
		{
			InitializeComponent();

			this.DataContext = GetData();

			CreateTimeTable(DateTime.Parse("2011/04/01"), DateTime.Parse("2011/05/01"), 7, GetData());
		}

		private void CreateTimeTable(DateTime min, DateTime max, int intervalDays, IEnumerable<SampleData> data)
		{
			grid.ColumnDefinitions.Clear();
			grid.RowDefinitions.Clear();

			int days = (max - min).Days;
			double w = 30;
			double h = 30;
			for (int i = 0; i < days; i++)
			{
				grid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(w) });
			}
			for (int row = 0; row < data.Count(); row++ )
			{
				grid.RowDefinitions.Add(new RowDefinition { Height = new GridLength(h) });
				for (int col = 0; col < days; col++)
				{
					var border = new Border { BorderBrush = Brushes.Black, BorderThickness = new Thickness(1) };
					grid.Children.Add(border);
					Grid.SetRow(border, row);
					Grid.SetColumn(border, col);
				}
			}
			int idx = 0;
			foreach (var item in data)
			{
				int from = Math.Abs((min - item.From).Days);
				int to = (item.To - item.From).Days + 1;
				var rect = new TextBlock { Background = Brushes.AliceBlue, Text = item.RangeLabel, FontSize = 9 };
				grid.Children.Add(rect);
				Grid.SetColumn(rect, from);
				Grid.SetColumnSpan(rect, to);
				Grid.SetRow(rect, idx);
				++idx;
			}
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
