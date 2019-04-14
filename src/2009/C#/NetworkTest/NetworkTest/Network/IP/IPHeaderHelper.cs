using System;
using System.Net;
using System.Net.Sockets;

namespace NetworkTest.Network.IP
{
	public class IPHeaderHelper
	{
		private IPHeader m_header;
		/// <summary>
		/// IPHeader構造体を取得します。
		/// </summary>
		public IPHeader RawHeader
		{
			get
			{
				return m_header;
			}
		}
		/// <summary>プロトコル バージョンを取得します。</summary>
		public int Version
		{
			get
			{
				return m_header.VersionAndHeaderLength >> 4; // 上位4bit
			}
		}
		/// <summary>ヘッダ長を取得します。</summary>
		public int HeaderLength
		{
			get
			{
				return m_header.VersionAndHeaderLength & 0x0f; // 下位4bit
			}
		}
		/// <summary>サービス種別 - 優先度を取得します。</summary>
		public PacketPriority Precedence
		{
			get
			{
				return (PacketPriority)(m_header.TypeOfService >> 5); // 上位3bit
			}
		}
		/// <summary>サービス種別 - 遅延度を取得します。</summary>
		public int Delay
		{
			get
			{
				return m_header.TypeOfService >> 4 & 0x1; // 4bit目
			}
		}
		/// <summary>サービス種別 - 転送量を取得します。</summary>
		public int Throughput
		{
			get
			{
				return m_header.TypeOfService >> 3 & 0x1; // 5bit目
			}
		}
		/// <summary>サービス種別 - 信頼性を取得します。</summary>
		public int Reliability
		{
			get
			{
				return m_header.TypeOfService >> 2 & 0x1; // 6bit目
			}
		}
		/// <summary>全長を取得します。</summary>
		public int TotalLength
		{
			get
			{
				return m_header.TotalLength;
			}
		}
		/// <summary>識別子を取得します。</summary>
		public int Identification
		{
			get
			{
				return m_header.Identification;
			}
		}
		/// <summary>TTL(Time to Live : 生存時間)を取得します。</summary>
		public int Ttl
		{
			get
			{
				return m_header.Ttl;
			}
		}
		/// <summary>プロトコルを取得します。</summary>
		public InternetProtocolType Protocol
		{
			get
			{
				return (InternetProtocolType)m_header.Protocol;
			}
		}
		/// <summary>チェックサムを取得します。</summary>
		public int Checksum
		{
			get
			{
				return m_header.Checksum;
			}
		}
		/// <summary>送信元IPアドレスを取得します。</summary>
		public IPAddress SourceAddress
		{
			get
			{
				return new IPAddress(m_header.SourceAddress);
			}
		}
		/// <summary>宛先IPアドレスを取得します。</summary>
		public IPAddress DestinationAddress
		{
			get
			{
				return new IPAddress(m_header.DestinationAddress);
			}
		}
		/// <summary>断片化しているかどうかを取得します。</summary>
		public bool IsFlagment
		{
			get
			{
				return this.DontFragment && this.MoreFragment;
			}
		}
		/// <summary>断片化を許可するかどうか(DF)を取得します。</summary>
		public bool DontFragment
		{
			get
			{
				return (m_header.FlagsAndOffset >> 14) == 0x1;
			}
		}
		/// <summary>継続パケットの有無(MF)を取得します。</summary>
		public bool MoreFragment
		{
			get
			{
				return (m_header.FlagsAndOffset >> 15) == 0x1;
			}
		}
		/// <summary>断片化パケットのオフセットを取得します。</summary>
		public int FlagmentOffset
		{
			get
			{
				return m_header.FlagsAndOffset & 0x1fff; // 下位13bit
			}
		}
		/// <summary>指定したバッファを使用して、新しいインスタンスを初期化します。</summary>
		/// <param name="buffer">受信したバッファ</param>
		unsafe public IPHeaderHelper(byte[] buffer)
		{
			fixed (byte* p = buffer)
			{
				m_header = *((IPHeader*)p);
			}
		}
	}
}
