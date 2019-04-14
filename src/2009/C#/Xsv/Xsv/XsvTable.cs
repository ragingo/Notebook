using System.IO;
using System.Text;

namespace Xsv
{
	public class XsvTable
	{
		public XsvColumnCollection Columns
		{
			get;
			set;
		}

		public XsvRowCollection Rows
		{
			get;
			set;
		}

		public XsvTable()
		{
			this.Columns = new XsvColumnCollection();
			this.Rows = new XsvRowCollection();
			this.Columns.Table = this;
			this.Rows.Table = this;
		}

		public XsvRow NewRow()
		{
			return new XsvRow(this.Columns);
		}

		public void Load(string path, char separator, bool headerIncluded)
		{
			this.Load(path, separator, headerIncluded, Encoding.Default);
		}

		public void Load(string path, char separator, bool headerIncluded, Encoding encoding)
		{
			string[] lines = File.ReadAllLines(path, encoding);

			for (int row = 0; row < lines.Length; row++)
			{
				string[] cols = lines[row].Split(separator);
				if (row == 0)
				{
					if (headerIncluded)
					{
						foreach (string col in cols)
						{
							this.Columns.Add(col);
						}
					}
					else
					{
						for (int col = 0; col < cols.Length; col++)
						{
							this.Columns.Add(col.ToString());
						}
					}
				}
				this.Rows.Add(cols);
			}
		}
	}
}
