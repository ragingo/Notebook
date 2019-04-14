using System;
using System.Globalization;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;

namespace WpfApplication1
{
	[ValueConversion(typeof(DataGridLength), typeof(GridLength))]
	class DataGridLengthToGridLengthConverter : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			if (value == null || !(value is DataGridLength))
			{
				return GridLength.Auto;
			}
			var result = new GridLength(((DataGridLength)value).DesiredValue, GridUnitType.Star);
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
