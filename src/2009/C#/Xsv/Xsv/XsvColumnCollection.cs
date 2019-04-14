using System;
using System.Collections.ObjectModel;

namespace Xsv
{
	public class XsvColumnCollection : Collection<XsvColumn>
	{
		public XsvTable Table
		{
			get;
			internal set;
		}

		public void Add(string name)
		{
			XsvColumn column = new XsvColumn(name);
			column.Table = this.Table;
			base.Add(column);
		}

		public void Add(string name, Type type)
		{
			XsvColumn column = new XsvColumn(name, type);
			column.Table = this.Table;
			base.Add(column);
		}

		public void Add(string name, Type type, string format)
		{
			XsvColumn column = new XsvColumn(name, type, format);
			column.Table = this.Table;
			base.Add(column);
		}
	}
}
