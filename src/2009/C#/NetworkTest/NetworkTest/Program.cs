using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.IO;
using NetworkTest.Network.IP;
using System.Text;
using NetworkTest.Network.Tcp;
using NetworkTest.Network.Udp;

namespace NetworkTest
{
	class Program
	{
		[DllImport("user32.dll")]
		private static extern int GetAsyncKeyState(int vKey);

		// http://www.picfun.com/lan00.html
		unsafe static void Main(string[] args)
		{
			using (Socket sock = new Socket(AddressFamily.InterNetwork, SocketType.Raw, ProtocolType.IP))
			{
				IPAddress address = Dns.GetHostAddresses(Dns.GetHostName())[1];
				int port = 0;
				sock.Bind(new IPEndPoint(address, port));
				sock.SetSocketOption(SocketOptionLevel.IP, SocketOptionName.HeaderIncluded, true);
				// Promiscuos Mode
				sock.IOControl(IOControlCode.ReceiveAll, new byte[] { 1, 0, 0, 0 }, new byte[] { 0, 0, 0, 0 });

				while (GetAsyncKeyState(0x20) == 0)
				{
					const int RECEIVE_SIZE = 65536;
					byte[] buff = new byte[RECEIVE_SIZE];
					int length = sock.Receive(buff, SocketFlags.None);

					IPHeaderHelper ipHeader = new IPHeaderHelper(buff);

					//if (ipHeader.RawHeader.SourceAddress != ipHeader.RawHeader.DestinationAddress)
					//{
					//    continue;
					//}
					//Console.WriteLine("Version                : " + ipHeader.Version);
					//Console.WriteLine("Length                 : " + ipHeader.HeaderLength);
					//Console.WriteLine("Precedence             : " + ipHeader.Precedence);
					//Console.WriteLine("Delay                  : " + ipHeader.Delay);
					//Console.WriteLine("Traffic                : " + ipHeader.Throughput);
					//Console.WriteLine("Reliability            : " + ipHeader.Reliability);
					//Console.WriteLine("TotalLength            : " + ipHeader.TotalLength);
					//Console.WriteLine("Identification         : " + ipHeader.Identification);
					//Console.WriteLine("IsFlagment             : " + ipHeader.IsFlagment);
					//Console.WriteLine("DontFragment           : " + ipHeader.DontFragment);
					//Console.WriteLine("MoreFragment           : " + ipHeader.MoreFragment);
					//Console.WriteLine("Offset                 : " + ipHeader.FlagmentOffset);
					//Console.WriteLine("TTL                    : " + ipHeader.Ttl);
					//Console.WriteLine("Protocol               : " + ipHeader.Protocol);
					//Console.WriteLine("Checksum               : " + ipHeader.Checksum);
					//Console.WriteLine("VersionAndHeaderLength : " + ipHeader.RawHeader.VersionAndHeaderLength);
					//Console.WriteLine("FlagsAndOffset         : " + ipHeader.RawHeader.FlagsAndOffset);
					//Console.WriteLine("TypeOfService          : " + ipHeader.RawHeader.TypeOfService);
					Console.WriteLine(ipHeader.SourceAddress + " --> " + ipHeader.DestinationAddress);
					try
					{
						// vista だから？レオパレスだから？めっちゃ遅い
						//Console.WriteLine(Dns.GetHostEntry(ipHeader.SourceAddress).HostName + " --> " + Dns.GetHostEntry(ipHeader.DestinationAddress).HostName);
					}
					catch (SocketException ex)
					{
						if (ex.SocketErrorCode == SocketError.HostNotFound)
						{
							Console.WriteLine("src od dest host not found...");
						}
					}

					// IP Header Option に TCP(or UDP) Header が 繋がっている考えで正しいか？
					{
						IPHeader h = ipHeader.RawHeader;
						int OPTION_SIZE = ipHeader.TotalLength - ipHeader.HeaderLength;
						byte[] option = new byte[OPTION_SIZE];
						Marshal.Copy(new IntPtr(h.Option), option, 0, OPTION_SIZE);

						switch (ipHeader.Protocol)
						{
						case InternetProtocolType.Tcp:
							{
								TcpHeaderHelper tcpHeader = new TcpHeaderHelper(option);
								Console.WriteLine(tcpHeader.SourcePort);
								Console.WriteLine(tcpHeader.DestinationPort);

								TcpHeader raw = tcpHeader.RawHeader;
								byte[] data = new byte[OPTION_SIZE];
								Marshal.Copy(new IntPtr(raw.Option), data, 0, OPTION_SIZE);
								Console.WriteLine(Encoding.GetEncoding(932).GetString(data).Trim('\0').Trim('\n'));
							}
							break;

						case InternetProtocolType.Udp:
							{
								UdpHeaderHelper udpHeader = new UdpHeaderHelper(option);
								Console.WriteLine(udpHeader.SourcePort);
								Console.WriteLine(udpHeader.DestinationPort);

								UdpHeader raw = udpHeader.RawHeader;
								byte[] data = new byte[OPTION_SIZE];
								Marshal.Copy(new IntPtr(raw.Option), data, 0, OPTION_SIZE);
								Console.WriteLine(Encoding.GetEncoding(932).GetString(data).Trim('\0').Trim('\n'));
							}
							break;

						default:
							break;
						}

						//Console.WriteLine(Encoding.GetEncoding(932).GetString(option).Trim('\0')); // ip option
					}

					Console.WriteLine("------------------------------------------------------------------");
				}
			}
		}
	}
}
