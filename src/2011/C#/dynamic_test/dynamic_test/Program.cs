using System;
using System.ComponentModel;
using System.Dynamic;
using System.Linq;
using System.Collections.Generic;

namespace ConsoleApplication1
{
	class Program
	{
		// まだ試してない：http://ideone.com/phG7O
		static void Main(string[] args)
		{
			var a = new ExpandoObject();
			a.AddMember("Name");
			a.SetValue("Name", "saku");
			dynamic b = a;
			Console.WriteLine(b.Name);
		}
	}

	public static class ExpandoObjectExtensions
	{
		public static void AddMember(this ExpandoObject instance, string name)
		{
			(instance as IDictionary<string, object>).Add(name, null);
		}
		public static dynamic GetValue(this ExpandoObject instance, string name)
		{
			return (instance as IDictionary<string, object>)[name];
		}
		public static void SetValue(this ExpandoObject instance, string name, object value)
		{
			(instance as IDictionary<string, object>)[name] = value;
		}
	}

	public class CustomDynamicObject : DynamicObject, INotifyPropertyChanged
	{
		private class PropertyInfo
		{
			public string Name
			{
				get;
				set;
			}
			public Type Type
			{
				get;
				set;
			}
			public Func<object> Getter
			{
				get;
				set;
			}
			public Action<object> Setter
			{
				get;
				set;
			}
			public bool IsReadOnly
			{
				get
				{
					return Setter == null;
				}
			}
			public ICollection<Attribute> Attributes
			{
				get;
				set;
			}
		}
		
		private Dictionary<string, PropertyInfo> _properties;

		public CustomDynamicObject()
		{
			_properties = new Dictionary<string, PropertyInfo>();
		}

		public override IEnumerable<string> GetDynamicMemberNames()
		{
			return _properties.Keys;
		}

		public void CreateProperty(string name, IEnumerable<Attribute> attrs = null, Func<object> getter = null, Action<object> setter = null)
		{
			var prop = new PropertyInfo();
			prop.Name = name;
			prop.Attributes = new List<Attribute>(attrs);
			prop.Getter = getter;
			prop.Setter = setter;
			_properties.Add(name, prop);
		}

		public override bool TryGetMember(GetMemberBinder binder, out object result)
		{
			if (!_properties.ContainsKey(binder.Name))
			{
				_properties.Add(binder.Name, null);
			}
			result = _properties[binder.Name];
			return true;
		}

		public override bool TrySetMember(SetMemberBinder binder, object value)
		{
			//_properties[binder.Name] = value;
			if (PropertyChanged != null)
			{
				PropertyChanged(this, new PropertyChangedEventArgs(binder.Name));
			}
			return true;
		}

		public event PropertyChangedEventHandler PropertyChanged;
	}
}
