using System.IO;
using System.Text;
using System.Windows;
using System.Windows.Controls;

namespace CarManagementSystem.Controls
{
	public static class WebBrowserExtentions
	{
		public static readonly DependencyProperty DocumentProperty =
			DependencyProperty.RegisterAttached("Document", typeof(string), typeof(WebBrowserExtentions),
				new UIPropertyMetadata(DocumentPropertyChanged));

		public static string GetDocument(DependencyObject obj)
		{
			return (string)obj.GetValue(DocumentProperty);
		}

		public static void SetDocument(DependencyObject obj, string value)
		{
			obj.SetValue(DocumentProperty, value);
		}

		public static void DocumentPropertyChanged(DependencyObject obj, DependencyPropertyChangedEventArgs e)
		{
			var browser = obj as WebBrowser;
			if (browser == null)
			{
				return;
			}
			string document = (string)e.NewValue;
			if (string.IsNullOrWhiteSpace(document))
			{
				browser.Navigate("about:blank");
			}
			else
			{
				var ms = new MemoryStream(Encoding.Default.GetBytes(document));
				browser.NavigateToStream(ms);
			}
		}
	}
}
