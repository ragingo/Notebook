using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace ConsoleApplication1
{
	class Program
	{
		private const int adSaveCreateOverWrite = 2;
		static void Main(string[] args)
		{
			string filename = @"C:\Users\User1\Desktop\a.eml";
			//string content = File.ReadAllText(filename);

			//dynamic msg = new Message();
			//msg.DataSource.Open(filename);

			//dynamic msg = Activator.CreateInstance(Type.GetTypeFromProgID("CDO.Message"));
			//msg.Open(filename);

			//dynamic stream = Activator.CreateInstance(Type.GetTypeFromProgID("ADODB.Stream"));
			//stream.Open();
			//stream.LoadFromFile(filename);
			//stream.Position = 0;
			//dynamic msg = Activator.CreateInstance(Type.GetTypeFromProgID("CDO.Message"));
			//msg.DataSource.OpenObject(stream, "_Stream");
			//msg.Attachments.DeleteAll();
			//msg.DataSource.SaveToObject(stream, "_Stream");
			////msg.DataSource.Save();
			//stream.Position = 0;
			//stream.SaveToFile(filename, adSaveCreateOverWrite);
			//stream.Close();

			ADODB.StreamClass stream = new ADODB.StreamClass();
			stream.Open();
			stream.LoadFromFile(filename);
			CDO.MessageClass msg = new CDO.MessageClass();
			msg.DataSource.OpenObject(stream, "_Stream");
			msg.Attachments.DeleteAll();
			msg.DataSource.SaveToObject(stream, "_Stream");
			stream.SaveToFile(filename, ADODB.SaveOptionsEnum.adSaveCreateOverWrite);
			stream.Close();
			System.Runtime.InteropServices.Marshal.FinalReleaseComObject(msg);
			System.Runtime.InteropServices.Marshal.FinalReleaseComObject(stream);
		}
	}
}
