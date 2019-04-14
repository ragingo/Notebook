using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics.Eventing.Reader;

namespace ConsoleApplication1
{
	class SecurityLog
	{
		private static readonly Dictionary<ProviderMetadataCache<int?>, string> Tasks =
			new Dictionary<ProviderMetadataCache<int?>, string>(new ProviderMetadataComparer<int?>());
		private static readonly Dictionary<ProviderMetadataCache<short?>, string> OpCodes =
			new Dictionary<ProviderMetadataCache<short?>, string>(new ProviderMetadataComparer<short?>());
		private static readonly Dictionary<ProviderMetadataCache<long?>, string> Keywords =
			new Dictionary<ProviderMetadataCache<long?>, string>(new ProviderMetadataComparer<long?>());

		private static int count = 0;
		public SecurityLog(EventLogRecord rec, string[] refs)
		{
			var props = rec.GetPropertyValues(new EventLogPropertySelector(refs));

			Console.WriteLine(count++);
			ProcessId = (int)(uint)props[2];
			TimeCreated = (DateTime?)props[1];
			Source = (string)props[0];

			var keyTask = new ProviderMetadataCache<int?>(Source, (int?)(ushort)props[3]);
			if (!Tasks.ContainsKey(keyTask))
			{
				Tasks[keyTask] = rec.TaskDisplayName;
			}
			Task = Tasks[keyTask];

			var keyOpeCode = new ProviderMetadataCache<short?>(Source, (short?)(byte)props[4]);
			if (!OpCodes.ContainsKey(keyOpeCode))
			{
				OpCodes[keyOpeCode] = rec.OpcodeDisplayName;
			}
			OpCode = OpCodes[keyOpeCode];

			var keyKeyword = new ProviderMetadataCache<long?>(Source, (long?)(ulong)props[5]);
			if (!Keywords.ContainsKey(keyKeyword))
			{
				Keywords[keyKeyword] = rec.KeywordsDisplayNames.FirstOrDefault();
			}

			Message = rec.FormatDescription();
		}
		public SecurityLog(EventRecord rec)
		{
			Console.WriteLine(count++);
			ProcessId = rec.ProcessId;
			TimeCreated = rec.TimeCreated;
			Source = rec.ProviderName;

			var keyTask = new ProviderMetadataCache<int?>(rec.ProviderName, rec.Task);
			if (!Tasks.ContainsKey(keyTask))
			{
				Tasks[keyTask] = rec.TaskDisplayName;
			}
			Task = Tasks[keyTask];

			var keyOpeCode = new ProviderMetadataCache<short?>(rec.ProviderName, rec.Opcode);
			if (!OpCodes.ContainsKey(keyOpeCode))
			{
				OpCodes[keyOpeCode] = rec.OpcodeDisplayName;
			}
			OpCode = OpCodes[keyOpeCode];

			var keyKeyword = new ProviderMetadataCache<long?>(rec.ProviderName, rec.Keywords);
			if (!Keywords.ContainsKey(keyKeyword))
			{
				Keywords[keyKeyword] = rec.KeywordsDisplayNames.FirstOrDefault();
			}

			Message = rec.FormatDescription();
		}
		public static void ClearCache()
		{
			Tasks.Clear();
			OpCodes.Clear();
			Keywords.Clear();
		}
		public string Source
		{
			get;
			set;
		}
		public string Task
		{
			get;
			set;
		}
		public int? ProcessId
		{
			get;
			set;
		}
		public DateTime? TimeCreated
		{
			get;
			set;
		}
		public string OpCode
		{
			get;
			set;
		}
		public string Keyword
		{
			get;
			set;
		}
		public string Message
		{
			get;
			set;
		}
		public override string ToString()
		{
			StringBuilder sb = new StringBuilder();
			sb.Append(Source);
			sb.Append(", ");
			sb.Append(Task);
			sb.Append(", ");
			sb.Append(ProcessId);
			sb.Append(", ");
			sb.Append(TimeCreated);
			sb.Append(", ");
			sb.Append(OpCode);
			sb.Append(", ");
			sb.Append(Keyword);
			sb.Append(", ");
			sb.Append(Message);
			return sb.ToString();
		}
	}
}
