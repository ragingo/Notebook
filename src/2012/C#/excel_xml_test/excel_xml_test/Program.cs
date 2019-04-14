using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using System.Xml.XPath;
using System.IO;
using System.Xml;
using System.Diagnostics;

namespace ConsoleApplication1
{
	class Program
	{
		// てか実装に時間かかるから、NPIにするかな。。。
		static void Main(string[] args)
		{
			string filename = @"C:\Users\User1\Desktop\test1.xml";

			string xml = string.Join(Environment.NewLine, File.ReadLines(filename).Skip(2));
			var root = XDocument.Parse(xml);

			var book = root.Element(ExcelXmlConstants.NameSpace.SpreadSheet.Workbook);
			var styles =
				from style in
					book.Elements(ExcelXmlConstants.NameSpace.SpreadSheet.Styles)
						.Elements(ExcelXmlConstants.NameSpace.SpreadSheet.Style)
				let id = style.Attribute(ExcelXmlConstants.NameSpace.SpreadSheet.ID)
				let name = style.Attribute(ExcelXmlConstants.NameSpace.SpreadSheet.Name)
				let interior = style.Element(ExcelXmlConstants.NameSpace.SpreadSheet.Interior)
				let color = interior != null ? interior.Attribute(ExcelXmlConstants.NameSpace.SpreadSheet.Color) : null
				select new
				{
					Id = id != null ? id.Value : null,
					Name = name != null ? name.Value : null,
					Interior = new
					{
						Color = color != null ? color.Value : null
					}
				};

			foreach (var style in styles)
			{
				Console.WriteLine(style);
			}
		}
	}

	static class ExcelXmlConstants
	{
		public static class NameSpace
		{
			private const string Ss = "urn:schemas-microsoft-com:office:spreadsheet"; // ss
			private const string Excel = "urn:schemas-microsoft-com:office:excel"; // x
			private const string Office = "urn:schemas-microsoft-com:office:office"; // o

			public static class SpreadSheet
			{
				public static readonly string Workbook;
				public static readonly string Styles;
				public static readonly string Style;
				public static readonly string Interior;
				public static readonly string ID;
				public static readonly string Name;
				public static readonly string Color;
				static SpreadSheet()
				{
					Workbook = "{" + Ss + "}Workbook";
					Styles = "{" + Ss + "}Styles";
					Style = "{" + Ss + "}Style";
					Interior = "{" + Ss + "}Interior";
					ID = "{" + Ss + "}ID";
					Name = "{" + Ss + "}Name";
					Color = "{" + Ss + "}Color";
				}
			}
		}
	}
}
