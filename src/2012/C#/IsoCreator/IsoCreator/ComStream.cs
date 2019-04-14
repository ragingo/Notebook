using System;
using System.IO;
using System.Runtime.InteropServices;
using ComTypes = System.Runtime.InteropServices.ComTypes;

namespace Sakura.Iso
{
	enum STGC
	{
		STGC_DEFAULT = 0,
		STGC_OVERWRITE = 1,
		STGC_ONLYIFCURRENT = 2,
		STGC_DANGEROUSLYCOMMITMERELYTODISKCACHE = 4,
		STGC_CONSOLIDATE = 8
	}
	enum STATFLAG
	{
		STATFLAG_DEFAULT = 0,
		STATFLAG_NONAME = 1,
		STATFLAG_NOOPEN = 2
	}
	class ComStream : Stream
	{
		private long _currentPosition;
		private ComTypes.IStream _baseStream;

		public ComStream(ComTypes.IStream baseStream)
		{
			if (baseStream == null)
			{
				throw new ArgumentNullException();
			}
			_baseStream = baseStream;
		}

		public override bool CanRead
		{
			get { return true; }
		}

		public override bool CanSeek
		{
			get { return true; }
		}

		public override bool CanWrite
		{
			get { return true; }
		}

		public override void Flush()
		{
			_baseStream.Commit((int)STGC.STGC_DEFAULT);
		}

		public override long Length
		{
			get
			{
				ComTypes.STATSTG stat;
				_baseStream.Stat(out stat, (int)STATFLAG.STATFLAG_DEFAULT);
				return stat.cbSize;
			}
		}

		public override long Position
		{
			get
			{
				return _currentPosition;
			}
			set
			{
				Seek(value, SeekOrigin.Begin);
			}
		}

		public override int Read(byte[] buffer, int offset, int count)
		{
			IntPtr pcbRead = Marshal.AllocHGlobal(IntPtr.Size);
			_baseStream.Read(buffer, buffer.Length, pcbRead);
			int len = Marshal.ReadInt32(pcbRead);
			Marshal.FreeHGlobal(pcbRead);
			return len;
		}

		public override long Seek(long offset, SeekOrigin origin)
		{
			IntPtr newPosPtr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(long)));
			_baseStream.Seek(offset, (int)origin, newPosPtr);
			long newPos = newPosPtr.ToInt64();
			Marshal.FreeHGlobal(newPosPtr);
			_currentPosition = newPos;
			return newPos;
		}

		public override void SetLength(long value)
		{
			_baseStream.SetSize(value);
		}

		public override void Write(byte[] buffer, int offset, int count)
		{
			_baseStream.Write(buffer, count, IntPtr.Zero);
		}

		public override void Close()
		{
			base.Close();
			if (_baseStream != null)
			{
				Marshal.ReleaseComObject(_baseStream);
				_baseStream = null;
			}
		}
	}
}
