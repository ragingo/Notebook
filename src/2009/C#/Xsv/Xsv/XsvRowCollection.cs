using System.Collections.ObjectModel;

namespace Xsv
{
	public class XsvRowCollection : Collection<XsvRow>
	{
		public XsvTable Table
		{
			get;
			internal set;
		}

		public new void Add(XsvRow row)
		{
			base.Add(row);
		}

		public void Add(params object[] items)
		{
			XsvRow row = this.Table.NewRow();
			row.ItemArray = items;
			base.Add(row);
		}
	}
}
