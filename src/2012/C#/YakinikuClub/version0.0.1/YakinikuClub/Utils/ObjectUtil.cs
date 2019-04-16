using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.ComponentModel;
using System.Text;
using System.Collections;

namespace YakinikuClub.Utils
{
	public static class ObjectUtil
	{
		public static string DumpProperties(object instance)
		{
			if (instance == null)
			{
				throw new ArgumentNullException("instance");
			}

			var result = new StringBuilder();
			foreach (PropertyDescriptor prop in TypeDescriptor.GetProperties(instance))
			{
				if (result.Length > 0)
				{
					result.AppendLine(",");
				}
				result.AppendFormat("[{0} = {1}]", prop.Name, prop.GetValue(instance));
			}
			result.Insert(0, "Type:" + instance.GetType().Name + "{" + Environment.NewLine);
			result.AppendLine();
			result.Append("}");
			return result.ToString();
		}

		public static void CopyTo(object source, object destination)
		{
			if (source == null)
			{
				throw new ArgumentNullException("source");
			}
			if (destination == null)
			{
				throw new ArgumentNullException("destination");
			}

			var srcProps = new Dictionary<string, object>();
			foreach (PropertyDescriptor prop in TypeDescriptor.GetProperties(source))
			{
				srcProps[prop.Name] = prop.GetValue(source);
			}
			foreach (PropertyDescriptor prop in TypeDescriptor.GetProperties(destination))
			{
				if (srcProps.ContainsKey(prop.Name))
				{
					prop.SetValue(destination, srcProps[prop.Name]);
				}
			}
		}

		public static void CopyTo(IList source, IList destination)
		{
			if (source == null)
			{
				throw new ArgumentNullException("source");
			}
			if (destination == null)
			{
				throw new ArgumentNullException("destination");
			}

			for (int i = 0; i < source.Count; i++)
			{
				CopyTo(source[i], destination[i]);
			}
		}
	}
}