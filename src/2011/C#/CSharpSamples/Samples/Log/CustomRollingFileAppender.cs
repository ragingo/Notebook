using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using log4net.Appender;

namespace Samples.Log
{
	class CustomRollingFileAppender : RollingFileAppender
	{
		public Exception Exception
		{
			get;
			private set;
		}
		protected override void OpenFile(string fileName, bool append)
		{
			try
			{
				base.OpenFile(fileName, append);
				Exception = null;
			}
			catch (Exception e)
			{
				Exception = e;
			}
		}
	}
}
