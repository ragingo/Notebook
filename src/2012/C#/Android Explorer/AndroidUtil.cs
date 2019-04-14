using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Configuration;

namespace Android_Explorer
{
	class AndroidSettings
	{
		public static readonly DirectoryInfo DefaultTemporaryDirectory;
		public DirectoryInfo SdkDirectory { get; set; }
		public DirectoryInfo TemporaryDirectory { get; set; }
		static AndroidSettings()
		{
			if (Directory.Exists(ConfigurationManager.AppSettings["DefaultTemporaryDirectory"]))
			{
				DefaultTemporaryDirectory = new DirectoryInfo(ConfigurationManager.AppSettings["DefaultTemporaryDirectory"]);
			}
		}
		public AndroidSettings()
		{
			TemporaryDirectory = DefaultTemporaryDirectory;
			if (Directory.Exists(ConfigurationManager.AppSettings["SdkDirectory"]))
			{
				SdkDirectory = new DirectoryInfo(ConfigurationManager.AppSettings["SdkDirectory"]);
			}
		}
	}

	class Adb
	{
		private static readonly Encoding DefaultEncoding = new UTF8Encoding(false);
		private const int EXECUTION_TIMEOUT = 100000;
		public readonly string ProgramPath;
		public AndroidSettings Settings { get; set; }

		public Adb()
		{
			string path = string.Empty;
			Settings = new AndroidSettings();
			if (Settings.SdkDirectory != null)
			{
				path = Settings.SdkDirectory.FullName;
			}
			ProgramPath = Path.Combine(path, "adb");
		}

		private ProcessStartInfo CreateStartInfo()
		{
			ProcessStartInfo info = new ProcessStartInfo();
			info.CreateNoWindow = true;
			info.RedirectStandardError = true;
			info.RedirectStandardInput = true;
			info.RedirectStandardOutput = true;
			info.StandardErrorEncoding = DefaultEncoding;
			info.StandardOutputEncoding = DefaultEncoding;
			info.WindowStyle = ProcessWindowStyle.Hidden;
			info.UseShellExecute = false;
			return info;
		}

		public IEnumerable<string> Execute(params string[] args)
		{
			Process proc = new Process();
			proc.StartInfo = CreateStartInfo();
			proc.StartInfo.FileName = ProgramPath;
			proc.StartInfo.Arguments = string.Join(" ", args);
			Debug.WriteLine(proc.StartInfo.FileName + " " + proc.StartInfo.Arguments);

			proc.Start();
			proc.WaitForExit(EXECUTION_TIMEOUT);

			List<string> result = new List<string>();
			while (!proc.StandardOutput.EndOfStream)
			{
				string line = proc.StandardOutput.ReadLine();
				if (string.IsNullOrEmpty(line))
				{
					continue;
				}
				result.Add(line);
				Debug.WriteLine(line);
			}
			return result;
		}

		public IEnumerable<string> InvokeShell(string device, params string[] args)
		{
			return Execute(new[] { "-s", device, "shell" }.Concat(args).ToArray());
		}

		public IEnumerable<string> GetDevices()
		{
			return Execute(new[] { "devices" });
		}

		public void Download(string device, params string[] args)
		{
			string src = "\"" + args[0] + "\"";
			string dst = "\"" + args[1] + "\"";
			Execute("-s", device,"pull", src, dst);
			return;
		}
	}
}
