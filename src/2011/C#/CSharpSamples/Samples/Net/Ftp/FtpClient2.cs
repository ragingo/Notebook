using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Net;
using System.Text.RegularExpressions;
using System.Threading;

namespace Samples.Net.Ftp
{
	class FtpClient2
	{
		public string HostName
		{
			get;
			private set;
		}
		public string UserName
		{
			get;
			private set;
		}
		public string Password
		{
			get;
			private set;
		}
		private Socket _socket;
		private static readonly Encoding DefaultEncoding = Encoding.GetEncoding("Shift_JIS");

		public FtpClient2(string hostName, string userName, string password, int port = 21)
		{
			HostName = hostName;
			UserName = userName;
			Password = password;
			_socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
			_socket.Connect(hostName, port);
			Console.WriteLine(ToText(Receive(10)));

			Send("USER", userName);
			Console.WriteLine(ToText(Receive(10)));
			Send("PASS", password);
			Console.WriteLine(ToText(Receive(10)));
		}

		private int Send(string command, params string[] parameters)
		{
			string cmd = command + " " + string.Join(" ", parameters) + "\n";
			Console.WriteLine(cmd.Trim('\r', '\n'));
			byte[] buf = DefaultEncoding.GetBytes(cmd);
			return _socket.Send(buf);
		}

		private IEnumerable<byte[]> Receive(int milliSecondsWait = 0)
		{
			const int BUF_SIZE = 1024;
			List<byte[]> list = new List<byte[]>();
			byte[] buf = new byte[BUF_SIZE];
			int readBytes;

			while (true)
			{
				if (milliSecondsWait != 0)
				{
					Thread.Sleep(milliSecondsWait);
				}
				readBytes = _socket.Receive(buf);
				if (readBytes == 0)
				{
					break;
				}
				list.Add(buf);
				if (readBytes < BUF_SIZE)
				{
					break;
				}
			}
			return list;
		}

		private void DiscardResponse(IEnumerable<byte[]> receiceData)
		{
			receiceData.ToList();
		}

		private string ToText(IEnumerable<byte[]> receiveData)
		{
			List<byte> list = new List<byte>();
			foreach (var d in receiveData)
			{
				list.AddRange(d);
			}
			return DefaultEncoding.GetString(list.ToArray()).Trim('\0', ' ', '\r', '\n');
		}

		public string ChangeDirectory(string path)
		{
			Send("CWD", path);
			return ToText(Receive());
		}

		public string PrintWorkingDirectory()
		{
			Send("PWD");
			return ToText(Receive());
		}

		public string GetListDirectory()
		{
			Send("NLST");
			return ToText(Receive());
		}

		public string GetListDirectoryDetails()
		{
			Send("LIST");
			return ToText(Receive());
		}

		public void SetPassiveMode()
		{
			Send("PASV");
			DiscardResponse(Receive());
		}
	}
}
