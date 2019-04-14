using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Management;
using System.Dynamic;
using System.Reflection;

namespace ConsoleApplication1
{
	class DynamicMemberInfo
	{
		public string Name { get; set; }
		public MemberTypes MemberTypes { get; set; }
		public Type Type { get; set; }
	}
	class ManagementBaseObjectDynamicObject : DynamicObject
	{
		private Dictionary<string, Type> _members = new Dictionary<string, Type>();
		private ManagementBaseObject _obj = null;

		public ManagementBaseObjectDynamicObject(ManagementBaseObject obj)
		{
			_obj = obj;
			foreach (PropertyData data in _obj.Properties)
			{
				_members[data.Name] = ConvertFromCimType(data.Type, data.IsArray);
			}
		}

		public override bool TryGetMember(GetMemberBinder binder, out object result)
		{
			result = _obj.GetPropertyValue(binder.Name);
			return true;
		}

		public override IEnumerable<string> GetDynamicMemberNames()
		{
			return _members.Keys;
		}

		public override bool TryConvert(ConvertBinder binder, out object result)
		{
			return base.TryConvert(binder, out result);
		}

		private static Type ConvertFromCimType(CimType type, bool isArray)
		{
			Type t = null;
			switch (type)
			{
			case CimType.Boolean:
				t = typeof(bool);
				break;
			case CimType.Char16:
				t = typeof(char);
				break;
			case CimType.DateTime:
				t = typeof(DateTime);
				break;
			case CimType.Object:
				t = typeof(object);
				break;
			case CimType.Real32:
				t = typeof(decimal);
				break;
			case CimType.Real64:
				t = typeof(decimal);
				break;
			case CimType.Reference:
				t = typeof(object);
				break;
			case CimType.SInt16:
				t = typeof(short);
				break;
			case CimType.SInt32:
				t = typeof(int);
				break;
			case CimType.SInt64:
				t = typeof(long);
				break;
			case CimType.SInt8:
				t = typeof(sbyte);
				break;
			case CimType.String:
				t = typeof(string);
				break;
			case CimType.UInt16:
				t = typeof(ushort);
				break;
			case CimType.UInt32:
				t = typeof(uint);
				break;
			case CimType.UInt64:
				t = typeof(ulong);
				break;
			case CimType.UInt8:
				t = typeof(byte);
				break;
			}
			if (isArray)
			{
				t = t.MakeArrayType();
			}
			return t;
		}

		public override string ToString()
		{
			var result =
				"{" +
					string.Join(", ",
						_obj.Properties
							.Cast<PropertyData>()
							.Select(data => data.Name + ":" + data.Value)
					) +
				"}";
			return result;
		}
	}

	static class ManagementBaseObjectExtensions
	{
		public static dynamic ToDynamic(this ManagementBaseObject managementObject)
		{
			return new ManagementBaseObjectDynamicObject(managementObject);
		}
	}
}
