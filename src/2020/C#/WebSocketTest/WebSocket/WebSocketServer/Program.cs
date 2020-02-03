using System;
using System.Net;
using System.Threading.Tasks;

namespace WebSocketServer
{
    // https://developer.mozilla.org/ja/docs/Web/API/WebSockets_API/Writing_WebSocket_server
    // https://developer.mozilla.org/ja/docs/WebSockets-840092-dup/Writing_WebSocket_servers
    // https://triple-underscore.github.io/RFC6455-ja.html
    class Program
    {
        static async Task Main()
        {
            var ws = new WsServer();
            await ws.Listen(IPAddress.Parse("127.0.0.1"), 8080);
        }
    }
}
