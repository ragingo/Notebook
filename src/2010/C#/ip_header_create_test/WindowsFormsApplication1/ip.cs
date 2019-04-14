using System;
using System.Collections;
using System.Runtime.InteropServices;
using System.Reflection;
using System.ComponentModel;
using System.Net;

namespace WindowsFormsApplication1
{
	public enum IPHeaderItem
	{
		Version,
		HeaderLength,
		TosPrecedence,
		TosDelay,
		TosThroughput,
		TosReliability,
		TosReserved,
		TotalLength,
		Identification,
		VcfReserved,
		VcfDontFragment,
		VcfMoreFragment,
		FragmentOffset,
		Ttl,
		Protocol,
		Checksum,
		SourceAddress,
		DestinationAddress
	}

	public class IPHeader2 : NotifyPropertyChangedBase
	{
		private IPHeader _ip = new IPHeader();
		private Func<string> _currentSetProperty = () => MethodInfo.GetCurrentMethod().Name.Replace("set_", "");

		public int Version
		{
			get
			{
				return _ip[IPHeaderItem.Version].ToInt32();
			}
			set
			{
				_ip[IPHeaderItem.Version].SetInt32(value);
				OnPropertyChanged(_currentSetProperty());
			}
		}
		public int HeaderLength
		{
			get
			{
				return _ip[IPHeaderItem.HeaderLength].ToInt32();
			}
			set
			{
				_ip[IPHeaderItem.HeaderLength].SetInt32(value);
				OnPropertyChanged(_currentSetProperty());
			}
		}
		public PacketPriority TosPrecedence
		{
			get
			{
				return (PacketPriority)_ip[IPHeaderItem.TosPrecedence].ToByte();
			}
			set
			{
				_ip[IPHeaderItem.TosPrecedence].SetByte((byte)value);
				OnPropertyChanged(_currentSetProperty());
			}
		}
		public byte TosDelay
		{
			get
			{
				return _ip[IPHeaderItem.TosDelay].ToByte();
			}
			set
			{
				_ip[IPHeaderItem.TosDelay].SetByte(value);
				OnPropertyChanged(_currentSetProperty());
			}
		}
		public byte TosThroughput
		{
			get
			{
				return _ip[IPHeaderItem.TosThroughput].ToByte();
			}
			set
			{
				_ip[IPHeaderItem.TosThroughput].SetByte(value);
				OnPropertyChanged(_currentSetProperty());
			}
		}
		public byte TosReliability
		{
			get
			{
				return _ip[IPHeaderItem.TosReliability].ToByte();
			}
			set
			{
				_ip[IPHeaderItem.TosReliability].SetByte(value);
				OnPropertyChanged(_currentSetProperty());
			}
		}
		public byte TosReserved
		{
			get
			{
				return _ip[IPHeaderItem.TosReserved].ToByte();
			}
			set
			{
				_ip[IPHeaderItem.TosReserved].SetByte(value);
				OnPropertyChanged(_currentSetProperty());
			}
		}
		public int TotalLength
		{
			get
			{
				return _ip[IPHeaderItem.TotalLength].ToInt32();
			}
			set
			{
				_ip[IPHeaderItem.TotalLength].SetInt32(value);
				OnPropertyChanged(_currentSetProperty());
			}
		}
		public int Identification
		{
			get
			{
				return _ip[IPHeaderItem.Identification].ToInt32();
			}
			set
			{
				_ip[IPHeaderItem.Identification].SetInt32(value);
				OnPropertyChanged(_currentSetProperty());
			}
		}
		public byte VcfReserved
		{
			get
			{
				return _ip[IPHeaderItem.VcfReserved].ToByte();
			}
			set
			{
				_ip[IPHeaderItem.VcfReserved].SetByte(value);
				OnPropertyChanged(_currentSetProperty());
			}
		}
		public byte VcfDontFragment
		{
			get
			{
				return _ip[IPHeaderItem.VcfDontFragment].ToByte();
			}
			set
			{
				_ip[IPHeaderItem.VcfDontFragment].SetByte(value);
				OnPropertyChanged(_currentSetProperty());
			}
		}
		public byte VcfMoreFragment
		{
			get
			{
				return _ip[IPHeaderItem.VcfMoreFragment].ToByte();
			}
			set
			{
				_ip[IPHeaderItem.VcfMoreFragment].SetByte(value);
				OnPropertyChanged(_currentSetProperty());
			}
		}
		public int FragmentOffset
		{
			get
			{
				return _ip[IPHeaderItem.FragmentOffset].ToInt32();
			}
			set
			{
				_ip[IPHeaderItem.FragmentOffset].SetInt32(value);
				OnPropertyChanged(_currentSetProperty());
			}
		}
		public int Ttl
		{
			get
			{
				return _ip[IPHeaderItem.Ttl].ToInt32();
			}
			set
			{
				_ip[IPHeaderItem.Ttl].SetInt32(value);
				OnPropertyChanged(_currentSetProperty());
			}
		}
		public InternetProtocolType Protocol
		{
			get
			{
				return (InternetProtocolType)_ip[IPHeaderItem.Protocol].ToInt32();
			}
			set
			{
				_ip[IPHeaderItem.Protocol].SetInt32((int)value);
				OnPropertyChanged(_currentSetProperty());
			}
		}
		public int Checksum
		{
			get
			{
				return _ip[IPHeaderItem.Checksum].ToInt32();
			}
			set
			{
				_ip[IPHeaderItem.Checksum].SetInt32(value);
				OnPropertyChanged(_currentSetProperty());
			}
		}
		public IPAddress SourceAddress
		{
			get
			{
				return new IPAddress(_ip[IPHeaderItem.SourceAddress].ToInt32());
			}
			set
			{
				_ip[IPHeaderItem.SourceAddress].SetInt32((int)value.Address);
				OnPropertyChanged(_currentSetProperty());
			}
		}
		public IPAddress DestinationAddress
		{
			get
			{
				return new IPAddress(_ip[IPHeaderItem.DestinationAddress].ToInt32());
			}
			set
			{
				_ip[IPHeaderItem.DestinationAddress].SetInt32((int)value.Address);
				OnPropertyChanged(_currentSetProperty());
			}
		}

