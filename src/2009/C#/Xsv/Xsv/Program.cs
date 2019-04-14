using System;
using System.Text;

namespace Xsv
{
	class Program
	{
		static void Main(string[] args)
		{
			XsvTable table = new XsvTable();
			table.Load(@"d:\dummy.csv", ',', true, Encoding.GetEncoding(932));

			foreach (var row in table.Rows)
			{
				foreach (var col in table.Columns)
				{
					Console.Write(row[col.Name]);
					Console.Write(", ");
				}
				Console.WriteLine();
			}
		}
	}
}
