using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ConsoleApplication1
{
	class ProviderMetadataCache<T>
	{
		public string ProviderName
		{
			get;
			private set;
		}
		public T Value
		{
			get;
			private set;
		}
		public ProviderMetadataCache(string providerName, T value)
		{
			ProviderName = providerName;
			Value = value;
		}
		public override int GetHashCode()
		{
			return this.ToString().GetHashCode();
		}
		public override string ToString()
		{
			StringBuilder sb = new StringBuilder();
			sb.Append(ProviderName);
			sb.Append(Value);
			return sb.ToString();
		}
	}
}
