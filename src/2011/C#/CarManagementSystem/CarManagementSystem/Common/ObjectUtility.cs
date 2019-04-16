using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

namespace CarManagementSystem.Common
{
	public static class ObjectUtility
	{
		public static T DeepCopy<T>(T original)
			where T : class, new()
		{
			using (var ms = new MemoryStream())
			{
				var bf = new BinaryFormatter();
				bf.Serialize(ms, original);
				ms.Position = 0;
				T result = (T)bf.Deserialize(ms);
				return result;
			}
		}

		public static void CopyPropertiesValue<T>(T source, T destination)
		{
			foreach (var prop in typeof(T).GetProperties())
			{
				prop.SetValue(destination, prop.GetValue(source, null), null);
			}
		}
	}
}