		[Bindable(false)]
		[Browsable(false)]
		public IPRawHeader RawHeader
		{
			get
			{
				return _ip.RawHeader;
			}
		}

		[Bindable(false)]
		[Browsable(false)]
		public byte[] RawData
		{
			get
			{
				return _ip.RawData;
			}
		}
		public void CreatePacket()
		{
			_ip.CreatePacket();
		}
	}

	public class IPHeader
	{
		private BitArray _version            = new BitArray( 4, false);
		private BitArray _headerLength       = new BitArray( 4, false);
		private BitArray _tos_precedence     = new BitArray( 3, false);
		private BitArray _tos_delay          = new BitArray( 1, false);
		private BitArray _tos_throughput     = new BitArray( 1, false);
		private BitArray _tos_reliability    = new BitArray( 1, false);
		private BitArray _tos_reserved       = new BitArray( 2, false);
		private BitArray _totalLength        = new BitArray(16, false);
		private BitArray _identification     = new BitArray(16, false);
		private BitArray _vcf_reserved       = new BitArray( 1, false);
		private BitArray _vcf_dontFragment   = new BitArray( 1, false);
		private BitArray _vcf_moreFragment   = new BitArray( 1, false);
		private BitArray _fragmentOffset     = new BitArray(13, false);
		private BitArray _ttl                = new BitArray( 8, false);
		private BitArray _protocol           = new BitArray( 8, false);
		private BitArray _checksum           = new BitArray(16, false);
		private BitArray _sourceAddress      = new BitArray(32, false);
		private BitArray _destinationAddress = new BitArray(32, false);
		private IPRawHeader _rawHeader;
		private byte[] _data = new byte[20];

