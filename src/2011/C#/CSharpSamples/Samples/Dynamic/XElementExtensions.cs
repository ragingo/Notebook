using System.Xml.Linq;

namespace Samples.Dynamic
{
	static class XElementExtensions
	{
		public static dynamic ToDynamic(this XElement element)
		{
			return new XmlDynamicObject(element);
		}
	}
}
