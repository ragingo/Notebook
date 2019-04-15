using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;

namespace Samples.Util.Extensions
{
	public static class BitArrayExtensions
	{
		public static void SetBoolean(this BitArray source, bool value)
		{
			source.SetAll(false);
			source.Set(0, value);
		}
		public static void SetInt32(this BitArray source, int value)
		{
			var b = (Convert.ToString(value, 2) as IEnumerable<char>)
						.Select(x => Convert.ToBoolean(Convert.ToInt32(x.ToString())))
						.Reverse()
						.ToArray();

			source.SetAll(false);
			for (int i = 0; i < b.Length; i++)
			{
				source.Set(i, b[i]);
			}
		}
		public static void SetByte(this BitArray source, byte value)
		{
			var b = (Convert.ToString(value, 2) as IEnumerable<char>)
						.Select(x => Convert.ToBoolean(Convert.ToByte(x.ToString())))
						.Reverse()
						.ToArray();

			source.SetAll(false);
			for (int i = 0; i < b.Length; i++)
			{
				source.Set(i, b[i]);
			}
		}
		public static bool ToBoolean(this BitArray source)
		{
			var value = new bool[1];
			source.SetBits(source.GetBits().ToArray());
			source.CopyTo(value, 0);
			return value[0];
		}
		public static int ToInt32(this BitArray source)
		{
			var value = new int[1];
			source.SetBits(source.GetBits().ToArray());
			source.CopyTo(value, 0);
			return value[0];
		}
		public static byte ToByte(this BitArray source)
		{
			var value = new byte[1];
			source.SetBits(source.GetBits().ToArray());
			source.CopyTo(value, 0);
			return value[0];
		}
		public static IEnumerable<byte> GetBits(this BitArray source)
		{
			var result =
				from bit in source.Cast<bool>()
				select Convert.ToByte(bit);
			return result;
		}
		public static void SetBits(this BitArray source, bool[] array)
		{
			if (source.Count != array.Length)
			{
				throw new ArgumentException("配列の長さは同じでなければなりません。");
			}
			for (int i = 0; i < source.Count; i++)
			{
				source.Set(i, array[i]);
			}
		}
		public static void SetBits(this BitArray source, byte[] array)
		{
			if (source.Count != array.Length)
			{
				throw new ArgumentException("配列の長さは同じでなければなりません。");
			}
			for (int i = 0; i < source.Count; i++)
			{
				source.Set(i, Convert.ToBoolean(array[i]));
			}
		}
		public static void SetBits(this BitArray source, BitArray[] array)
		{
			var bits =
				(from x in array
				 from y in x.GetBits().Select(b => Convert.ToBoolean(b))
				 select y).ToArray();

			source.SetBits(bits);
		}
	}
}