		public BitArray this[IPHeaderItem item]
		{
			get
			{
				switch (item)
				{
				case IPHeaderItem.Version:
					return _version;
				case IPHeaderItem.HeaderLength:
					return _headerLength;
				case IPHeaderItem.TosPrecedence:
					return _tos_precedence;
				case IPHeaderItem.TosDelay:
					return _tos_delay;
				case IPHeaderItem.TosThroughput:
					return _tos_throughput;
				case IPHeaderItem.TosReliability:
					return _tos_reliability;
				case IPHeaderItem.TosReserved:
					return _tos_reserved;
				case IPHeaderItem.TotalLength:
					return _totalLength;
				case IPHeaderItem.Identification:
					return _identification;
				case IPHeaderItem.VcfReserved:
					return _vcf_reserved;
				case IPHeaderItem.VcfDontFragment:
					return _vcf_dontFragment;
				case IPHeaderItem.VcfMoreFragment:
					return _vcf_moreFragment;
				case IPHeaderItem.FragmentOffset:
					return _fragmentOffset;
				case IPHeaderItem.Ttl:
					return _ttl;
				case IPHeaderItem.Protocol:
					return _protocol;
				case IPHeaderItem.Checksum:
					return _checksum;
				case IPHeaderItem.SourceAddress:
					return _sourceAddress;
				case IPHeaderItem.DestinationAddress:
					return _destinationAddress;
				default:
					return null;
				}
			}
		}

		public IPHeader()
		{
		}

		public IPRawHeader RawHeader
		{
			get
			{
				return _rawHeader;
			}
		}
		public byte[] RawData
		{
			get
			{
				return _data;
			}
		}
		public unsafe void CreatePacket()
		{
			Array.Clear(_data, 0, _data.Length);
			fixed (byte* buf_ptr = _data)
			{
				int totalLength = _totalLength.ToInt32();
				int identification = _identification.ToInt32();
				int fragmentOffset = _fragmentOffset.ToInt32();
				int sourceAddress = _sourceAddress.ToInt32();
				int destinationAddress = _destinationAddress.ToInt32();

				byte* p = buf_ptr;
				*p++ = (byte)(_version.ToByte() << 4 | _headerLength.ToByte());
				*p++ = (byte)(_tos_precedence.ToByte() << 5 |
							  _tos_delay.ToByte() << 4 |
							  _tos_throughput.ToByte() << 3 |
							  _tos_reliability.ToByte() << 2 |
							  _tos_reserved.ToByte());
				*p++ = (byte)(totalLength & 0xff);
				*p++ = (byte)(totalLength >> 8);
				*p++ = (byte)(identification & 0xff);
				*p++ = (byte)(identification >> 8);
				*p++ = (byte)(fragmentOffset & 0xff);
				*p++ = (byte)(_vcf_reserved.ToByte() << 15 |
							  _vcf_dontFragment.ToByte() << 14 |
							  _vcf_moreFragment.ToByte() << 13 |
							  fragmentOffset >> 8);
				*p++ = _ttl.ToByte();
				*p++ = _protocol.ToByte();
				*p++ = (byte)(_checksum.ToInt32() & 0xff);
				*p++ = (byte)(_checksum.ToInt32() >> 8);
				*p++ = (byte)(sourceAddress & 0xff);
				*p++ = (byte)(sourceAddress >> 8 & 0xff);
				*p++ = (byte)(sourceAddress >> 16 & 0xff);
				*p++ = (byte)(sourceAddress >> 24 & 0xff);
				*p++ = (byte)(destinationAddress & 0xff);
				*p++ = (byte)(destinationAddress >> 8 & 0xff);
				*p++ = (byte)(destinationAddress >> 16 & 0xff);
				*p++ = (byte)(destinationAddress >> 24 & 0xff);

				_rawHeader = *(IPRawHeader*)buf_ptr;
			}
		}
	}

	[StructLayout(LayoutKind.Explicit)]
	public struct IPRawHeader
	{
		[FieldOffset( 0)] public byte VersionAndHeaderLength;
		[FieldOffset( 1)] public byte TypeOfService;
		[FieldOffset( 2)] public ushort TotalLength;
		[FieldOffset( 4)] public ushort Identification;
		[FieldOffset( 6)] public ushort FlagsAndOffset;
		[FieldOffset( 8)] public byte Ttl;
		[FieldOffset( 9)] public byte Protocol;
		[FieldOffset(10)] public ushort Checksum;
		[FieldOffset(12)] public uint SourceAddress;
		[FieldOffset(16)] public uint DestinationAddress;
	}

	public enum PacketPriority : byte
	{
		Routine = 0x0,
		Priority = 0x1,
		Immediate = 0x2,
		Flash = 0x3,
		FlashOverride = 0x4,
		CriticEcp = 0x5,
		InternetworkControl = 0x6,
		NetworkControl = 0x7,
	}

