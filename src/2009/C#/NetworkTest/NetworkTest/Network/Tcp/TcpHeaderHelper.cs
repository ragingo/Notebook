using System;
using System.Collections.Generic;
using System.Text;
using System.Net;

namespace NetworkTest.Network.Tcp
{
	public class TcpHeaderHelper
	{
		private TcpHeader m_header;

		public TcpHeader RawHeader
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

		unsafe public TcpHeaderHelper(byte[] buffer)
		{
			fixed (byte* p = buffer)
			{
				m_header = *((TcpHeader*)p);
			}
		}
	}
}
