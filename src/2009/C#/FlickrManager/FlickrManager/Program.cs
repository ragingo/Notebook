using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.Diagnostics;

namespace FlickrManager
{
	static class Program
	{
		/// <summary>
		/// アプリケーションのメイン エントリ ポイントです。
		/// </summary>
		[STAThread]
		static void Main()
		{
			TraceSource ts = new TraceSource("MainTraceSource");
			ts.TraceEvent(TraceEventType.Information, 5, "Informational Event Occurred.");
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(new MainForm());
			ts.Flush();
			ts.Close();
		}
	}
}
