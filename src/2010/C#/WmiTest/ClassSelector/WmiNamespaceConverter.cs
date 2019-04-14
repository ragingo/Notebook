using System;
using System.Collections.Generic;
using System.Globalization;
using System.Management;
using System.Windows.Data;

namespace ClassSelector
{
	public class WmiNamespaceConverter : IValueConverter
	{
		private static IEnumerable<ManagementPath> GetWmiNamespaces(ManagementPath path, string condition)
		{
			var scope = new ManagementScope(path, null);
			var searcher = new ManagementObjectSearcher(scope, new SelectQuery("__NAMESPACE", condition));

			foreach (ManagementObject obj in searcher.Get())
			{
				yield return new ManagementPath(path.Path + "\\" + obj["Name"]);
			}
		}

		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			if (value is ManagementPath)
			{
				try
				{
					return GetWmiNamespaces(value as ManagementPath, null);
				}
				catch
				{
				}
			}
			return null;
		}

		public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			throw new NotImplementedException();
		}
	}
}
