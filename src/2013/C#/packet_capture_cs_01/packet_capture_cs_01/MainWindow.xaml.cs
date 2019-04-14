using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Net.Sockets;
using System.Net;
using System.Threading;
using System.Windows.Threading;
using System.Collections.ObjectModel;

namespace packet_capture_cs_01
{
	/// <summary>
	/// MainWindow.xaml の相互作用ロジック
	/// </summary>
	public partial class MainWindow : Window
	{
		private ObservableCollection<IPv4Header> _dataSource = new ObservableCollection<IPv4Header>();

		public MainWindow()
		{
			InitializeComponent();

			dataGrid1.ItemsSource = _dataSource;
		}

		private void button1_Click(object sender, RoutedEventArgs e)
		{
			Socket sock = new Socket(AddressFamily.InterNetwork, SocketType.Raw, ProtocolType.IP);
			sock.Bind(new IPEndPoint(IPAddress.Parse("192.168.1.22"), 0));
			sock.SetSocketOption(SocketOptionLevel.IP, SocketOptionName.HeaderIncluded, true);
			sock.IOControl(IOControlCode.ReceiveAll, new byte[] { 1, 0, 0, 0 }, new byte[] { 0, 0, 0, 0 });

			ThreadPool.QueueUserWorkItem(obj =>
			{
				while (true)
				{
					byte[] buf = new byte[4096];
					int len = sock.Receive(buf, SocketFlags.None);

					if (len <= 0)
					{
						continue;
					}


					dataGrid1.Dispatcher.BeginInvoke(
						(Action)(() => {
							IPv4Header header = new IPv4Header();
							header.Version = buf[0] >> 4;
							_dataSource.Add(header);
						}), DispatcherPriority.Loaded
					);
				}
			});
		}
	}

	class IPv4Header
	{
		public int Version { get; set; }
	}
}
