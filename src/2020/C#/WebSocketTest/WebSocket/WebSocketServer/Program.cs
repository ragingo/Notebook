using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Security.Cryptography;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace WebSocketServer
{
    // https://developer.mozilla.org/ja/docs/Web/API/WebSockets_API/Writing_WebSocket_server
    // https://developer.mozilla.org/ja/docs/WebSockets-840092-dup/Writing_WebSocket_servers
    // https://triple-underscore.github.io/RFC6455-ja.html
    class Program
    {
        private const string CommonResponseHeader =
            "HTTP/1.1 101 Switching Protocols\r\n" +
            "Connection: Upgrade\r\n" +
            "Upgrade: websocket\r\n";
        private const string WsGuid = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

        static async Task Main()
        {
            var listener = new TcpListener(IPAddress.Parse("127.0.0.1"), 8080);
            listener.Start();

            var client = await listener.AcceptTcpClientAsync().ConfigureAwait(false);
            var stream = client.GetStream();

            while (true)
            {
                if (!stream.DataAvailable)
                {
                    continue;
                }

                byte[] buf = new byte[client.Available];
                int len = await stream.ReadAsync(buf, 0, buf.Length).ConfigureAwait(false);
                if (len == 0)
                {
                    continue;
                }

                string data = Encoding.UTF8.GetString(buf);
                if (data.StartsWith("GET"))
                {
                    var r = new Regex("Sec-WebSocket-Key: (.*)");
                    var m = r.Match(data);
                    if (m.Groups.Count > 0)
                    {
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
                        await stream.WriteAsync(res, 0, res.Length).ConfigureAwait(false);
                        await stream.FlushAsync().ConfigureAwait(false);
                    }
                }
                else
                {
                    Console.WriteLine("--------------------------------------------------");
                    Console.WriteLine(string.Join(" ", buf.Select(x => x.ToString("X2"))));
                    //Console.WriteLine(data);
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

                    if (payloadLen <= 125)
                    {
                        if (mask == 1)
                        {
                            var decodedValue = new List<byte>();
                            byte[] maskKey = new[] { buf[2], buf[3], buf[4], buf[5] };
                            for (int i = 6; i < buf.Length; i++)
                            {
                                decodedValue.Add((byte)(buf[i] ^ maskKey[i % 4]));
                            }
                            // TODO: 毎回デコード結果が違う。調べる。
                            Console.WriteLine(string.Join(" ", decodedValue.Select(x => x.ToString("X2"))));
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
    }
}
