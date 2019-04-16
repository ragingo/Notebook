namespace Bizeen
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
			this.wbBizeen = new System.Windows.Forms.WebBrowser();
			this.SuspendLayout();
			// 
			// wbBizeen
			// 
			this.wbBizeen.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.wbBizeen.Location = new System.Drawing.Point(9, 26);
			this.wbBizeen.Margin = new System.Windows.Forms.Padding(0);
			this.wbBizeen.MinimumSize = new System.Drawing.Size(20, 20);
			this.wbBizeen.Name = "wbBizeen";
			this.wbBizeen.ScrollBarsEnabled = false;
			this.wbBizeen.Size = new System.Drawing.Size(304, 418);
			this.wbBizeen.TabIndex = 1;
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.Color.White;
			this.ClientSize = new System.Drawing.Size(322, 453);
			this.Controls.Add(this.wbBizeen);
			this.DoubleBuffered = true;
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "MainForm";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Show;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.TopMost = true;
			this.TransparencyKey = System.Drawing.Color.White;
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.WebBrowser wbBizeen;
	}
}

