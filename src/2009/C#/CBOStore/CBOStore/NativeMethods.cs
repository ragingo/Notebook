using System;
using System.Runtime.InteropServices;

namespace CBOStore
{
	class NativeMethods
	{
		[DllImport("user32.dll", CharSet = CharSet.Auto)]
		public extern static int SendMessage(IntPtr hWnd, int Msg, IntPtr wParam, IntPtr lParam);

		[DllImport("user32.dll")]
		public static extern IntPtr SetClipboardViewer(IntPtr hWndNewViewer);

		[DllImport("user32.dll")]
		public static extern bool ChangeClipboardChain(IntPtr hWndRemove, IntPtr hWndNewNext);
	}
}
