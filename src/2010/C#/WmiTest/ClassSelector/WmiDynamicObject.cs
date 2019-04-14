using System.Collections.Generic;
using System.Dynamic;
using System.Linq;
using System.Management;

namespace ClassSelector
{
	class WmiDynamicObject : DynamicObject
	{
		private ManagementBaseObject _instance;
		public WmiDynamicObject(ManagementBaseObject instance)
		{
			_instance = instance;
		}
		public override bool TryGetMember(GetMemberBinder binder, out object result)
		{
			result = _instance.Properties[binder.Name].Value;
			return true;
		}
		public override IEnumerable<string> GetDynamicMemberNames()
		{
			return _instance.Properties.OfType<PropertyData>().Select(x => x.Name);
		}
	}
}
