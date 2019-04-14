namespace RegexChecker
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
			if(disposing && (components != null))
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
			this.splitContainer1 = new System.Windows.Forms.SplitContainer();
			this.txtInput = new System.Windows.Forms.TextBox();
			this.splitContainer2 = new System.Windows.Forms.SplitContainer();
			this.txtPattern = new System.Windows.Forms.TextBox();
			this.txtResult = new System.Windows.Forms.TextBox();
			this.menuStrip1 = new System.Windows.Forms.MenuStrip();
			this.tsmiSetting = new System.Windows.Forms.ToolStripMenuItem();
			this.tsmiFontSetting = new System.Windows.Forms.ToolStripMenuItem();
			this.編集ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.tsmiAllSelect = new System.Windows.Forms.ToolStripMenuItem();
			this.statusStrip1 = new System.Windows.Forms.StatusStrip();
			this.slblResult = new System.Windows.Forms.ToolStripStatusLabel();
			this.splitContainer1.Panel1.SuspendLayout();
			this.splitContainer1.Panel2.SuspendLayout();
			this.splitContainer1.SuspendLayout();
			this.splitContainer2.Panel1.SuspendLayout();
			this.splitContainer2.Panel2.SuspendLayout();
			this.splitContainer2.SuspendLayout();
			this.menuStrip1.SuspendLayout();
			this.statusStrip1.SuspendLayout();
			this.SuspendLayout();
			// 
			// splitContainer1
			// 
			this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitContainer1.Location = new System.Drawing.Point(0, 26);
			this.splitContainer1.Name = "splitContainer1";
			this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// splitContainer1.Panel1
			// 
			this.splitContainer1.Panel1.Controls.Add(this.txtInput);
			// 
			// splitContainer1.Panel2
			// 
			this.splitContainer1.Panel2.Controls.Add(this.splitContainer2);
			this.splitContainer1.Size = new System.Drawing.Size(776, 463);
			this.splitContainer1.SplitterDistance = 166;
			this.splitContainer1.TabIndex = 0;
			this.splitContainer1.TabStop = false;
			// 
			// txtInput
			// 
			this.txtInput.Dock = System.Windows.Forms.DockStyle.Fill;
			this.txtInput.Location = new System.Drawing.Point(0, 0);
			this.txtInput.Multiline = true;
			this.txtInput.Name = "txtInput";
			this.txtInput.Size = new System.Drawing.Size(776, 166);
			this.txtInput.TabIndex = 0;
			// 
			// splitContainer2
			// 
			this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitContainer2.Location = new System.Drawing.Point(0, 0);
			this.splitContainer2.Name = "splitContainer2";
			this.splitContainer2.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// splitContainer2.Panel1
			// 
			this.splitContainer2.Panel1.Controls.Add(this.txtPattern);
			// 
			// splitContainer2.Panel2
			// 
			this.splitContainer2.Panel2.Controls.Add(this.txtResult);
			this.splitContainer2.Size = new System.Drawing.Size(776, 293);
			this.splitContainer2.SplitterDistance = 146;
			this.splitContainer2.TabIndex = 0;
			this.splitContainer2.TabStop = false;
			// 
			// txtPattern
			// 
			this.txtPattern.Dock = System.Windows.Forms.DockStyle.Fill;
			this.txtPattern.Location = new System.Drawing.Point(0, 0);
			this.txtPattern.Multiline = true;
			this.txtPattern.Name = "txtPattern";
			this.txtPattern.Size = new System.Drawing.Size(776, 146);
			this.txtPattern.TabIndex = 0;
			// 
			// txtResult
			// 
			this.txtResult.Dock = System.Windows.Forms.DockStyle.Fill;
			this.txtResult.Location = new System.Drawing.Point(0, 0);
			this.txtResult.Multiline = true;
			this.txtResult.Name = "txtResult";
			this.txtResult.Size = new System.Drawing.Size(776, 143);
			this.txtResult.TabIndex = 0;
			// 
			// menuStrip1
			// 
			this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsmiSetting,
            this.編集ToolStripMenuItem});
			this.menuStrip1.Location = new System.Drawing.Point(0, 0);
			this.menuStrip1.Name = "menuStrip1";
			this.menuStrip1.Size = new System.Drawing.Size(776, 26);
			this.menuStrip1.TabIndex = 1;
			this.menuStrip1.Text = "menuStrip1";
			// 
			// tsmiSetting
			// 
			this.tsmiSetting.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsmiFontSetting});
			this.tsmiSetting.Name = "tsmiSetting";
			this.tsmiSetting.Size = new System.Drawing.Size(44, 22);
			this.tsmiSetting.Text = "設定";
			// 
			// tsmiFontSetting
			// 
			this.tsmiFontSetting.Name = "tsmiFontSetting";
			this.tsmiFontSetting.Size = new System.Drawing.Size(152, 22);
			this.tsmiFontSetting.Text = "フォント";
			this.tsmiFontSetting.Click += new System.EventHandler(this.tsmiFontSetting_Click);
			// 
			// 編集ToolStripMenuItem
			// 
			this.編集ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tsmiAllSelect});
			this.編集ToolStripMenuItem.Name = "編集ToolStripMenuItem";
			this.編集ToolStripMenuItem.Size = new System.Drawing.Size(44, 22);
			this.編集ToolStripMenuItem.Text = "編集";
			// 
			// tsmiAllSelect
			// 
			this.tsmiAllSelect.Name = "tsmiAllSelect";
			this.tsmiAllSelect.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.A)));
			this.tsmiAllSelect.Size = new System.Drawing.Size(171, 22);
			this.tsmiAllSelect.Text = "全て選択";
			this.tsmiAllSelect.Click += new System.EventHandler(this.tsmiAllSelect_Click);
			// 
			// statusStrip1
			// 
			this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.slblResult});
			this.statusStrip1.Location = new System.Drawing.Point(0, 467);
			this.statusStrip1.Name = "statusStrip1";
			this.statusStrip1.Size = new System.Drawing.Size(776, 22);
			this.statusStrip1.TabIndex = 2;
			this.statusStrip1.Text = "statusStrip1";
			// 
			// slblResult
			// 
			this.slblResult.Font = new System.Drawing.Font("ＭＳ ゴシック", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.slblResult.Name = "slblResult";
			this.slblResult.Size = new System.Drawing.Size(89, 17);
			this.slblResult.Text = "              ";
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(776, 489);
			this.Controls.Add(this.statusStrip1);
			this.Controls.Add(this.splitContainer1);
			this.Controls.Add(this.menuStrip1);
			this.MainMenuStrip = this.menuStrip1;
			this.Name = "MainForm";
			this.ShowIcon = false;
			this.Text = "正規表現チェッカー";
			this.splitContainer1.Panel1.ResumeLayout(false);
			this.splitContainer1.Panel1.PerformLayout();
			this.splitContainer1.Panel2.ResumeLayout(false);
			this.splitContainer1.ResumeLayout(false);
			this.splitContainer2.Panel1.ResumeLayout(false);
			this.splitContainer2.Panel1.PerformLayout();
			this.splitContainer2.Panel2.ResumeLayout(false);
			this.splitContainer2.Panel2.PerformLayout();
			this.splitContainer2.ResumeLayout(false);
			this.menuStrip1.ResumeLayout(false);
			this.menuStrip1.PerformLayout();
			this.statusStrip1.ResumeLayout(false);
			this.statusStrip1.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.SplitContainer splitContainer1;
		private System.Windows.Forms.SplitContainer splitContainer2;
		private System.Windows.Forms.TextBox txtInput;
		private System.Windows.Forms.TextBox txtPattern;
		private System.Windows.Forms.TextBox txtResult;
		private System.Windows.Forms.MenuStrip menuStrip1;
		private System.Windows.Forms.ToolStripMenuItem tsmiSetting;
		private System.Windows.Forms.ToolStripMenuItem tsmiFontSetting;
		private System.Windows.Forms.ToolStripMenuItem 編集ToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem tsmiAllSelect;
		private System.Windows.Forms.StatusStrip statusStrip1;
		private System.Windows.Forms.ToolStripStatusLabel slblResult;
	}
}

