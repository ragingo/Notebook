using System;
using System.Runtime.InteropServices;

namespace NetworkTest.Network.IP
{
	[StructLayout(LayoutKind.Explicit)]
	public struct IPHeader
	{
		[FieldOffset(0)] public byte VersionAndHeaderLength;		// IP Version and IP Header Length
		[FieldOffset(1)] public byte TypeOfService;					// Type of Service
		[FieldOffset(2)] public ushort TotalLength;					// Total Packet Length
		[FieldOffset(4)] public ushort Identification;				// ID
		[FieldOffset(6)] public ushort FlagsAndOffset;				// Flags and Offset
		[FieldOffset(8)] public byte Ttl;							// Time To Live
		[FieldOffset(9)] public byte Protocol;						// Protocol
		[FieldOffset(10)] public ushort Checksum;					// Checksum
		[FieldOffset(12)] public uint SourceAddress;				// Source IP Address
		[FieldOffset(16)] public uint DestinationAddress;			// Destination IP Address
		[FieldOffset(20)] unsafe public fixed byte Option[65535-20];// Option & Padding
	}
}
