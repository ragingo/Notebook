namespace WindowsInformation
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
			this.tvClassList = new System.Windows.Forms.TreeView();
			this.dgvDetail = new System.Windows.Forms.DataGridView();
			this.splitContainer1.Panel1.SuspendLayout();
			this.splitContainer1.Panel2.SuspendLayout();
			this.splitContainer1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.dgvDetail)).BeginInit();
			this.SuspendLayout();
			// 
			// splitContainer1
			// 
			this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitContainer1.Location = new System.Drawing.Point(0, 0);
			this.splitContainer1.Name = "splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this.splitContainer1.Panel1.Controls.Add(this.tvClassList);
			// 
			// splitContainer1.Panel2
			// 
			this.splitContainer1.Panel2.Controls.Add(this.dgvDetail);
			this.splitContainer1.Size = new System.Drawing.Size(880, 581);
			this.splitContainer1.SplitterDistance = 220;
			this.splitContainer1.TabIndex = 0;
			// 
			// tvClassList
			// 
			this.tvClassList.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tvClassList.Location = new System.Drawing.Point(0, 0);
			this.tvClassList.Name = "tvClassList";
			this.tvClassList.Size = new System.Drawing.Size(220, 581);
			this.tvClassList.TabIndex = 0;
			// 
			// dgvDetail
			// 
			this.dgvDetail.AllowUserToAddRows = false;
			this.dgvDetail.AllowUserToDeleteRows = false;
			this.dgvDetail.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.dgvDetail.Dock = System.Windows.Forms.DockStyle.Fill;
			this.dgvDetail.Location = new System.Drawing.Point(0, 0);
			this.dgvDetail.Name = "dgvDetail";
			this.dgvDetail.ReadOnly = true;
			this.dgvDetail.RowTemplate.Height = 21;
			this.dgvDetail.Size = new System.Drawing.Size(656, 581);
			this.dgvDetail.TabIndex = 0;
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(880, 581);
			this.Controls.Add(this.splitContainer1);
			this.Name = "MainForm";
			this.Text = "Windows Information";
			this.splitContainer1.Panel1.ResumeLayout(false);
			this.splitContainer1.Panel2.ResumeLayout(false);
			this.splitContainer1.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.dgvDetail)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.SplitContainer splitContainer1;
		private System.Windows.Forms.TreeView tvClassList;
		private System.Windows.Forms.DataGridView dgvDetail;

	}
}

