using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Reflection;
using System.Text;

namespace Samples.Reflection
{
	static class MethodBaseExtentions
	{
		public static T Invoke<T>(this MethodBase self, object obj, params object[] parameters)
		{
			return (T)self.Invoke(obj, parameters);
		}
		public static T Invoke<T>(this MethodBase self, object obj, BindingFlags invokeAttr, Binder binder, CultureInfo culture, params object[] parameters)
		{
			return (T)self.Invoke(obj, invokeAttr, binder, parameters, culture);
		}
	}

}
