using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NPOI_test1
{
	class XlCell
	{
		public string Name { get; set; }
		public string Value { get; set; }
		public bool IsChanged { get; set; }
		public XlColumn Column { get; set; }
	}
}
