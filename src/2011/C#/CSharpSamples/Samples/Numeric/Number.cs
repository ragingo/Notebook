using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Samples.Numeric
{
	[Serializable]
	public struct Number : IEquatable<Number>, IComparable<Number>, IFormattable
	{
		private object _value;
		public object Value
		{
			get
			{
				return _value;
			}
		}
		public Number(sbyte value)
		{
			_value = value;
		}
		public Number(byte value)
		{
			_value = value;
		}
		public Number(short value)
		{
			_value = value;
		}
		public Number(ushort value)
		{
			_value = value;
		}
		public Number(int value)
		{
			_value = value;
		}
		public Number(uint value)
		{
			_value = value;
		}
		public Number(long value)
		{
			_value = value;
		}
		public Number(ulong value)
		{
			_value = value;
		}
		public Number(float value)
		{
			_value = value;
		}
		public Number(double value)
		{
			_value = value;
		}
		public Number(decimal value)
		{
			_value = value;
		}
		public Number(Number value)
		{
			_value = value._value;
		}

		public static implicit operator Number(sbyte value)
		{
			return new Number(value);
		}
		public static implicit operator sbyte(Number value)
		{
			return (sbyte)value._value;
		}
		public static implicit operator Number(byte value)
		{
			return new Number(value);
		}
		public static implicit operator byte(Number value)
		{
			return (byte)value._value;
		}
		public static implicit operator Number(short value)
		{
			return new Number(value);
		}
		public static implicit operator short(Number value)
		{
			return (short)value._value;
		}
		public static implicit operator Number(ushort value)
		{
			return new Number(value);
		}
		public static implicit operator ushort(Number value)
		{
			return (ushort)value._value;
		}
		public static implicit operator Number(int value)
		{
			return new Number(value);
		}
		public static implicit operator int(Number value)
		{
			return (int)value._value;
		}
		public static implicit operator Number(uint value)
		{
			return new Number(value);
		}
		public static implicit operator uint(Number value)
		{
			return (uint)value._value;
		}
		public static implicit operator Number(long value)
		{
			return new Number(value);
		}
		public static implicit operator long(Number value)
		{
			return (long)value._value;
		}
		public static implicit operator Number(ulong value)
		{
			return new Number(value);
		}
		public static implicit operator ulong(Number value)
		{
			return (ulong)value._value;
		}
		public static implicit operator Number(float value)
		{
			return new Number(value);
		}
		public static implicit operator float(Number value)
		{
			return (float)value._value;
		}
		public static implicit operator Number(double value)
		{
			return new Number(value);
		}
		public static implicit operator double(Number value)
		{
			return (double)value._value;
		}
		public static implicit operator Number(decimal value)
		{
			return new Number(value);
		}
		public static implicit operator decimal(Number value)
		{
			return (decimal)value._value;
		}

		public override string ToString()
		{
			return _value.ToString();
		}
		public string ToString(string format, IFormatProvider formatProvider)
		{
			return (_value as IFormattable).ToString(format, formatProvider);
		}
		public override int GetHashCode()
		{
			return _value.GetHashCode();
		}
		public override bool Equals(object obj)
		{
			IConvertible conv = obj as IConvertible;
			if (conv == null)
			{
				return false;
			}
			switch (conv.GetTypeCode())
			{
			case TypeCode.Byte:
				return this.Equals((byte)obj);
			case TypeCode.Decimal:
				return this.Equals((decimal)obj);
			case TypeCode.Double:
				return this.Equals((double)obj);
			case TypeCode.Int16:
				return this.Equals((short)obj);
			case TypeCode.Int32:
				return this.Equals((int)obj);
			case TypeCode.Int64:
				return this.Equals((long)obj);
			case TypeCode.SByte:
				return this.Equals((sbyte)obj);
			case TypeCode.Single:
				return this.Equals((float)obj);
			case TypeCode.UInt16:
				return this.Equals((ushort)obj);
			case TypeCode.UInt32:
				return this.Equals((uint)obj);
			case TypeCode.UInt64:
				return this.Equals((ulong)obj);
			}
			return false;
		}
		public bool Equals(Number other)
		{
			if (_value.GetType() != other._value.GetType())
			{
				return false;
			}
			if (!_value.Equals(other._value))
			{
				return false;
			}
			return true;
		}
		public int CompareTo(Number other)
		{
			return (_value as IComparable).CompareTo(other._value);
		}
	}
}
