using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using System.Xml.XPath;

namespace Samples.Dynamic
{
	class XmlTest
	{
		public static void Test1()
		{
			DateTime d1 = DateTime.Now;

			var result =
				from elems in XDocument.Load(@".\Dynamic\person-informations.xml").XPathSelectElements("//personal-information")
				let d = elems.ToDynamic()
				where d.FullName.Length == 3
				select d;
				//where (int)elems.Element("Age") % 2 == 0
				//select elems;

			foreach (var row in result)
			{
				Console.WriteLine(row);
			}
			Console.WriteLine(result.Count() + "件ヒット！");

			DateTime d2 = DateTime.Now;

			Console.WriteLine((d2-d1).TotalSeconds + " sec");
		}
	}
}
