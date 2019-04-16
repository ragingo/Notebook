using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace Bizeen
{
	public partial class MainForm : Form
	{
		private bool _isDown = false;
		private bool _loadCompleted = false;
		private Point _pos;
		private const string ID_IMG_CLOSE = "IMG_CLOSE_BUTTON";
		private HtmlElementEventHandler _clickHandler = null;

		public MainForm()
		{
			InitializeComponent();
			wbBizeen.DocumentText = File.ReadAllText("resources\\page.htm");
			wbBizeen.Document.MouseDown += new HtmlElementEventHandler(Document_MouseDown);
			wbBizeen.Document.MouseMove += new HtmlElementEventHandler(Document_MouseMove);
			wbBizeen.Document.MouseUp += new HtmlElementEventHandler(Document_MouseUp);
			wbBizeen.DocumentCompleted += new WebBrowserDocumentCompletedEventHandler(wbBizeen_DocumentCompleted);
			wbBizeen.NewWindow += new CancelEventHandler(wbBizeen_NewWindow);
			wbBizeen.Navigating += new WebBrowserNavigatingEventHandler(wbBizeen_Navigating);
			_clickHandler = new HtmlElementEventHandler(CloseButton_Click);

			this.Cursor = Cursors.Hand;
			Cursor.Current = Cursors.Hand;
		}

		void CloseButton_Click(object sender, HtmlElementEventArgs e)
		{
			this.Close();
		}

		private void wbBizeen_Navigating(object sender, WebBrowserNavigatingEventArgs e)
		{
			e.Cancel = _loadCompleted;
		}

		private void wbBizeen_NewWindow(object sender, CancelEventArgs e)
		{
			e.Cancel = _loadCompleted;
		}

		private void wbBizeen_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e)
		{
			if (_clickHandler != null)
			{
				wbBizeen.Document.GetElementById(ID_IMG_CLOSE).Click += _clickHandler;
			}
			_clickHandler = null;
			_loadCompleted = true;
		}

		private void Document_MouseDown(object sender, HtmlElementEventArgs e)
		{
			_isDown = true;
			_pos = e.MousePosition;
		}

		private void Document_MouseMove(object sender, HtmlElementEventArgs e)
		{
			if (_isDown)
			{
				this.Left += e.MousePosition.X - _pos.X;
				this.Top += e.MousePosition.Y - _pos.Y;
			}
		}

		private void Document_MouseUp(object sender, HtmlElementEventArgs e)
		{
			_isDown = false;
		}
	}
}
