using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Diagnostics.Eventing.Reader;
using System.Linq;
using System.Text;
using System.IO;
using System.Threading;

namespace ConsoleApplication1
{
	class Program
	{
		static void Main1(string[] args)
		{
			var logs = new List<SecurityLog>();
			var reader = new EventLogReader(new EventLogQuery("Security", PathType.LogName));

			Stopwatch sw = Stopwatch.StartNew();

			for (var evt = reader.ReadEvent(); evt != null; evt = reader.ReadEvent())
			{
				var rec = evt as EventLogRecord;
				var log = new SecurityLog(rec);
				logs.Add(log);
			}
			SecurityLog.ClearCache();

			sw.Stop();
			Console.WriteLine((sw.ElapsedMilliseconds / 1000.0) + "秒");
			Console.WriteLine(logs.Count + "件");
		}
		static void Main2(string[] args)
		{
			Stopwatch watch = Stopwatch.StartNew();
			using (var sw = new StreamWriter("d:\\logs\\" + DateTime.Now.ToString("yyyyMMddHHmmss") + ".csv"))
			{
				foreach (var line in GetData())
				{
					sw.WriteLine(line);
				}
			}
			watch.Stop();
			Console.WriteLine((watch.ElapsedMilliseconds / 1000.0) + "秒");
		}
		static void Main(string[] args)
		{
			Stopwatch watch = Stopwatch.StartNew();

			var lines = GetData2().ToList();
			using (var sw = new StreamWriter("d:\\logs\\" + DateTime.Now.ToString("yyyyMMddHHmmss") + ".csv"))
			{
				foreach (var line in lines)
				{
					sw.WriteLine(line);
				}
			}
			watch.Stop();
			Console.WriteLine((watch.ElapsedMilliseconds / 1000.0) + "秒");
		}
		static IEnumerable<SecurityLog> GetData()
		{
			var reader = new EventLogReader(new EventLogQuery("Security", PathType.LogName));
			for (var evt = reader.ReadEvent(); evt != null; evt = reader.ReadEvent())
			{
				var rec = evt as EventLogRecord;
				yield return new SecurityLog(rec);
			}
			SecurityLog.ClearCache();
		}
		static IEnumerable<SecurityLog> GetData2()
		{
			string[] xPathRef = new string[] { 
				"Event/System/Provider/@Name",
				"Event/System/TimeCreated/@SystemTime",
				"Event/System/Execution/@ProcessID",
				"Event/System/Task",
				"Event/System/Opcode",
				"Event/System/Keywords",
			};
			var reader = new EventLogReader(new EventLogQuery("Security", PathType.LogName));
			for (var evt = reader.ReadEvent(); evt != null; evt = reader.ReadEvent())
			{
				var rec = evt as EventLogRecord;
				yield return new SecurityLog(rec, xPathRef);
			}
			SecurityLog.ClearCache();
		}
	}
}

