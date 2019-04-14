using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;

namespace Xsv
{
	public class XsvRow
	{
		private Dictionary<string, object> _values = new Dictionary<string, object>();

		public XsvTable Table
		{
			get;
			internal set;
		}

		public object this[string columnName]
		{
			get
			{
				return _values[columnName];
			}
			set
			{
				_values[columnName] = value;
			}
		}

		public object[] ItemArray
		{
			get
			{
				return _values.Values.ToArray();
			}
			set
			{
				int i = 0;
				string[] cols = _values.Keys.ToArray();
				foreach (string col in cols)
				{
					if (value != null && value.Length > 0)
					{
						_values[col] = value[i++];
					}
					else
					{
						_values[col] = null;
					}
				}
			}
		}

		[EditorBrowsable(EditorBrowsableState.Never)]
		internal XsvRow(XsvColumnCollection columns)
		{
			foreach (var col in columns)
			{
				_values.Add(col.Name, null);
			}
		}
	}
}
