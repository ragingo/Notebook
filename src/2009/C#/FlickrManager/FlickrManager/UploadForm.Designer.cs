namespace FlickrManager
{
	partial class UploadForm
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.lvPictures = new System.Windows.Forms.ListView();
			this.btnUpload = new System.Windows.Forms.Button();
			this.btnOpenFileDlg = new System.Windows.Forms.Button();
			this.bgwUpload = new System.ComponentModel.BackgroundWorker();
			this.pbReport = new System.Windows.Forms.ProgressBar();
			this.SuspendLayout();
			// 
			// lvPictures
			// 
			this.lvPictures.AllowDrop = true;
			this.lvPictures.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.lvPictures.Location = new System.Drawing.Point(12, 12);
			this.lvPictures.Name = "lvPictures";
			this.lvPictures.Size = new System.Drawing.Size(648, 345);
			this.lvPictures.TabIndex = 0;
			this.lvPictures.UseCompatibleStateImageBehavior = false;
			this.lvPictures.DragDrop += new System.Windows.Forms.DragEventHandler(this.lvPictures_DragDrop);
			this.lvPictures.DragEnter += new System.Windows.Forms.DragEventHandler(this.lvPictures_DragEnter);
			// 
			// btnUpload
			// 
			this.btnUpload.Location = new System.Drawing.Point(12, 363);
			this.btnUpload.Name = "btnUpload";
			this.btnUpload.Size = new System.Drawing.Size(92, 34);
			this.btnUpload.TabIndex = 1;
			this.btnUpload.Text = "アップロード";
			this.btnUpload.UseVisualStyleBackColor = true;
			this.btnUpload.Click += new System.EventHandler(this.btnUpload_Click);
			// 
			// btnOpenFileDlg
			// 
			this.btnOpenFileDlg.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btnOpenFileDlg.Location = new System.Drawing.Point(626, 363);
			this.btnOpenFileDlg.Name = "btnOpenFileDlg";
			this.btnOpenFileDlg.Size = new System.Drawing.Size(34, 23);
			this.btnOpenFileDlg.TabIndex = 2;
			this.btnOpenFileDlg.Text = "...";
			this.btnOpenFileDlg.UseVisualStyleBackColor = true;
			this.btnOpenFileDlg.Click += new System.EventHandler(this.btnOpenFileDlg_Click);
			// 
			// bgwUpload
			// 
			this.bgwUpload.WorkerReportsProgress = true;
			this.bgwUpload.WorkerSupportsCancellation = true;
			this.bgwUpload.DoWork += new System.ComponentModel.DoWorkEventHandler(this.bgwUpload_DoWork);
			this.bgwUpload.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.bgwUpload_RunWorkerCompleted);
			this.bgwUpload.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.bgwUpload_ProgressChanged);
			// 
			// pbReport
			// 
			this.pbReport.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.pbReport.Location = new System.Drawing.Point(13, 547);
			this.pbReport.Name = "pbReport";
			this.pbReport.Size = new System.Drawing.Size(308, 23);
			this.pbReport.TabIndex = 3;
			// 
			// UploadForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(672, 582);
			this.Controls.Add(this.pbReport);
			this.Controls.Add(this.btnOpenFileDlg);
			this.Controls.Add(this.btnUpload);
			this.Controls.Add(this.lvPictures);
			this.Name = "UploadForm";
			this.Text = "UploadForm";
			this.Load += new System.EventHandler(this.UploadForm_Load);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.ListView lvPictures;
		private System.Windows.Forms.Button btnUpload;
		private System.Windows.Forms.Button btnOpenFileDlg;
		private System.ComponentModel.BackgroundWorker bgwUpload;
		private System.Windows.Forms.ProgressBar pbReport;
	}
}