using System;
using System.Collections.Generic;
using System.ComponentModel;

namespace ConsoleApplication1
{
	class CustomPropertyDescriptor : PropertyDescriptor
	{
		private Dictionary<string, object> _cache;
		public CustomPropertyDescriptor(string name, Dictionary<string, object> cache)
			: base(name, null)
		{
			_cache = cache;
		}
		public override bool CanResetValue(object component)
		{
			return false;
		}
		public override Type ComponentType
		{
			get { return typeof(CustomPropertyDescriptor); }
		}
		public override object GetValue(object component)
		{
			return _cache[Name];
		}
		public override bool IsReadOnly
		{
			get { throw new NotImplementedException(); }
		}
		public override Type PropertyType
		{
			get { return _cache[Name].GetType(); }
		}
		public override void ResetValue(object component)
		{
		}
		public override void SetValue(object component, object value)
		{
			_cache[Name] = value;
		}
		public override bool ShouldSerializeValue(object component)
		{
			return false;
		}
	}
}
