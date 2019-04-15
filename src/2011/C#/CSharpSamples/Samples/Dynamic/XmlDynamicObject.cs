using System;
using System.Collections.Generic;
using System.Dynamic;
using System.Linq;
using System.Linq.Expressions;
using System.Runtime.CompilerServices;
using System.Xml.Linq;

namespace Samples.Dynamic
{
	class XmlDynamicObject : DynamicObject
	{
		private object _value;
		private XElement _element;

		private XmlDynamicObject(object value)
		{
			_value = value;
		}

		public XmlDynamicObject(XElement element)
		{
			_element = element;
		}

		public override bool TryGetMember(GetMemberBinder binder, out object result)
		{
			if (_element != null)
			{
				_value = _element.Element(binder.Name).Value;
				result = new XmlDynamicObject(_value);
			}
			else
			{
				_value = _value.GetType().GetProperty(binder.Name).GetValue(_value, null);
				result = _value;
			}
			return true;
		}
		public override bool TryInvokeMember(InvokeMemberBinder binder, object[] args, out object result)
		{
			Type[] types = args.Select(x => x.GetType()).ToArray();
			result = _value.GetType().GetMethod(binder.Name, types).Invoke(_value, args);
			return true;
		}

		#region operator overloads
		public static bool operator <(XmlDynamicObject lhs, object rhs)
		{
			return Compare(lhs._value, rhs) == -1;
		}
		public static bool operator >(XmlDynamicObject lhs, object rhs)
		{
			return Compare(lhs._value, rhs) == 1;
		}
		public static bool operator ==(XmlDynamicObject lhs, object rhs)
		{
			return Compare(lhs._value, rhs) == 0;
		}
		public static bool operator !=(XmlDynamicObject lhs, object rhs)
		{
			return Compare(lhs._value, rhs) != 0;
		}
		public static bool operator <=(XmlDynamicObject lhs, object rhs)
		{
			int ret = Compare(lhs._value, rhs);
			return (ret == -1 || ret == 0);
		}
		public static bool operator >=(XmlDynamicObject lhs, object rhs)
		{
			int ret = Compare(lhs._value, rhs);
			return (ret == 1 || ret == 0);
		}
		public static object operator +(XmlDynamicObject lhs, object rhs)
		{
			return Calculate(ExpressionType.Add, lhs._value, rhs);
		}
		public static object operator -(XmlDynamicObject lhs, object rhs)
		{
			return Calculate(ExpressionType.Subtract, lhs._value, rhs);
		}
		public static object operator *(XmlDynamicObject lhs, object rhs)
		{
			return Calculate(ExpressionType.Multiply, lhs._value, rhs);
		}
		public static object operator /(XmlDynamicObject lhs, object rhs)
		{
			return Calculate(ExpressionType.Divide, lhs._value, rhs);
		}
		public static object operator %(XmlDynamicObject lhs, object rhs)
		{
			return Calculate(ExpressionType.Modulo, lhs._value, rhs);
		}
		#endregion

		private static object Calculate(ExpressionType type, object lhs, object rhs)
		{
			object newLhs = lhs;
			if (lhs.GetType() != rhs.GetType())
			{
				newLhs = Convert.ChangeType(lhs, rhs.GetType());
			}

			Expression body = null;
			Expression left = Expression.Constant(newLhs);
			Expression right = Expression.Constant(rhs);
			switch (type)
			{
			case ExpressionType.Add:
				body = Expression.Add(left, right);
				break;
			case ExpressionType.Subtract:
				body = Expression.Subtract(left, right);
				break;
			case ExpressionType.Multiply:
				body = Expression.Multiply(left, right);
				break;
			case ExpressionType.Divide:
				body = Expression.Divide(left, right);
				break;
			case ExpressionType.Modulo:
				body = Expression.Modulo(left, right);
				break;
			default:
				throw new NotSupportedException(type + " is not supported.");
			}

			LambdaExpression exp = Expression.Lambda(body);
			Delegate method = exp.Compile();
			object result = method.DynamicInvoke();
			return result;
		}

		private static int Compare(object lhs, object rhs)
		{
			Type type = rhs.GetType();
			object newLhs = Convert.ChangeType(lhs, type);
			Type comparer = typeof(Comparer<>).MakeGenericType(type);
			object inst = comparer.GetProperty("Default").GetValue(comparer, null);
			int ret = (int)comparer.GetMethod("Compare").Invoke(inst, new object[] { newLhs, rhs });
			return ret;
		}

		public override string ToString()
		{
			return _element.ToString();
		}

		public override int GetHashCode()
		{
			return RuntimeHelpers.GetHashCode(this);
		}

		public override bool Equals(object obj)
		{
			var target = obj as XmlDynamicObject;
			if (target == null)
			{
				return false;
			}
			else if (this.GetHashCode() != target.GetHashCode())
			{
				return false;
			}
			return true;
		}
	}
}
