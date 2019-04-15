using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Cache;
using System.Net.Security;
using System.Text;
using System.Text.RegularExpressions;

namespace Samples.Net.Ftp
{
	public class FtpClient : IDisposable
	{
		private bool _isDisposed = false;
		private Uri _uri;
		private FtpWebRequest _request;
		private ICredentials _credentials;

		private static readonly RequestCachePolicy CachePolicy = 
			new RequestCachePolicy(RequestCacheLevel.NoCacheNoStore);

		public FtpClient(string uri, string user, string password)
		{
			_uri = new Uri(uri);
			if (_uri.Scheme != Uri.UriSchemeFtp)
			{
				throw new ArgumentException("不正なスキームが指定されました。ftpを指定して下さい。", "uri");
			}
			_credentials = new NetworkCredential(user, password);
			Send(_uri, WebRequestMethods.Ftp.PrintWorkingDirectory, true, DiscardResponse);
		}

		~FtpClient()
		{
			Dispose(false);
		}

		protected virtual void Dispose(bool disposing)
		{
			if (_isDisposed)
			{
				return;
			}
			_isDisposed = true;
			if (disposing)
			{
				Uri uri = new Uri("ftp://" + _uri.Host);
				Send(uri, WebRequestMethods.Ftp.PrintWorkingDirectory, false, DiscardResponse);
			}
		}

		public void Dispose()
		{
			GC.SuppressFinalize(this);
			Dispose(true);
		}

		object DiscardResponse(FtpWebRequest request)
		{
			(request.GetResponse() as IDisposable).Dispose();
			return null;
		}

		IEnumerable<string> GetResponseLines(FtpWebRequest request)
		{
			using (var res = request.GetResponse())
			using (var sr = new StreamReader(res.GetResponseStream(), Encoding.UTF8))
			{
				while (!sr.EndOfStream)
				{
					yield return sr.ReadLine();
				}
			}
		}

		string GetResponseString(FtpWebRequest request)
		{
			return string.Join("", GetResponseLines(request));
		}

		private T Send<T>(Uri uri, string method, bool keepAlive, Func<FtpWebRequest, T> func)
		{
			_request = FtpWebRequest.Create(uri) as FtpWebRequest;
			_request.KeepAlive = keepAlive;
			_request.Credentials = _credentials;
			_request.Method = method;
			_request.CachePolicy = CachePolicy;
			return func(_request);
		}

		public string ChangeDirectory(string path)
		{
			_uri = new Uri(_uri, _uri.OriginalString + "/" + path);
			Send(_uri, WebRequestMethods.Ftp.PrintWorkingDirectory, true, DiscardResponse);
			return _uri.AbsoluteUri;
		}

		public string PrintWorkingDirectory()
		{
			return _uri.AbsoluteUri;
		}

		public string MakeDirectory(string name)
		{
			Uri uri = new Uri(_uri, _uri.OriginalString + "/" + name);
			try
			{
				Send(uri, WebRequestMethods.Ftp.MakeDirectory, true, DiscardResponse);
			}
			catch (WebException e)
			{
				switch ((e.Response as FtpWebResponse).StatusCode)
				{
				case FtpStatusCode.ActionNotTakenFileUnavailable:
					break;
				default:
					throw e;
				}
			}
			return uri.AbsoluteUri;
		}

		public long UploadFiles(IEnumerable<string> fileNames)
		{
			const int BUF_SIZE = 1024 * 8;
			long totalBytes = 0L;

			foreach (var fileName in fileNames)
			{
				Uri uri = new Uri(_uri.OriginalString + "/" + (new FileInfo(fileName)).Name);
				Send(uri, WebRequestMethods.Ftp.UploadFile, true, req =>
				{
					using (var st = req.GetRequestStream())
					using (var fs = File.OpenRead(fileName))
					{
						byte[] buffer = new byte[BUF_SIZE];
						int readBytes;
						while (true)
						{
							readBytes = fs.Read(buffer, 0, BUF_SIZE);
							if (readBytes == 0)
							{
								break;
							}
							st.Write(buffer, 0, readBytes);
							totalBytes += readBytes;
						}
					}
					return totalBytes;
				});
			}
			return totalBytes;
		}
	}
}
