using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Dynamic;
using System.Linq.Expressions;

namespace ConsoleApplication1
{
	class CustomDynamicObject : IDynamicMetaObjectProvider, ICustomTypeDescriptor
	{
		private readonly Dictionary<string, object> _cache = new Dictionary<string, object>();
		public DynamicMetaObject GetMetaObject(Expression parameter)
		{
			return new CustomDynamicMetaObject(parameter, this, _cache);
		}

		public AttributeCollection GetAttributes()
		{
			return new AttributeCollection();
		}

		public string GetClassName()
		{
			throw new NotImplementedException();
		}

		public string GetComponentName()
		{
			throw new NotImplementedException();
		}

		public TypeConverter GetConverter()
		{
			throw new NotImplementedException();
		}

		public EventDescriptor GetDefaultEvent()
		{
			throw new NotImplementedException();
		}

		public PropertyDescriptor GetDefaultProperty()
		{
			throw new NotImplementedException();
		}

		public object GetEditor(Type editorBaseType)
		{
			throw new NotImplementedException();
		}

		public EventDescriptorCollection GetEvents(Attribute[] attributes)
		{
			throw new NotImplementedException();
		}

		public EventDescriptorCollection GetEvents()
		{
			throw new NotImplementedException();
		}

		public PropertyDescriptorCollection GetProperties(Attribute[] attributes)
		{
			throw new NotImplementedException();
		}

		public PropertyDescriptorCollection GetProperties()
		{
			var props = new PropertyDescriptorCollection(null);
			foreach (var kvp in _cache)
			{
				props.Add(new CustomPropertyDescriptor(kvp.Key, _cache));
			}
			return props;
		}

		public object GetPropertyOwner(PropertyDescriptor pd)
		{
			throw new NotImplementedException();
		}
	}
}
