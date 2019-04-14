using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Net.Sockets;
using System.Text;

namespace PopClient_Test
{
	public class PopClient : IDisposable
	{
		private TcpClient pop;

		public PopClient()
		{
		}

		private string Receive(bool multiLine)
		{
			string msg = "";
			do
			{
				byte[] buff = new byte[pop.ReceiveBufferSize];
				pop.GetStream().Read(buff, 0, buff.Length);
				msg += Encoding.ASCII.GetString(buff).TrimEnd('\0');
			}
			while(pop.GetStream().DataAvailable || 
				  (!multiLine && !msg.EndsWith("\r\n")) || 
				  ( multiLine && !msg.EndsWith("\r\n.\r\n")));
			return msg;
		}

		private void Send(string command)
		{
			byte[] buff = Encoding.ASCII.GetBytes(command);
			pop.GetStream().Write(buff, 0, buff.Length);
		}

		public void Connect(string host, int port)
		{
			pop = new TcpClient(host, port);
			Debug.WriteLine(Receive(false));
		}

		public void Disconnect()
		{
			Send("QUIT\r\n");
			Debug.WriteLine(Receive(false));
			pop.Close();
		}

		public void Login(string userName, string password)
		{
			Send("USER " + userName + "\r\n");
			Debug.WriteLine(Receive(false));

			Send("PASS " + password + "\r\n");
			Debug.WriteLine(Receive(false));
		}

		public int GetMailCount()
		{
			Send("STAT\r\n");
			string stat = Receive(false);
			Debug.WriteLine(stat);

			int count = int.Parse(stat.Split(' ')[0]);
			return count;
		}

		public IEnumerable<string> GetMailUidls()
		{
			Send("UIDL\r\n");
			string uidlList = Receive(true);
			Debug.WriteLine(uidlList);

			uidlList = uidlList.TrimStart("+OK\r\n".ToCharArray());
			uidlList = uidlList.TrimEnd("\r\n.\r\n".ToCharArray());

			foreach(string uidl in uidlList.Split("\r\n".ToCharArray(), StringSplitOptions.RemoveEmptyEntries))
			{
				yield return uidl;
			}
		}

		public string GetMailHeader(int mailNumber)
		{
			Send("TOP " + mailNumber.ToString() + " 0\r\n");
			string mailHeader = Receive(true);
			Debug.WriteLine(mailHeader);

			return mailHeader;
		}

		public string GetMailBody(int mailNumber)
		{
			Send("RETR " + mailNumber.ToString() + "\r\n");
			string body = Receive(true);

			char[] trimChars = new char[] { '\r', '\n', '.' };
			string header = GetMailHeader(mailNumber).TrimEnd(trimChars);
			body = body.Replace(header, null);
			body = body.TrimStart(trimChars);
			body = body.TrimEnd(trimChars);
			Debug.WriteLine(body);

			byte[] text = Encoding.ASCII.GetBytes(body);
			return Encoding.GetEncoding("iso-2022-jp").GetString(text);
		}

		public void Dispose()
		{
			Disconnect();
		}
	}
}
