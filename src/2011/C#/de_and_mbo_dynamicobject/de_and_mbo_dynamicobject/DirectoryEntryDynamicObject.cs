using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Dynamic;
using System.DirectoryServices;

namespace ConsoleApplication1
{
	class DirectoryEntryDynamicObject : DynamicObject
	{
		private DirectoryEntry _container = null;
		public DirectoryEntryDynamicObject(DirectoryEntry container)
		{
			_container = container;
		}
		public override bool TryGetMember(GetMemberBinder binder, out object result)
		{
			result = _container.InvokeGet(binder.Name);
			return true;
		}
	}

	static class DirectoryEntryExtension
	{
		public static dynamic ToDynamicObject(this DirectoryEntry container)
		{
			return new DirectoryEntryDynamicObject(container);
		}
	}

}
