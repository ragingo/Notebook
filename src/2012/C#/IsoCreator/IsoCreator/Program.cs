using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace Sakura.Iso
{
	class Program
	{
		static void Main(string[] args)
		{
			var paramList = new Dictionary<string, string>();

			for (int i=0; i < args.Length; ++i)
			{
				switch (args[i])
				{
				case "-i":
				case "/i":
					if (i + 1 >= args.Length || string.IsNullOrEmpty(args[i + 1]))
					{
						Console.WriteLine("エラー : {0}", args[i] + "のパラメータが設定されていない");
						return;
					}
					paramList["-i"] = args[++i];
					break;

				case "-o":
				case "/o":
					if (i + 1 >= args.Length || string.IsNullOrEmpty(args[i + 1]))
					{
						Console.WriteLine("エラー : {0}", args[i] + "のパラメータが設定されていない");
						return;
					}
					paramList["-o"] = args[++i];
					break;

				case "-v":
				case "/v":
					if (i + 1 >= args.Length || string.IsNullOrEmpty(args[i + 1]))
					{
						Console.WriteLine("エラー : {0}", args[i] + "のパラメータが設定されていない");
						return;
					}
					paramList["-v"] = args[++i];
					break;

				case "-l":
				case "/l":
					if (i + 1 >= args.Length || string.IsNullOrEmpty(args[i + 1]))
					{
						Console.WriteLine("エラー : {0}", args[i] + "のパラメータが設定されていない");
						return;
					}
					paramList["-l"] = args[++i];
					break;
				}
			}

			Func<string, string> getParamValue = (string key) =>
			{
				if (paramList.ContainsKey(key))
				{
					return paramList[key];
				}
				return "";
			};

			string inputPath = getParamValue("-i");
			string outputFileName = getParamValue("-o");
			string volumeName = getParamValue("-v");
			string listFileName = getParamValue("-l");

			var iso = new IsoCreator();
			if (!string.IsNullOrEmpty(volumeName))
			{
				iso.VolumeName = volumeName;
			}
			// -iを優先
			if (string.IsNullOrEmpty(inputPath) &&
				!string.IsNullOrEmpty(listFileName) &&
				File.Exists(listFileName))
			{
				var lines = File.ReadAllLines(listFileName);
				if (lines.Length == 0)
				{
					Environment.Exit(-1);
					return;
				}
				iso.RootDirectory = lines[0];
				iso.AddItems(lines.Skip(1));
				File.Delete(listFileName);
			}
			else if (Directory.Exists(inputPath))
			{
				iso.RootDirectory = inputPath;
				iso.AddItems(Directory.EnumerateFileSystemEntries(inputPath, "*.*", SearchOption.AllDirectories));
			}
			else if(File.Exists(inputPath))
			{
				iso.RootDirectory = Path.GetDirectoryName(inputPath);
				iso.AddItem(inputPath);
			}
			if (string.IsNullOrEmpty(outputFileName))
			{
				outputFileName = Guid.NewGuid().ToString() + ".iso";
			}
			iso.Save(File.Create(outputFileName));
		}
	}
}
