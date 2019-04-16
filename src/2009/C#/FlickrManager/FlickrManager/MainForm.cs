using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace FlickrManager
{
	public partial class MainForm : Form
	{
		public MainForm()
		{
			InitializeComponent();
		}

		private void btnUpload_Click(object sender, EventArgs e)
		{
			using (UploadForm form = new UploadForm())
			{
				form.ShowDialog();
			}
		}
	}
}
