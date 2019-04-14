using System;

namespace chart_test4
{
	public class SampleData
	{
		public string Title { get; set; }
		public DateTime From { get; set; }
		public DateTime To { get; set; }
		public DateTime Star { get; set; }
		public string RangeLabel
		{
			get
			{
				return string.Format("{0:MM/dd}-{1:MM/dd}", From, To);
			}
		}
		public string StarLabel
		{
			get
			{
				return string.Format("{0:MM/dd}", Star);
			}
		}
	}
}
