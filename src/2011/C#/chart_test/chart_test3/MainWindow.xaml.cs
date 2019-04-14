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

namespace chart_test3
{
	public partial class MainWindow : Window
	{
		public MainWindow()
		{
			InitializeComponent();

			this.DataContext = GetData();
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

	public class DateTimeToDoubleConverter : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
		{
			return ((DateTime)value).ToOADate();
		}

		public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
		{
			throw new NotImplementedException();
		}
	}

}
