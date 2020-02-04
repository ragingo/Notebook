using System;
using System.Diagnostics;
using System.Net;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace WebSocketServer
{
    // https://developer.mozilla.org/ja/docs/Web/API/WebSockets_API/Writing_WebSocket_server
    // https://developer.mozilla.org/ja/docs/WebSockets-840092-dup/Writing_WebSocket_servers
    // https://triple-underscore.github.io/RFC6455-ja.html
    class Program
    {
        private static async Task Main()
        {
            var ws = new WsServer();
            _ = Task.Run(Work(ws)).ConfigureAwait(false);
            await ws.Listen(IPAddress.Parse("127.0.0.1"), 8080).ConfigureAwait(false);
        }

        private static Tuple<string, string> GetToolInfo()
        {
            if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
            {
                return Tuple.Create(@"C:\Program Files\Wireshark\tshark.exe", "-i 6");
            }
            if (RuntimeInformation.IsOSPlatform(OSPlatform.OSX))
            {
                return Tuple.Create("tshark", "-i 1");
            }
            throw new NotSupportedException();
        }

        private static Func<Task> Work(WsServer ws)
        {
            var (path, args) = GetToolInfo();
            var psi = new ProcessStartInfo(path, args);
            psi.UseShellExecute = false;
            psi.RedirectStandardError = false;
            psi.RedirectStandardOutput = true;
            psi.RedirectStandardInput = false;
            psi.CreateNoWindow = false;

            var p = Process.Start(psi);
            var reader = p.StandardOutput;

            return async () =>
            {
                while (true)
                {
                    if (reader.EndOfStream)
                    {
                        await Task.Delay(100).ConfigureAwait(false);
                        continue;
                    }

                    string line = await reader.ReadLineAsync().ConfigureAwait(false);
                    ws.SendMessage(line);

                    await Task.Delay(50).ConfigureAwait(false);
                }
            };
        }
    }
}
