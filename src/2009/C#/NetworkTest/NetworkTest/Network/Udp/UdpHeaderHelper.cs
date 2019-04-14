using System;
using System.Collections.Generic;
using System.Text;
using System.Net;

namespace NetworkTest.Network.Udp
{
	public class UdpHeaderHelper
	{
		private UdpHeader m_header;

		public UdpHeader RawHeader
		{
			get
			{
				return m_header;
			}
		}

		public int SourcePort
		{
			get
			{
				return m_header.SourcePort;
			}
		}

		public int DestinationPort
		{
			get
			{
				return m_header.DestinationPort;
			}
		}

		unsafe public UdpHeaderHelper(byte[] buffer)
		{
			fixed (byte* p = buffer)
			{
				m_header = *((UdpHeader*)p);
			}
		}
	}
}
