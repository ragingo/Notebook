using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using FlickrNet;
using System.Diagnostics;

namespace FlickrManager
{
	public partial class UploadForm : Form
	{
		private string _frob;

		public UploadForm()
		{
			InitializeComponent();

			lvPictures.LargeImageList = new ImageList();
		}


		// 画面ロード
		private void UploadForm_Load(object sender, EventArgs e)
		{
			Cursor.Current = Cursors.WaitCursor;

			Flickr f = new Flickr(Constants.ApiKey, Constants.SharedSecret);
			_frob = f.AuthGetFrob();
			string url = f.AuthCalcUrl(_frob, AuthLevel.Write);
			Process.Start(url);

			Cursor.Current = Cursors.Default;
		}

		// 送信ボタン
		private void btnUpload_Click(object sender, EventArgs e)
		{
			try
			{
				Flickr f = new Flickr(Constants.ApiKey, Constants.SharedSecret);
				Auth a = f.AuthGetToken(_frob);
				f = new Flickr(Constants.ApiKey, Constants.SharedSecret, a.Token);

				Cursor.Current = Cursors.WaitCursor;
				bgwUpload.RunWorkerAsync(f);
			}
			catch (FlickrException fe)
			{
				MessageBox.Show(fe.Message);
			}
		}

		// 「ファイルを開く」ダイアログ
		private void btnOpenFileDlg_Click(object sender, EventArgs e)
		{
			using (OpenFileDialog dlg = new OpenFileDialog())
			{
				dlg.Filter = "ImageFiles|*.jpg;*.jpeg;*.gif;*.png;*.bmp;*.tiff;*.ico";
				dlg.Multiselect = true;
				if (dlg.ShowDialog() == DialogResult.OK)
				{
					foreach (string path in dlg.FileNames)
					{
						AddListViewItem(path);
					}
				}
			}
		}

		// ドラッグ状態で領域に侵入
		private void lvPictures_DragEnter(object sender, DragEventArgs e)
		{
			if (!e.Data.GetDataPresent(DataFormats.FileDrop))
			{
				return;
			}

			bool result = ((string[])e.Data.GetData(DataFormats.FileDrop)).All(pred =>
			{
				return
					pred.EndsWith(".jpg") ||
					pred.EndsWith(".jpeg") ||
					pred.EndsWith(".gif") ||
					pred.EndsWith(".png") ||
					pred.EndsWith(".bmp") ||
					pred.EndsWith(".tiff") ||
					pred.EndsWith(".ico");
			});

			if (result)
			{
				e.Effect = DragDropEffects.Move;
			}
			else
			{
				e.Effect = DragDropEffects.None;
			}
		}

		// ドロップ時
		private void lvPictures_DragDrop(object sender, DragEventArgs e)
		{
			foreach (string path in (string[])e.Data.GetData(DataFormats.FileDrop))
			{
				AddListViewItem(path);
			}
		}

		// ListViewにアイテムを追加
		private void AddListViewItem(string path)
		{
			lvPictures.LargeImageList.Images.Add(Image.FromFile(path));
			lvPictures.Items.Add(path, Path.GetFileName(path), lvPictures.LargeImageList.Images.Count - 1);
		}

		// 非同期処理（送信処理）
		private void bgwUpload_DoWork(object sender, DoWorkEventArgs e)
		{
			List<ListViewItem> items = new List<ListViewItem>();
			lvPictures.Invoke((Action)(
				() =>
				{
					items.AddRange(lvPictures.Items.Cast<ListViewItem>());
				}
			));

			Flickr f = e.Argument as Flickr;

			int progress = 0;
			int count = items.Count;
			Dictionary<string, string> result = new Dictionary<string, string>();
			foreach (string path in items.Select(item => item.Name))
			{
				string photoID = f.UploadPicture(
					path,			// 場所
					"title",		// 題
					"description",	// 説明
					"tag",			// 検索タグ
					false,			// 全体に公開？
					false,			// 家族まで公開？
					false			// 友達まで公開？
				);
				result[path] = photoID;

				LogWriter.Instance.TraceData(TraceEventType.Information, 1, photoID);

				double per = (double)++progress / (double)count;
				bgwUpload.ReportProgress((int)(per * 100.0));
			}

			e.Result = result;
		}

		// 進捗状況
		private void bgwUpload_ProgressChanged(object sender, ProgressChangedEventArgs e)
		{
			pbReport.Value = e.ProgressPercentage;
		}

		// 処理完了
		private void bgwUpload_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
		{
			Cursor.Current = Cursors.Default;
			MessageBox.Show("送信完了です。");
			//e.Result as Dictionary<string, string>
		}
	}
}
