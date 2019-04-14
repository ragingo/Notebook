using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NPOI_test1
{
	class XlColumn
	{
		public int Sequence { get; set; }
		public string LogicalName { get; set; }
		public string PhysicalName { get; set; }
		public bool IsPrimaryKey { get; set; }
		public string DataType { get; set; }
		public int DataLength { get; set; }
		public bool IsRequired { get; set; }
		public object DefaultValue { get; set; }
	}
}
