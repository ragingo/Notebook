namespace WebSocketServer
{
    struct WsHeader
    {
        public bool Fin;
        public byte Rcv1;
        public byte Rcv2;
        public byte Rcv3;
        public OpCode OpCode;
        public bool Mask;
        public int PayloadLength;

        public static WsHeader Parse(byte[] bytes)
        {
            var header = new WsHeader();
            if (bytes == null || bytes.Length < 2)
            {
                return header;
            }

            header.Fin = (bytes[0] >> 7) == 1;
            header.Rcv1 = (byte)(bytes[0] >> 6 & 1);
            header.Rcv2 = (byte)(bytes[0] >> 5 & 1);
            header.Rcv3 = (byte)(bytes[0] >> 4 & 1);
            header.OpCode = (OpCode)(bytes[0] & 0x0f);
            header.Mask = (bytes[1] >> 7) == 1;
            header.PayloadLength = bytes[1] & 0x7f;
            return header;
        }

        public static WsHeader Create(bool fin, OpCode opCode, int PayloadLength)
        {
            var header = new WsHeader();
            header.Fin = fin;
            header.Rcv1 = 0;
            header.Rcv2 = 0;
            header.Rcv3 = 0;
            header.OpCode = opCode;
            header.Mask = false;
            header.PayloadLength = PayloadLength;
            return header;
        }

        public byte[] ToBinary()
        {
            var bytes = new byte[2];
            bytes[0] = (byte)(((Fin ? 1 : 0) << 7) | (byte)OpCode);
            bytes[1] = (byte)PayloadLength;
            return bytes;
        }
    }
}
