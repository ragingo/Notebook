using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Common
{
	public class Util
	{
		public static int GetAge()
		{
			return (DateTime.Now - (new DateTime(1987, 10, 8))).Days / 365;
		}
	}
}
