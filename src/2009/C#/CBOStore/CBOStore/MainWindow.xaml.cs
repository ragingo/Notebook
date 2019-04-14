using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Interop;
using WinForm = System.Windows.Forms;

namespace CBOStore
{
	public partial class MainWindow : Window
	{
		private IntPtr _nextHwnd;
		private CBObjectCollection _objList = new CBObjectCollection();

		public MainWindow()
		{
			InitializeComponent();
		}

		private void Window_Loaded(object sender, RoutedEventArgs e)
		{
			IntPtr hWnd = new WindowInteropHelper(this).Handle;

			HwndSource.FromHwnd(hWnd).AddHook(WndProc);

			_nextHwnd = NativeMethods.SetClipboardViewer(hWnd);

			this.DataContext = _objList;
		}

		private IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
		{
			switch (msg)
			{
			case NativeConstants.WM_DESTROY:
				NativeMethods.ChangeClipboardChain(hwnd, _nextHwnd);
				handled = true;
				break;

			case NativeConstants.WM_DRAWCLIPBOARD:
				try
				{
					if (Clipboard.ContainsText())
					{
						string s = Clipboard.GetText();
						_objList.Add(new CBObject
						{
							Title = s.Length > 100 ? s.Substring(0, 100) : s, 
							Text = s
						});
					}
					//else if (Clipboard.ContainsImage())
					//{
					//    listView1.Items.Add(new ContentPresenter{ Content =  GetClipboardImage() });
					//}
				}
				catch{}

				if (_nextHwnd.ToInt32() != 0)
				{
					NativeMethods.SendMessage(_nextHwnd, msg, wParam, lParam);
				}
				handled = true;
				break;

			case NativeConstants.WM_CHANGECBCHAIN:
				_nextHwnd = lParam;
				if (lParam.ToInt32() != 0)
				{
					NativeMethods.SendMessage(_nextHwnd, msg, wParam, lParam);
				}
				handled = true;
				break;
			}
			return IntPtr.Zero;
		}

		private static Image GetClipboardImage()
		{
			System.Drawing.Bitmap bmp = (System.Drawing.Bitmap)WinForm.Clipboard.GetImage();
			BitmapSource source =
				Imaging.CreateBitmapSourceFromHBitmap(
					bmp.GetHbitmap(),
					IntPtr.Zero,
					Int32Rect.Empty,
					BitmapSizeOptions.FromEmptyOptions());
			bmp.Dispose();

			Image img = new Image();
			img.BeginInit();
			img.Source = source;
			img.EndInit();
			return img;
		}
	}
}
