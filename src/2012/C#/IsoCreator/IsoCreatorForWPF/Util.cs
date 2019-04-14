using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace IsoCreatorForWPF
{
	public static class Util
	{
		public static void RaisePropertyChanged<T>(ViewModelBase instance, string propertyName, ref T oldValue, ref T newValue)
		{
			if (oldValue == null && newValue != null)
			{
				oldValue = newValue;
				instance.RaisePropertyChanged(propertyName);
			}
			else if (!oldValue.Equals(newValue))
			{
				oldValue = newValue;
				instance.RaisePropertyChanged(propertyName);
			}
		}
	}
}
