using System;
using System.Collections.Generic;
using System.Configuration;
using System.IO;
using System.Linq;
using System.Text;
using NPOI.HSSF.UserModel;
using NPOI.SS.UserModel;

namespace NPOI_test1
{
	class Program
	{
		private static readonly string OutputDirectory = ConfigurationManager.AppSettings["OutputDirectory"];
		private static readonly string InputFile = ConfigurationManager.AppSettings["InputFile"];

		static void Main(string[] args)
		{
			var book = new HSSFWorkbook(new FileStream(InputFile, FileMode.Open, FileAccess.Read, FileShare.ReadWrite));
			var dataSheet = book.GetSheetAt(0);

			string currentTableName = string.Empty;
			var tables = new Dictionary<string, List<XlRow>>();
			for (int rowIdx = 0; rowIdx < dataSheet.PhysicalNumberOfRows; rowIdx++)
			{
				var row = dataSheet.GetRow(rowIdx);
				var rowData = new XlRow(ReadRowData(row));

				// table row
				if (IsTableRowData(rowData))
				{
					string tableName = rowData[0];
					currentTableName = tableName;
					rowData.Type = XlRowType.Table;
					tables[tableName] = new List<XlRow>();
					tables[tableName].Add(rowData);

					// header row
					row = dataSheet.GetRow(++rowIdx);
					rowData = new XlRow(ReadRowData(row));
					rowData.Type = XlRowType.Header;
					tables[tableName].Add(rowData);
					for (int colIdx = 0; colIdx < rowData.Cells.Count; colIdx++)
					{
						var col = rowData.Cells[colIdx];
						col.Name = col.Value;
					}
				}
				// data row
				else
				{
					// not (i or u or d) 
					if (string.IsNullOrEmpty(rowData.Cells.First().Value))
					{
						continue;
					}
					rowData.Type = XlRowType.Data;
					tables[currentTableName].Add(rowData);
					// header row
					var header = tables[currentTableName].Single(x => x.Type == XlRowType.Header);
					for (int colIdx = 0; colIdx < header.Cells.Count; colIdx++)
					{
						rowData.Cells[colIdx].Name = header.Cells[colIdx].Name;
					}
				}
			}

			foreach (var table in tables)
			{
				Console.WriteLine("----------------- table : {0} -----------------", table.Key);
				// read schema
				var schemaSheet = book.GetSheet(table.Key);
				var schemaData = new List<XlColumn>();
				for (int schemaRowIdx = 2; schemaRowIdx < schemaSheet.PhysicalNumberOfRows; schemaRowIdx++)
				{
					var row = schemaSheet.GetRow(schemaRowIdx);
					var rowData = new XlRow(ReadRowData(row));
					rowData.Type = XlRowType.Data;
					var col = new XlColumn();
					col.Sequence = Convert.ToInt32(rowData.Cells[0].Value);
					col.PhysicalName = rowData.Cells[2].Value;
					col.IsPrimaryKey = !string.IsNullOrEmpty(rowData.Cells[3].Value);
					col.DataType = rowData.Cells[4].Value;
					col.IsRequired = !string.IsNullOrEmpty(rowData.Cells[8].Value);
					col.DefaultValue = rowData.Cells[9].Value;
					schemaData.Add(col);
				}

				foreach (var row in table.Value.Where(x => x.Type == XlRowType.Data))
				{
					var ret = GenerateSql(table.Key, row, schemaData);
					string path = Path.Combine(OutputDirectory, table.Key + "_" + ret.Item1 + ".sql");
					File.AppendAllText(path, ret.Item2 + Environment.NewLine, Encoding.GetEncoding("Shift-JIS"));
					Console.WriteLine(ret.Item2);
				}
			}
		}

		private static Tuple<string, string> GenerateSql(string table, XlRow row, List<XlColumn> schema)
		{
			string kind = "";
			var query = new StringBuilder();
			switch (row.Cells[0].Value)
			{
			case "A":
				{
					kind = "ins";
					string values =
						string.Join(", ",
							row.Cells
								.Where(x => !string.IsNullOrEmpty(x.Name))
								.OrderBy(x => schema.Single(y => y.PhysicalName == x.Name).Sequence)
								.Select(x => GetXlCellValue(x, schema)));
					query.AppendFormat("insert into {0} values({1});", table, values);
				}
				break;
			case "U":
				{
					kind = "upd";
					string changeFields =
						string.Join(", ",
							from cell in row.Cells
							where !string.IsNullOrEmpty(cell.Name)
							where cell.IsChanged
							select cell.Name + " = " + GetXlCellValue(cell, schema)
						);
					string filterFields =
						string.Join(" and ",
							from cell in row.Cells
							where !string.IsNullOrEmpty(cell.Name)
							where schema.Single(x => x.PhysicalName == cell.Name).IsPrimaryKey
							select cell.Name + " = " + GetXlCellValue(cell, schema)
						);
					query.AppendFormat("update {0} set {1} where {2};", table, changeFields, filterFields);
				}
				break;
			case "D":
				{
					kind = "del";
					string filterFields =
						string.Join(" and ",
							from cell in row.Cells
							where !string.IsNullOrEmpty(cell.Name)
							where schema.Single(x => x.PhysicalName == cell.Name).IsPrimaryKey
							select cell.Name + " = " + GetXlCellValue(cell, schema)
						);
					query.AppendFormat("delete from {0} where {1};", table, filterFields);
				}
				break;
			}

			return Tuple.Create(kind, query.ToString());
		}

		static string GetStringCellValue(IRow row, int cellnum)
		{
			var cell = row.GetCell(cellnum);
			if (cell == null)
			{
				return string.Empty;
			}
			return cell.StringCellValue;
		}

		static string GetXlCellValue(XlCell cell, List<XlColumn> schema)
		{
			var col = schema.Single(x => x.PhysicalName == cell.Name);
			string value = "";
			switch (col.DataType.ToLower())
			{
			case "varchar2":
			case "nvarchar2":
			case "char":
			case "nchar":
			case "date":
			case "timestamp":
				value = "'" + cell.Value + "'";
				break;
			default:
				value = cell.Value;
				break;
			}
			return value;
		}

		static List<XlCell> ReadRowData(IRow row)
		{
			var cells = new List<XlCell>();
			for (int colIdx = 0; colIdx < 256; colIdx++)
			{
				var cell = row.GetCell(colIdx);
				if (cell == null)
				{
					if (colIdx > 0)
					{
						break;
					}
				}
				var c = new XlCell();
				c.Value = GetStringCellValue(row, colIdx);
				c.IsChanged = cell == null ? false : cell.CellStyle.FillForegroundColor == IndexedColors.YELLOW.Index;
				cells.Add(c);
			}
			return cells;
		}

		static bool IsTableRowData(XlRow row)
		{
			return
				!string.IsNullOrEmpty(row.Cells.First().Value) &&
				row.Cells.Skip(1).FirstOrDefault() == null;
		}
	}
}
