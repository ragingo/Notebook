using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace NetworkTest.Network.Tcp
{
	[StructLayout(LayoutKind.Explicit)]
	public struct TcpHeader
	{
		[FieldOffset(0)] public ushort SourcePort;					// 
		[FieldOffset(2)] public ushort DestinationPort;				// 
		[FieldOffset(4)] public uint SequenceNumber;				// 
		[FieldOffset(8)] public uint AcknowledgmentNumber;			// 
		[FieldOffset(12)] public ushort DataOffsetAndCotnrolBits;	// Data Offset and Reserved area and Control bits
		[FieldOffset(14)] public ushort Window;						// Window
		[FieldOffset(16)] public ushort Checksum;					// Checksum
		[FieldOffset(18)] public ushort UrgentPointer;				// 
		[FieldOffset(20)] unsafe public fixed byte Option[40];		// 
	}
}
