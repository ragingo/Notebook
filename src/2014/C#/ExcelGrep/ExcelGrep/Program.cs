using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace ExcelGrep
{
	class Program
	{
		static void Main(string[] args)
		{
			//string fileName = args[0];

			string fileName = @"D:\temp\a.xlsx";
			if (Path.GetExtension(fileName) != ".xls" &&
				Path.GetExtension(fileName) != ".xlsm" &&
				Path.GetExtension(fileName) != ".xlsx")
			{
				return;
			}

			Regex regex = new Regex(@"\d+");

			dynamic xlApp = Activator.CreateInstance(Type.GetTypeFromProgID("Excel.Application"));
			xlApp.DisplayAlerts = false;
			xlApp.ScreenUpdating = false;
			xlApp.Visible = false;

			dynamic xlBooks = xlApp.Workbooks;
			dynamic xlBook = xlBooks.Open(fileName);
			dynamic xlSheets = xlBook.Sheets;
			dynamic xlSheet = xlSheets[1];

			dynamic xlRange1 = xlSheet.Range("A1:ZZ10000");
			dynamic xlArray = xlRange1.Value;

			for (int row = 1; row <= 10000; row++)
			{
				for (int col = 1; col <= 100; col++)
				{
					dynamic value = xlArray[row, col] ?? string.Empty;
					if (regex.IsMatch(Convert.ToString(value)))
					{
						Console.WriteLine("{0} Row:{1}, Col:{2}", xlSheet.Name, row, col);
					}
				}
			}

			xlBook.Close();
			xlApp.Quit();
		}
	}
}
