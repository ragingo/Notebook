using System;
using System.Management;
using System.Collections.Generic;
using System.Text;

namespace WmiTest
{
	abstract class WmiBase
	{
		protected WmiBase()
		{
		}

		public static T CreateInstance<T>(ManagementBaseObject obj) where T : WmiBase
		{
			T instance = typeof(T).GetConstructor(new Type[] { }).Invoke(null) as T;
			foreach (var prop in typeof(T).GetProperties())
			{
				if (prop.GetCustomAttributes(typeof(WmiPropertyAttribute), true).Length == 0)
				{
					continue;
				}
				object param;
				if (prop.PropertyType == typeof(DateTime))
				{
					string value = (string)obj[prop.Name];
					if (value != null)
					{
						param = ManagementDateTimeConverter.ToDateTime(value);
					}
					else
					{
						param = null;
					}
				}
				else
				{
					param = obj[prop.Name];
				}
				prop.SetValue(instance, param, null);
			}
			return instance;
		}

		public override string ToString()
		{
			int count = 0;
			StringBuilder sb = new StringBuilder();
			sb.Append("[");
			foreach (var prop in this.GetType().GetProperties())
			{
				if (prop.GetCustomAttributes(typeof(WmiPropertyAttribute), true).Length == 0)
				{
					continue;
				}
				if (count > 0)
				{
					sb.Append(", ");
				}
				count++;
				sb.Append(prop.Name)
					.Append("=")
					.Append(prop.GetValue(this, null));
			}
			sb.Append("]");
			return sb.ToString();
		}

		public abstract string ClassName
		{
			get;
		}
	}
}
