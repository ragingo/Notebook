using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace NetworkTest.Network.Udp
{
	[StructLayout(LayoutKind.Explicit)]
	public struct UdpHeader
	{
		[FieldOffset(0)] public ushort SourcePort;
		[FieldOffset(2)] public ushort DestinationPort;
		[FieldOffset(4)] public ushort Length;
		[FieldOffset(6)] public ushort Checksum;
		[FieldOffset(8)] unsafe public fixed byte Option[65536];
	}
}
