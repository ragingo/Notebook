using System;
using System.Windows;
using System.Windows.Data;
using System.Windows.Threading;
using WinForm = System.Windows.Forms;

namespace TaskManager
{
	public partial class MainWindow : Window
	{
		private DispatcherTimer _timer;
		private WinForm.NumericUpDown _updwnInterval;

		public MainWindow()
		{
			InitializeComponent();

			_updwnInterval = (WinForm.NumericUpDown)updwnInterval.Child;
			_updwnInterval.ValueChanged += NumericUpDown_ValueChanged;

			_timer = new DispatcherTimer();
			_timer.Interval = new TimeSpan(0, 0, (int)_updwnInterval.Value);
			_timer.Tick += delegate
			{
				(this.Resources["TaskList"] as ObjectDataProvider).Refresh();
			};
		}

		private void Window_Loaded(object sender, RoutedEventArgs e)
		{
			_timer.Start();
		}

		private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			_timer.Stop();
		}

		private void NumericUpDown_ValueChanged(object sender, EventArgs e)
		{
			_timer.Interval = new TimeSpan(0, 0, (int)_updwnInterval.Value);
		}
	}
}
