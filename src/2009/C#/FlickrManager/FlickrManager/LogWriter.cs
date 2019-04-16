using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;

namespace FlickrManager
{
	class LogWriter
	{
		private static TraceSource _instance = new TraceSource("MainTraceSource");
		public static TraceSource Instance
		{
			get
			{
				return _instance;
			}
		}
	}
}
