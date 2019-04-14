using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Windows.Controls;
using System.Windows.Media.Imaging;
using Drawing = System.Drawing;

namespace TaskManager
{
	public class TaskInfo
	{
		public Image ApplicationIcon
		{
			get;
			set;
		}

		public Process Process
		{
			get;
			set;
		}

		public IEnumerable<TaskInfo> GetData(string machineName)
		{
			foreach (var p in Process.GetProcesses(machineName))
			{
				TaskInfo ti = new TaskInfo();
				ti.Process = p;

				if (!string.IsNullOrEmpty(p.MainWindowTitle))
				{
					ti.ApplicationIcon = GetApplicationIcon(p.MainModule.FileName);
				}

				yield return ti;
			}
		}

		public IEnumerable<TaskInfo> GetData()
		{
			return GetData(Environment.MachineName);
		}

		private Image GetApplicationIcon(string path)
		{
			using (var ms = new MemoryStream())
			{
				Drawing.Icon.ExtractAssociatedIcon(path).Save(ms);

				BitmapImage bmp = new BitmapImage();
				bmp.BeginInit();
				bmp.StreamSource = ms;
				bmp.EndInit();

				Image img = new Image();
				img.Width = 16;
				img.Height = 16;
				img.Source = bmp;

				return img;
			}
		}
	}
}
