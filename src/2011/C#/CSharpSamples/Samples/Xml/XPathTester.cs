using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using System.Xml.XPath;
using System.IO;

namespace Samples.Xml
{
	class XPathTester
	{
		private static string Test(string doc, string query)
		{
			return
				XDocument.Parse(doc, LoadOptions.None)
					.XPathSelectElements(query)
					.Select(x => x.Value)
					.Aggregate((x, y) => x += y);
		}

		public static void Test()
		{
			string doc = File.ReadAllText("Xml\\doc.txt");
			string query = File.ReadAllText("Xml\\query.txt");
			Console.WriteLine("doc:");
			Console.WriteLine(doc);
			Console.WriteLine("-".PadLeft(100, '-'));
			Console.WriteLine("query:");
			Console.WriteLine(query);
			Console.WriteLine("-".PadLeft(100, '-'));
			Console.WriteLine("result:");
			Console.WriteLine(Test(doc, query));
			Console.WriteLine("-".PadLeft(100, '-'));
		}
	}
}
