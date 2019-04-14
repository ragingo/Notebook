using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ConsoleApplication1
{
	class ProviderMetadataComparer<T> : IEqualityComparer<ProviderMetadataCache<T>>
	{
		public bool Equals(ProviderMetadataCache<T> x, ProviderMetadataCache<T> y)
		{
			return x.GetHashCode() == y.GetHashCode();
		}

		public int GetHashCode(ProviderMetadataCache<T> obj)
		{
			return obj.GetHashCode();
		}
	}
}
