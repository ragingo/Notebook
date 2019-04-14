namespace TokyoCarEventGirlViewer
{
	partial class MainForm
	{
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		/// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows フォーム デザイナで生成されたコード

		/// <summary>
		/// デザイナ サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディタで変更しないでください。
		/// </summary>
		private void InitializeComponent()
		{
			this.wbBase = new System.Windows.Forms.WebBrowser();
			this.wbExtractImage = new System.Windows.Forms.WebBrowser();
			this.txtBase = new System.Windows.Forms.TextBox();
			this.txtExtractImagewbExtractImage = new System.Windows.Forms.TextBox();
			this.btnPrev = new System.Windows.Forms.Button();
			this.btnNext = new System.Windows.Forms.Button();
			this.rbVol1 = new System.Windows.Forms.RadioButton();
			this.rbVol2 = new System.Windows.Forms.RadioButton();
			this.rbVol3 = new System.Windows.Forms.RadioButton();
			this.SuspendLayout();
			// 
			// wbBase
			// 
			this.wbBase.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)));
			this.wbBase.Location = new System.Drawing.Point(13, 63);
			this.wbBase.MinimumSize = new System.Drawing.Size(20, 20);
			this.wbBase.Name = "wbBase";
			this.wbBase.Size = new System.Drawing.Size(484, 589);
			this.wbBase.TabIndex = 0;
			this.wbBase.DocumentCompleted += new System.Windows.Forms.WebBrowserDocumentCompletedEventHandler(this.wbBase_DocumentCompleted);
			// 
			// wbExtractImage
			// 
			this.wbExtractImage.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.wbExtractImage.Location = new System.Drawing.Point(503, 41);
			this.wbExtractImage.MinimumSize = new System.Drawing.Size(20, 20);
			this.wbExtractImage.Name = "wbExtractImage";
			this.wbExtractImage.Size = new System.Drawing.Size(493, 640);
			this.wbExtractImage.TabIndex = 1;
			// 
			// txtBase
			// 
			this.txtBase.Location = new System.Drawing.Point(13, 13);
			this.txtBase.Name = "txtBase";
			this.txtBase.ReadOnly = true;
			this.txtBase.Size = new System.Drawing.Size(484, 19);
			this.txtBase.TabIndex = 2;
			// 
			// txtExtractImagewbExtractImage
			// 
			this.txtExtractImagewbExtractImage.Location = new System.Drawing.Point(503, 13);
			this.txtExtractImagewbExtractImage.Name = "txtExtractImagewbExtractImage";
			this.txtExtractImagewbExtractImage.ReadOnly = true;
			this.txtExtractImagewbExtractImage.Size = new System.Drawing.Size(493, 19);
			this.txtExtractImagewbExtractImage.TabIndex = 2;
			// 
			// btnPrev
			// 
			this.btnPrev.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.btnPrev.Location = new System.Drawing.Point(13, 658);
			this.btnPrev.Name = "btnPrev";
			this.btnPrev.Size = new System.Drawing.Size(75, 23);
			this.btnPrev.TabIndex = 3;
			this.btnPrev.Text = "前へ(&P)";
			this.btnPrev.UseVisualStyleBackColor = true;
			this.btnPrev.Click += new System.EventHandler(this.btnPrev_Click);
			// 
			// btnNext
			// 
			this.btnNext.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.btnNext.Location = new System.Drawing.Point(422, 658);
			this.btnNext.Name = "btnNext";
			this.btnNext.Size = new System.Drawing.Size(75, 23);
			this.btnNext.TabIndex = 3;
			this.btnNext.Text = "次へ(&N)";
			this.btnNext.UseVisualStyleBackColor = true;
			this.btnNext.Click += new System.EventHandler(this.btnNext_Click);
			// 
			// rbVol1
			// 
			this.rbVol1.AutoSize = true;
			this.rbVol1.Checked = true;
			this.rbVol1.Location = new System.Drawing.Point(13, 39);
			this.rbVol1.Name = "rbVol1";
			this.rbVol1.Size = new System.Drawing.Size(83, 16);
			this.rbVol1.TabIndex = 4;
			this.rbVol1.TabStop = true;
			this.rbVol1.Text = "Volume1(&A)";
			this.rbVol1.UseVisualStyleBackColor = true;
			this.rbVol1.CheckedChanged += new System.EventHandler(this.RadioButton_CheckedChanged);
			// 
			// rbVol2
			// 
			this.rbVol2.AutoSize = true;
			this.rbVol2.Location = new System.Drawing.Point(102, 39);
			this.rbVol2.Name = "rbVol2";
			this.rbVol2.Size = new System.Drawing.Size(83, 16);
			this.rbVol2.TabIndex = 4;
			this.rbVol2.Text = "Volume2(&B)";
			this.rbVol2.UseVisualStyleBackColor = true;
			this.rbVol2.CheckedChanged += new System.EventHandler(this.RadioButton_CheckedChanged);
			// 
			// rbVol3
			// 
			this.rbVol3.AutoSize = true;
			this.rbVol3.Location = new System.Drawing.Point(191, 39);
			this.rbVol3.Name = "rbVol3";
			this.rbVol3.Size = new System.Drawing.Size(83, 16);
			this.rbVol3.TabIndex = 4;
			this.rbVol3.Text = "Volume3(&C)";
			this.rbVol3.UseVisualStyleBackColor = true;
			this.rbVol3.CheckedChanged += new System.EventHandler(this.RadioButton_CheckedChanged);
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(1008, 701);
			this.Controls.Add(this.rbVol3);
			this.Controls.Add(this.rbVol2);
			this.Controls.Add(this.rbVol1);
			this.Controls.Add(this.btnNext);
			this.Controls.Add(this.btnPrev);
			this.Controls.Add(this.txtExtractImagewbExtractImage);
			this.Controls.Add(this.txtBase);
			this.Controls.Add(this.wbExtractImage);
			this.Controls.Add(this.wbBase);
			this.MaximizeBox = false;
			this.Name = "MainForm";
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
			this.Text = "TokyoCarEventGirlViewer";
			this.Load += new System.EventHandler(this.MainForm_Load);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.WebBrowser wbBase;
		private System.Windows.Forms.WebBrowser wbExtractImage;
		private System.Windows.Forms.TextBox txtBase;
		private System.Windows.Forms.TextBox txtExtractImagewbExtractImage;
		private System.Windows.Forms.Button btnPrev;
		private System.Windows.Forms.Button btnNext;
		private System.Windows.Forms.RadioButton rbVol1;
		private System.Windows.Forms.RadioButton rbVol2;
		private System.Windows.Forms.RadioButton rbVol3;
	}
}

