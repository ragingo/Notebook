using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Configuration;

namespace FlickrManager
{
	class Constants
	{
		public static readonly string ApiKey = 
			ConfigurationManager.AppSettings["ApiKey"];

		public static readonly string SharedSecret =
			ConfigurationManager.AppSettings["SharedSecret"];
	}
}
