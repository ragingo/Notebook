using System;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using mshtml;

namespace TokyoCarEventGirlViewer
{
	public partial class MainForm : Form
	{
		private const string URL_VOL1 = "http://news.livedoor.com/article/image_detail/4552478/?img_id={0}";
		private const string URL_VOL2 = "http://news.livedoor.com/article/image_detail/4553263/?img_id={0}";
		private const string URL_VOL3 = "http://news.livedoor.com/article/image_detail/4553820/?img_id={0}";
		private const int VOL1_START_IMAGE_ID = 926974;
		private const int VOL2_START_IMAGE_ID = 927466;
		private const int VOL3_START_IMAGE_ID = 927860;
		private int _prevImageId = 0;
		private int _nextImageId = 0;
		private string _currentVolume = URL_VOL1;

		public MainForm()
		{
			InitializeComponent();
		}

		private void MainForm_Load(object sender, EventArgs e)
		{
			Navigate(URL_VOL1, VOL1_START_IMAGE_ID);
		}

		// 「前へ」
		private void btnPrev_Click(object sender, EventArgs e)
		{
			Navigate(_currentVolume, _prevImageId);
		}

		// 「次へ」
		private void btnNext_Click(object sender, EventArgs e)
		{
			Navigate(_currentVolume, _nextImageId);
		}

		// 読み込み完了時
		private void wbBase_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e)
		{
			var doc = wbBase.Document;
			wbExtractImage.Navigate(GetImageSource(doc));
			SetLinkId(doc);

			txtBase.Text = wbBase.Url.OriginalString;
			txtExtractImagewbExtractImage.Text = wbExtractImage.Url.OriginalString;
		}

		// Volume切り替え
		private void RadioButton_CheckedChanged(object sender, EventArgs e)
		{
			if (rbVol1.Checked)
			{
				_currentVolume = URL_VOL1;
				Navigate(_currentVolume, VOL1_START_IMAGE_ID);
			}
			else if (rbVol2.Checked)
			{
				_currentVolume = URL_VOL2;
				Navigate(_currentVolume, VOL2_START_IMAGE_ID);
			}
			else if (rbVol3.Checked)
			{
				_currentVolume = URL_VOL3;
				Navigate(_currentVolume, VOL3_START_IMAGE_ID);
			}
		}

		// リンクID設定
		private void SetLinkId(HtmlDocument doc)
		{
			foreach (HtmlElement elem in doc.GetElementsByTagName("li"))
			{
				var li = elem.DomElement as HTMLLIElement;
				if (li != null)
				{
					if (li.className == "link-next")
					{
						_nextImageId = GetImageId(li);
					}
					else if (li.className == "link-prev")
					{
						_prevImageId = GetImageId(li);
					}
					Marshal.FinalReleaseComObject(li);
				}
			}
		}

		// 画像の場所を取得
		private string GetImageSource(HtmlDocument doc)
		{
			HTMLDivElement d = null;
			IHTMLImgElement i = null;

			var result =
				from elem in doc.GetElementsByTagName("div").OfType<HtmlElement>()
				let div = d = elem.DomElement as HTMLDivElement
				where div != null && div.className == "photo-detail"
				let img = i = div.firstChild as IHTMLImgElement
				where img != null && img.src.ToLower().EndsWith(".jpg")
				select img.src;

			if (d != null)
				Marshal.FinalReleaseComObject(d);
			if (i != null)
				Marshal.FinalReleaseComObject(i);

			return result.FirstOrDefault();

			//foreach (HtmlElement elem in doc.GetElementsByTagName("div"))
			//{
			//    var div = elem.DomElement as HTMLDivElement;
			//    if (div != null && div.className == "photo-detail")
			//    {
			//        var img = div.firstChild as IHTMLImgElement;
			//        if (img != null && img.src.ToLower().EndsWith("jpg"))
			//        {
			//            string src = img.src;
			//            Marshal.FinalReleaseComObject(img);
			//            Marshal.FinalReleaseComObject(div);
			//            return src;
			//        }
			//        Marshal.FinalReleaseComObject(div);
			//    }
			//}
			//return string.Empty;
		}

		// リンクID（イメージID）取得
		private int GetImageId(HTMLLIElement element)
		{
			int imageId = 0;
			var anchor = element.firstChild as IHTMLAnchorElement;
			if (anchor != null)
			{
				int.TryParse(Regex.Match(anchor.href, "(.*)(=)(.*)").Groups[3].Value, out imageId);
				//int.TryParse(Regex.Match(anchor.href, ".*=(?<img_id>.*)").Result("img_id"), out imageId);
				Marshal.FinalReleaseComObject(anchor);
			}
			return imageId;
		}

		// ページ移動
		private void Navigate(string volume, int imageId)
		{
			string url = string.Format(volume, imageId);
			wbBase.Navigate(url);
		}
	}
}
