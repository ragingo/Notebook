using System;

namespace Xsv
{
	public class XsvColumn
	{
		public XsvTable Table
		{
			get;
			internal set;
		}

		public Type DataType
		{
			get;
			internal set;
		}

		public string Name
		{
			get;
			internal set;
		}

		public string Format
		{
			get;
			internal set;
		}

		public XsvColumn(string name) : this(name, typeof(string))
		{
		}

		public XsvColumn(string name, Type type) : this(name, type, string.Empty)
		{
		}

		public XsvColumn(string name, Type type, string format)
		{
			this.Name = name;
			this.DataType = type;
			this.Format = format;
		}
	}
}
