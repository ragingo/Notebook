namespace CSharpScriptCompiler
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
			this.lstRefAsms = new System.Windows.Forms.CheckedListBox();
			this.txtSource = new System.Windows.Forms.RichTextBox();
			this.btnCompile = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// lstRefAsms
			// 
			this.lstRefAsms.FormattingEnabled = true;
			this.lstRefAsms.Location = new System.Drawing.Point(389, 12);
			this.lstRefAsms.Name = "lstRefAsms";
			this.lstRefAsms.Size = new System.Drawing.Size(180, 438);
			this.lstRefAsms.TabIndex = 0;
			// 
			// txtSource
			// 
			this.txtSource.Location = new System.Drawing.Point(13, 12);
			this.txtSource.Name = "txtSource";
			this.txtSource.Size = new System.Drawing.Size(370, 438);
			this.txtSource.TabIndex = 1;
			this.txtSource.Text = "";
			// 
			// btnCompile
			// 
			this.btnCompile.Location = new System.Drawing.Point(13, 456);
			this.btnCompile.Name = "btnCompile";
			this.btnCompile.Size = new System.Drawing.Size(75, 21);
			this.btnCompile.TabIndex = 2;
			this.btnCompile.Text = "Compile";
			this.btnCompile.UseVisualStyleBackColor = true;
			this.btnCompile.Click += new System.EventHandler(this.btnCompile_Click);
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(581, 482);
			this.Controls.Add(this.btnCompile);
			this.Controls.Add(this.txtSource);
			this.Controls.Add(this.lstRefAsms);
			this.Name = "MainForm";
			this.Text = "C# Script Compiler";
			this.Load += new System.EventHandler(this.MainForm_Load);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.CheckedListBox lstRefAsms;
		private System.Windows.Forms.RichTextBox txtSource;
		private System.Windows.Forms.Button btnCompile;
	}
}

