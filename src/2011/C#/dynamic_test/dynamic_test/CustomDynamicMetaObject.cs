using System.Collections.Generic;
using System.Dynamic;
using System.Linq.Expressions;

namespace ConsoleApplication1
{
	class CustomDynamicMetaObject : DynamicMetaObject
	{
		private Dictionary<string, object> _cache = new Dictionary<string, object>();
		public CustomDynamicMetaObject(Expression parameter, object value, Dictionary<string, object> cache)
			: base(parameter, BindingRestrictions.Empty, value)
		{
			_cache = cache;
		}
		public override DynamicMetaObject BindGetMember(GetMemberBinder binder)
		{
			return Create();
		}
		public override DynamicMetaObject BindSetMember(SetMemberBinder binder, DynamicMetaObject value)
		{
			_cache[binder.Name] = value.Value;
			return Create();
		}
		public override DynamicMetaObject BindInvoke(InvokeBinder binder, DynamicMetaObject[] args)
		{
			return Create();
		}
		public override DynamicMetaObject BindInvokeMember(InvokeMemberBinder binder, DynamicMetaObject[] args)
		{
			return Create();
		}

		private DynamicMetaObject Create()
		{
			return
				new DynamicMetaObject(
					Expression,
					BindingRestrictions.GetTypeRestriction(
						Expression,
						typeof(CustomDynamicObject)
					)
				);
		}
	}
}
