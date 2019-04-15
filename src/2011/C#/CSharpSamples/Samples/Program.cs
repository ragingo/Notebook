
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Diagnostics.Eventing.Reader;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using log4net;
using log4net.Appender;
using Samples.DirectoryService;
using Samples.Log;
using Samples.Xml;
using Samples.Dynamic;

namespace Samples
{
	class Program
	{
		static void Main(string[] args)
		{
			//GlobalContext.Properties["LogName"] = "test";
			//ILog logger = LogManager.GetLogger("NormalAppender");

			//if (Samples.Log.Util.GetAppender<CustomRollingFileAppender>().Exception != null)
			//{
			//    Console.WriteLine("open error!");
			//    return;
			//}

			//int pid = Process.GetCurrentProcess().Id;
			//string fmt = "Process : {0}, Value : {1}";
			//for (int i = 0; i < 1000; i++)
			//{
			//    logger.Info(string.Format(fmt, pid, i));
			//    Thread.Sleep(100);
			//}

			//LocalSam.ShowUsers1();
			//LocalSam.ShowGroups1();
			//Console.WriteLine("-----------------------------------------------");
			//LocalSam.ShowUsers2();
			//LocalSam.ShowGroups2();

			//XPathTester.Test();

			//XmlTest.Test1();

		}
	}
}