	public enum InternetProtocolType : int
	{
		Unknown = -1,
		Unspecified = 0,
		IP = 0,
		IPv6HopByHopOptions = 0,
		Icmp = 1,
		Igmp = 2,
		Ggp = 3,
		IPv4 = 4,
		St = 5,
		Tcp = 6,
		Cbt = 7,
		Egp = 8,
		Igp = 9,
		BbnRccMon = 10,
		NvpIi = 11,
		Pup = 12,
		Argus = 13,
		Emcon = 14,
		Xnet = 15,
		Chaos = 16,
		Udp = 17,
		Mux = 18,
		DcnMeas = 19,
		Hmp = 20,
		Prm = 21,
		XnsIdp = 22,
		Trunk1 = 23,
		Trunk2 = 24,
		Leaf1 = 25,
		Leaf2 = 26,
		Rdp = 27,
		Irtp = 28,
		IsoTp4 = 29,
		Netblt = 30,
		MfeNsp = 31,
		MeritInp = 32,
		Dccp = 33,
		ThreePC = 34,
		Idpr = 35,
		Xtp = 36,
		Ddp = 37,
		IdprCmtp = 38,
		Tppp = 39,
		Il = 40,
		IPv6 = 41,
		Sdrp = 42,
		IPv6RoutingHeader = 43,
		IPv6FragmentHeader = 44,
		Idrp = 45,
		Rsvp = 46,
		Gre = 47,
		Dsr = 48,
		Bna = 49,
		IPSecEncapsulatingSecurityPayload = 50,
		IPSecAuthenticationHeader = 51,
		INlsp = 52,
		Swipe = 53,
		Narp = 54,
		Mobile = 55,
		Tlsp = 56,
		Skip = 57,
		IcmpV6 = 58,
		IPv6NoNextHeader = 59,
		IPv6DestinationOptions = 60,
		Anyhostinternalprotocol = 61,
		Cftp = 62,
		Anylocalnetwork = 63,
		SatExpak = 64,
		Kryptolan = 65,
		Rvd = 66,
		Ippc = 67,
		Anydistributedfilesystem = 68,
		SatMon = 69,
		Visa = 70,
		Ipcv = 71,
		Cpnx = 72,
		Cphb = 73,
		Wsn = 74,
		Pvp = 75,
		BrSatMon = 76,
		SunNd = 77,
		WbMon = 78,
		WbExpak = 79,
		IsoIP = 80,
		Vmtp = 81,
		SecureVmtp = 82,
		Vines = 83,
		Ttp = 84,
		NsfnetIgp = 85,
		Dgp = 86,
		Tcf = 87,
		Eigrp = 88,
		Ospfigp = 89,
		SpriteRpc = 90,
		Larp = 91,
		Mtp = 92,
		Ax25 = 93,
		Ipip = 94,
		Micp = 95,
		SccSp = 96,
		Etherip = 97,
		Encap = 98,
		Anyprivateencryptionscheme = 99,
		Gmtp = 100,
		Ifmp = 101,
		Pnni = 102,
		Pim = 103,
		Aris = 104,
		Scps = 105,
		Qnx = 106,
		AN = 107,
		Ipcomp = 108,
		Snp = 109,
		CompaqPeer = 110,
		IpxInIp = 111,
		Vrrp = 112,
		Pgm = 113,
		Any0Hopprotocol = 114,
		L2Tp = 115,
		Ddx = 116,
		Iatp = 117,
		Stp = 118,
		Srp = 119,
		Uti = 120,
		Smp = 121,
		Sm = 122,
		Ptp = 123,
		Isisoveripv4 = 124,
		Fire = 125,
		Crtp = 126,
		Crudp = 127,
		Sscopmce = 128,
		Iplt = 129,
		Sps = 130,
		Pipe = 131,
		Sctp = 132,
		Fc = 133,
		RsvpE2EIgnore = 134,
		Mobilityheader = 135,
		Udplite = 136,
		MplsInIp = 137,
		Manet = 138,
		Hip = 139,
		Raw = 255,
		Ipx = 1000,
		Spx = 1256,
		SpxII = 1257,
	}
}
