using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using log4net;
using log4net.Appender;

namespace Samples.Log
{
	static class Util
	{
		public static T GetAppender<T>() where T : FileAppender
		{
			return
				LogManager.GetRepository()
					.GetAppenders()
					.FirstOrDefault(x => x is T) as T;
		}
	}
}
