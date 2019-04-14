using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NPOI_test1
{
	class XlRow
	{
		public XlRowType Type { get; set; }
		public List<XlCell> Cells { get; private set; }
		public string this[int columnIndex]
		{
			get
			{
				return Cells[columnIndex].Value;
			}
		}
		public XlRow(List<XlCell> cells)
		{
			Cells = new List<XlCell>(cells);
		}
	}
}
