using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using freedb;
using System.Net;

namespace test
{
	class Program
	{
		static void Main(string[] args)
		{
			List<CdaInfo> list = new List<CdaInfo>();
			foreach(string file in Directory.GetFiles("V:")) 
			{
				CdaInfo ci = freedb.freedb.ReadCda(File.ReadAllBytes(file));
				Console.WriteLine("-----------------------------------------------------");
				Console.WriteLine(ci.Track);
				Console.WriteLine(ci.Serial);
				Console.WriteLine(ci.HsgPosition);
				Console.WriteLine(ci.HsgLength);
				Console.WriteLine(ci.FramePosition);
				Console.WriteLine(ci.TimePosition);
				Console.WriteLine(ci.FrameLength);
				Console.WriteLine(ci.TimeLength);
				Console.WriteLine("-----------------------------------------------------");
				list.Add(ci);
			}

			string discid = freedb.freedb.CalculateDiscId(list);
			Console.WriteLine(discid);

			const string url = "http://freedbtest.dyndns.org/~cddb/cddb.cgi?";
			StringBuilder sb = new StringBuilder();
			sb.Append("cmd=cddb")
				.Append("+")
				.Append("query")
				.Append("+")
				.Append(discid)
				.Append("+")
				.Append(list.Count)
				.Append("+")
				.Append(string.Join("+", list.ConvertAll(x => x.HsgPosition.ToString()).ToArray()))
				.Append("+")
				.Append((int)list.Sum(x => x.TimeLength.TotalSeconds))
				.Append("&hello=joe+my.host.com+xmcd_via_email+v1.0")
				.Append("&proto=1");

			Console.WriteLine(url + sb.ToString());
			HttpWebRequest req = WebRequest.Create(url + sb.ToString()) as HttpWebRequest;
			HttpWebResponse res = req.GetResponse() as HttpWebResponse;
			using(var sr = new StreamReader(res.GetResponseStream()))
			{
				Console.WriteLine(sr.ReadToEnd());
			}
		}
	}
}
