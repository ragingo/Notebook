using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace WebSocketServer
{
    class WsServer
    {
        private const string CommonResponseHeader =
            "HTTP/1.1 101 Switching Protocols\r\n" +
            "Connection: Upgrade\r\n" +
            "Upgrade: websocket\r\n";
        private const string WsGuid = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

        private TcpListener _listener;
        private NetworkStream _stream;

        public WsServer()
        {
        }

        public async Task Listen(IPAddress address, int port)
        {
            _listener = new TcpListener(address, port);
            _listener.Start();

            await Process();
        }

        private async Task Process()
        {
            var client = await _listener.AcceptTcpClientAsync();
            _stream = client.GetStream();

            while (true)
            {
                if (!_stream.DataAvailable)
                {
                    {
                        var bytes = new List<byte>();
                        var data = Encoding.UTF8.GetBytes("yahoooo");
                        var header = WsHeader.Create(true, OpCode.Text, data.Length);
                        bytes.AddRange(header.ToBinary());
                        bytes.AddRange(data);
                        var arr = bytes.ToArray();
                        await _stream.WriteAsync(arr, 0, arr.Length).ConfigureAwait(false);
                        await Task.Delay(1000).ConfigureAwait(false);
                    }
                    continue;
                }

                byte[] buf = new byte[client.Available];
                int len = await _stream.ReadAsync(buf, 0, buf.Length).ConfigureAwait(false);
                if (len == 0)
                {
                    continue;
                }

                OnDataReceived(buf);
            }
        }

        private void OnDataReceived(byte[] buf)
        {
            string data = Encoding.UTF8.GetString(buf);

            if (data.StartsWith("GET"))
            {
                OnGetRequestReceived(data);
            }
            else
            {
                OnDataFrameReceived(buf);
            }
        }

        private async void OnGetRequestReceived(string data)
        {
            var r = new Regex("Sec-WebSocket-Key: (.*)");
            var m = r.Match(data);
            if (m.Groups.Count == 0)
            {
                return;
            }

            string key = m.Groups[1].Value.Trim();
            byte[] newKey = Encoding.UTF8.GetBytes(key + WsGuid);
            using var sha1 = SHA1.Create();
            byte[] hash = sha1.ComputeHash(newKey);
            string hashStr = Convert.ToBase64String(hash);

            var sb = new StringBuilder();
            sb.Append(CommonResponseHeader);
            sb.Append($"Sec-WebSocket-Accept: {hashStr}\r\n");
            sb.Append("\r\n");

            byte[] res = Encoding.UTF8.GetBytes(sb.ToString());
            await _stream.WriteAsync(res, 0, res.Length).ConfigureAwait(false);
            await _stream.FlushAsync().ConfigureAwait(false);
        }

        private void OnDataFrameReceived(byte[] buf)
        {
            Console.WriteLine("--------------------------------------------------");
            Console.WriteLine(string.Join(" ", buf.Select(x => x.ToString("X2"))));

            byte fields1 = buf[0];
            int fin = fields1 >> 7;
            int rsv1 = (int)(fields1 >> 6 & 0x01);
            int rsv2 = (int)(fields1 >> 5 & 0x01);
            int rsv3 = (int)(fields1 >> 4 & 0x01);
            int opcode = (int)(fields1 & 0x0f);
            Console.WriteLine($"fin: {fin}, rsv1: {rsv1}, rsv2: {rsv2}, rsv3: {rsv3}, opcode: {opcode}");

            byte fields2 = buf[1];
            int mask = fields2 >> 7;
            int payloadLen = (fields2 & 0x7f);
            Console.WriteLine($"mask: {mask}, payloadLen: {payloadLen}");

            switch ((OpCode)opcode)
            {
                case OpCode.Text:
                    OnTextFrameReceived(buf, payloadLen, mask == 1);
                    break;
            }
        }

        private void OnTextFrameReceived(byte[] buf, int payloadLen, bool mask)
        {
            if (payloadLen <= 125)
            {
                if (mask)
                {
                    var decodedValues = new List<byte>();
                    byte[] maskKey = new[] { buf[2], buf[3], buf[4], buf[5] }; // TODO: span slice でもっと見やすく
                    for (int i = 6; i < buf.Length; i++)
                    {
                        byte e = buf[i];
                        byte m = maskKey[(i-6) % 4];
                        decodedValues.Add((byte)(e ^ m));
                    }
                    Console.WriteLine($"decoded (hex): {string.Join(" ", decodedValues.Select(x => x.ToString("X2")))}");
                    Console.WriteLine($"decoded (str): {Encoding.UTF8.GetString(decodedValues.ToArray())}");
                }
            }
            else if (payloadLen == 126)
            {
                // TODO: 
            }
            else if (payloadLen == 127)
            {
                // TODO: 
            }
        }
    }
}
