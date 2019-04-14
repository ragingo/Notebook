using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WindowsFormsApplication1
{
	static class EnumerableExtensions
	{
		private static class IdentityFunction<TElement>
		{
			public static Func<TElement, TElement> Instance
			{
				get
				{
					return x => x;
				}
			}
		}

		public static KeyValuePair<TKey, TSource>[] ToKeyValuePairArray<TSource, TKey>(
			this IEnumerable<TSource> source,
			Func<TSource, TKey> keySelector)
		{
			return ToKeyValuePairArray<TSource, TKey, TSource>(source, keySelector, IdentityFunction<TSource>.Instance);
		}
		public static KeyValuePair<TKey, TValue>[] ToKeyValuePairArray<TSource, TKey, TValue>(
			this IEnumerable<TSource> source,
			Func<TSource, TKey> keySelector,
			Func<TSource, TValue> valueSelector)
		{
			if (source == null)
			{
				throw new ArgumentNullException("source");
			}
			if (keySelector == null)
			{
				throw new ArgumentNullException("keySelector");
			}
			if (valueSelector == null)
			{
				throw new ArgumentNullException("valueSelector");
			}
			return source.Select(x => new KeyValuePair<TKey, TValue>(keySelector(x), valueSelector(x))).ToArray();
		}
	}
}
