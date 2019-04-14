using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using log4net;
using Common;

namespace log4net_load_test
{
	class Program
	{
		static void Main(string[] args)
		{
			ILog log = LogManager.GetLogger("NormalAppender");

			CustomRollingFileAppender appender =
				LogManager.GetRepository()
					.GetAppenders()
					.First(x => x is CustomRollingFileAppender) as CustomRollingFileAppender;

			if (appender.Exception != null)
			{
				Console.WriteLine("has error!");
			}
			else
			{
				Console.WriteLine("normally.");
			}

			for (int i = 0; i < 10; i++)
			{
				log.Info(i);
			}

			Console.WriteLine("please, press any key.");
			Console.ReadKey();

			Console.WriteLine("age is " + Util.GetAge());
		}
	}
}
