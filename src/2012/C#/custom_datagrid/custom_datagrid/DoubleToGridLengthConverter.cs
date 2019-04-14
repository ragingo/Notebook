using System;
using System.Globalization;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;

namespace WpfApplication1
{
	[ValueConversion(typeof(double), typeof(GridLength))]
	class DoubleToGridLengthConverter : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			if (value == null || !(value is double))
			{
				return GridLength.Auto;
			}
			var result = new GridLength((double)value);
			return result;
		}

		public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			if (value == null || !(value is GridLength))
			{
				return DataGridLength.Auto;
			}
			var result = new DataGridLength(((GridLength)value).Value);
			return result;
		}
	}
}
