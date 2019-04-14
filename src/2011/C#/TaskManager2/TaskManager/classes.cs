using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Linq;
using System.Management;
using System.Threading;
using System.Windows.Threading;

namespace SakuraUtils.TaskManager
{
	public class ApplicationInfo
	{
		public long PID { get; set; }
		public string Task { get; set; }
		public string State { get; set; }
	}

	public class ProcessInfo
	{
		public string ImageName { get; set; }
		public long PID { get; set; }
	}

	public class TaskManager
	{
		private Thread _thread;
		private readonly Dispatcher UIThread;
		private ObservableCollection<ApplicationInfo> _applications;
		public ReadOnlyObservableCollection<ApplicationInfo> Applications { get; set; }
		private ObservableCollection<ProcessInfo> _processes;
		public ReadOnlyObservableCollection<ProcessInfo> Processes { get; set; }
		private ProcessWatcher _watcher;

		public TaskManager()
		{
			UIThread = Dispatcher.CurrentDispatcher;
			_thread = new Thread(Callback);
			_thread.IsBackground = true;
			_thread.Start();
			_watcher = new ProcessWatcher();
			_watcher.ProcessStarted += new ProcessStateChangedEventHandler(_watcher_ProcessStarted);
			_watcher.ProcessStopped += new ProcessStateChangedEventHandler(_watcher_ProcessStopped);
			_watcher.Start();
			_applications = new ObservableCollection<ApplicationInfo>();
			_processes = new ObservableCollection<ProcessInfo>();
			Applications = new ReadOnlyObservableCollection<ApplicationInfo>(_applications);
			Processes = new ReadOnlyObservableCollection<ProcessInfo>(_processes);
		}

		private void _watcher_ProcessStarted(object sender, ProcessStateChangedArgs e)
		{
			try
			{
				var p = Process.GetProcessById(e.ProcessId);
				CreateInfo(p);
			}
			catch (ArgumentException)
			{

			}
		}

		private void _watcher_ProcessStopped(object sender, ProcessStateChangedArgs e)
		{
			RemoveInfo(_applications, x => x.PID == e.ProcessId);
		}

		private void RemoveInfo<T>(ICollection<T> data, Func<T, bool> pred)
		{
			data.ToList().ForEach(x =>
			{
				if (pred(x))
				{
					Invoke(()=> data.Remove(x));
				}
			});
		}

		private void CreateInfo(Process p)
		{
			// apps
			if (p.MainWindowTitle.Length > 0 && p.MainWindowHandle != IntPtr.Zero)
			{
				Invoke(() =>
				{
					if (_applications.Any(x => x.PID == p.Id)) { return; }
					var item = new ApplicationInfo();
					item.PID = p.Id;
					item.Task = p.MainWindowTitle;
					item.State = p.Responding ? "実行中" : "応答なし";
					_applications.Add(item);
				});
			}
			// procs
			else
			{
				Invoke(() =>
				{
					if (_processes.Any(x => x.PID == p.Id)) { return; }
					var item = new ProcessInfo();
					item.ImageName = p.ProcessName;
					item.PID = p.Id;
					_processes.Add(item);
				});
			}
		}

		private void Callback()
		{
			foreach (var p in Process.GetProcesses())
			{
				CreateInfo(p);
			}
			while (true)
			{
				Thread.Sleep(1000);
			}
		}

		private void Invoke(Action action)
		{
			UIThread.Invoke((Action)delegate{
				action();
			});
		}

		private void UpdateInfo()
		{
		}
	}

	public class ProcessStateChangedArgs : EventArgs
	{
		public int ProcessId
		{
			get;
			private set;
		}
		public ProcessStateChangedArgs(int processId)
		{
			ProcessId = processId;
		}
	}
	public delegate void ProcessStateChangedEventHandler(object sender, ProcessStateChangedArgs e); 

	public class ProcessWatcher
	{
		public event ProcessStateChangedEventHandler ProcessStarted;
		public event ProcessStateChangedEventHandler ProcessStopped;

		public ProcessWatcher()
		{
		}
		public void Start()
		{
			var startWatch = new ManagementEventWatcher(new WqlEventQuery("SELECT * FROM Win32_ProcessStartTrace"));
			startWatch.EventArrived += new EventArrivedEventHandler(startWatch_EventArrived);
			startWatch.Start();
			var stopWatch = new ManagementEventWatcher(new WqlEventQuery("SELECT * FROM Win32_ProcessStopTrace"));
			stopWatch.EventArrived += new EventArrivedEventHandler(stopWatch_EventArrived);
			stopWatch.Start();
			//startWatch.Stop();
			//stopWatch.Stop();
		}
		private void startWatch_EventArrived(object sender, EventArrivedEventArgs e)
		{
			if (ProcessStarted != null)
			{
				ProcessStarted(this, new ProcessStateChangedArgs((int)(uint)e.NewEvent.Properties["ProcessID"].Value));
			}
		}
		private void stopWatch_EventArrived(object sender, EventArrivedEventArgs e)
		{
			if (ProcessStopped != null)
			{
				ProcessStopped(this, new ProcessStateChangedArgs((int)(uint)e.NewEvent.Properties["ProcessID"].Value));
			}
		}
	}
}
