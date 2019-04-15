using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;
using Samples.Util.Extensions;

namespace Samples.Net.IP
{
	class IPv4Header
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
		public byte Version
		{
			get { return _version.ToByte(); }
			set { _version.SetByte(value); }
		}
		public byte HeaderLength
		{
			get { return _headerLength.ToByte(); }
			set { _headerLength.SetByte(value); }
		}
		public byte TosPrecedence
		{
			get { return _tos_precedence.ToByte(); }
			set { _tos_precedence.SetByte(value); }
		}
		public byte TosDelay
		{
			get { return _tos_delay.ToByte(); }
			set { _tos_delay.SetByte(value); }
		}
		public byte TosThroughput
		{
			get { return _tos_throughput.ToByte(); }
			set { _tos_throughput.SetByte(value); }
		}
		public byte TosReliability
		{
			get { return _tos_reliability.ToByte(); }
			set { _tos_reliability.SetByte(value); }
		}
		public byte TosReserved
		{
			get { return _tos_reserved.ToByte(); }
			set { _tos_reserved.SetByte(value); }
		}
		public int TotalLength
		{
			get { return _totalLength.ToInt32(); }
			set { _totalLength.SetInt32(value); }
		}
		public int Identification
		{
			get { return _identification.ToInt32(); }
			set { _identification.SetInt32(value); }
		}
		public byte VcfReserved
		{
			get { return _vcf_reserved.ToByte(); }
			set { _vcf_reserved.SetByte(value); }
		}
		public byte VcfDontFragment
		{
			get { return _vcf_dontFragment.ToByte(); }
			set { _vcf_dontFragment.SetByte(value); }
		}
		public byte VcfMoreFragment
		{
			get { return _vcf_moreFragment.ToByte(); }
			set { _vcf_moreFragment.SetByte(value); }
		}
		public int FragmentOffset
		{
			get { return _fragmentOffset.ToInt32(); }
			set { _fragmentOffset.SetInt32(value); }
		}
		public byte Ttl
		{
			get { return _ttl.ToByte(); }
			set { _ttl.SetByte(value); }
		}
		public byte Protocol
		{
			get { return _protocol.ToByte(); }
			set { _protocol.SetByte(value); }
		}
		public int Checksum
		{
			get { return _checksum.ToInt32(); }
			set { _checksum.SetInt32(value); }
		}
		public long SourceAddress
		{
			get { return _sourceAddress.ToInt32(); }
			set { _sourceAddress.SetInt32((int)value); }
		}
		public long DestinationAddress
		{
			get { return _destinationAddress.ToInt32(); }
			set { _destinationAddress.SetInt32((int)value); }
		}

		public IPv4Header()
		{
		}
		public IPv4Header(byte[] data)
		{
			_version.SetByte((byte)(data[0] >> 4));
			_headerLength.SetByte((byte)(data[0] & 0x0f));
			_tos_precedence.SetByte((byte)(data[1] >> 5));
			_tos_delay.SetByte((byte)(data[1] >> 4 & 0x01));
			_tos_throughput.SetByte((byte)(data[1] >> 3 & 0x01));
			_tos_reliability.SetByte((byte)(data[1] >> 2 & 0x01));
			_tos_reserved.SetByte((byte)(data[1] & 0x03));
			_totalLength.SetInt32(data[2] << 8 | data[3]);
			_identification.SetInt32(data[4] << 8 | data[5]);
			_vcf_reserved.SetByte((byte)(data[6] >> 15));
			_vcf_dontFragment.SetByte((byte)(data[6] >> 14));
			_vcf_moreFragment.SetByte((byte)(data[6] >> 13));
			_fragmentOffset.SetInt32(data[6] >> 13 | data[7]);
			_ttl.SetByte(data[8]);
			_protocol.SetByte(data[9]);
			_checksum.SetInt32(data[10] << 8 | data[11]);
			_sourceAddress.SetInt32(data[12] << 24 | data[13] << 16 | data[14] << 8 | data[15]);
			_destinationAddress.SetInt32(data[16] << 24 | data[17] << 16 | data[18] << 8 | data[19]);
		}
		public byte[] CreateHeader()
		{
			int totalLength = _totalLength.ToInt32();
			int identification = _identification.ToInt32();
			int fragmentOffset = _fragmentOffset.ToInt32();
			int sourceAddress = _sourceAddress.ToInt32();
			int destinationAddress = _destinationAddress.ToInt32();

			byte[] data = new byte[20];
			data[0] = (byte)(_version.ToByte() << 4 | _headerLength.ToByte());
			data[1] = (byte)(_tos_precedence.ToByte() << 5 | 
							 _tos_delay.ToByte() << 4 | 
							 _tos_throughput.ToByte() << 3 | 
							 _tos_reliability.ToByte() << 2 | 
							 _tos_reserved.ToByte());
			data[2] = (byte)(totalLength & 0xff);
			data[3] = (byte)(totalLength >> 8);
			data[4] = (byte)(identification & 0xff);
			data[5] = (byte)(identification >> 8);
			data[6] = (byte)(fragmentOffset & 0xff);
			data[7] = (byte)(_vcf_reserved.ToByte() << 15 |
							 _vcf_dontFragment.ToByte() << 14 |
							 _vcf_moreFragment.ToByte() << 13 |
							 fragmentOffset & 0x5);
			data[8] = _ttl.ToByte();
			data[9] = _protocol.ToByte();
			data[10] = (byte)(_checksum.ToInt32() & 0xff);
			data[11] = (byte)(_checksum.ToInt32() >> 8);
			data[12] = (byte)(sourceAddress & 0xff);
			data[13] = (byte)(sourceAddress >> 8 & 0xff);
			data[14] = (byte)(sourceAddress >> 16 & 0xff);
			data[15] = (byte)(sourceAddress >> 24 & 0xff);
			data[16] = (byte)(destinationAddress & 0xff);
			data[17] = (byte)(destinationAddress >> 8 & 0xff);
			data[18] = (byte)(destinationAddress >> 16 & 0xff);
			data[19] = (byte)(destinationAddress >> 24 & 0xff);
			return data;
		}
	}
}
