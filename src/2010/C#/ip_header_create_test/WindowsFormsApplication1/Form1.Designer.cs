namespace WindowsFormsApplication1
{
	partial class Form1
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
			this.button1 = new System.Windows.Forms.Button();
			this.lblVersion = new System.Windows.Forms.Label();
			this.txtVersion = new System.Windows.Forms.TextBox();
			this.lblHeaderLength = new System.Windows.Forms.Label();
			this.txtHeaderLength = new System.Windows.Forms.TextBox();
			this.lblTos = new System.Windows.Forms.Label();
			this.lblPrecedence = new System.Windows.Forms.Label();
			this.lblDelay = new System.Windows.Forms.Label();
			this.txtTosDelay = new System.Windows.Forms.TextBox();
			this.lblThroughput = new System.Windows.Forms.Label();
			this.txtTosThroughput = new System.Windows.Forms.TextBox();
			this.lblReliability = new System.Windows.Forms.Label();
			this.txtTosReliability = new System.Windows.Forms.TextBox();
			this.lblReserved = new System.Windows.Forms.Label();
			this.txtTosReserved = new System.Windows.Forms.TextBox();
			this.lblTotalLength = new System.Windows.Forms.Label();
			this.txtTotalLength = new System.Windows.Forms.TextBox();
			this.lblIdentification = new System.Windows.Forms.Label();
			this.txtIdentification = new System.Windows.Forms.TextBox();
			this.cmbTosPrecedence = new System.Windows.Forms.ComboBox();
			this.SuspendLayout();
			// 
			// button1
			// 
			this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.button1.Location = new System.Drawing.Point(12, 557);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(75, 23);
			this.button1.TabIndex = 0;
			this.button1.Text = "button1";
			this.button1.UseVisualStyleBackColor = true;
			// 
			// lblVersion
			// 
			this.lblVersion.AutoSize = true;
			this.lblVersion.Location = new System.Drawing.Point(12, 13);
			this.lblVersion.Name = "lblVersion";
			this.lblVersion.Size = new System.Drawing.Size(44, 12);
			this.lblVersion.TabIndex = 1;
			this.lblVersion.Text = "Version";
			// 
			// txtVersion
			// 
			this.txtVersion.Location = new System.Drawing.Point(93, 10);
			this.txtVersion.Name = "txtVersion";
			this.txtVersion.Size = new System.Drawing.Size(100, 19);
			this.txtVersion.TabIndex = 2;
			// 
			// lblHeaderLength
			// 
			this.lblHeaderLength.AutoSize = true;
			this.lblHeaderLength.Location = new System.Drawing.Point(12, 38);
			this.lblHeaderLength.Name = "lblHeaderLength";
			this.lblHeaderLength.Size = new System.Drawing.Size(75, 12);
			this.lblHeaderLength.TabIndex = 1;
			this.lblHeaderLength.Text = "HeaderLength";
			// 
			// txtHeaderLength
			// 
			this.txtHeaderLength.Location = new System.Drawing.Point(93, 35);
			this.txtHeaderLength.Name = "txtHeaderLength";
			this.txtHeaderLength.Size = new System.Drawing.Size(100, 19);
			this.txtHeaderLength.TabIndex = 2;
			// 
			// lblTos
			// 
			this.lblTos.AutoSize = true;
			this.lblTos.Location = new System.Drawing.Point(36, 63);
			this.lblTos.Name = "lblTos";
			this.lblTos.Size = new System.Drawing.Size(88, 12);
			this.lblTos.TabIndex = 1;
			this.lblTos.Text = "Type Of Service";
			// 
			// lblPrecedence
			// 
			this.lblPrecedence.AutoSize = true;
			this.lblPrecedence.Location = new System.Drawing.Point(60, 86);
			this.lblPrecedence.Name = "lblPrecedence";
			this.lblPrecedence.Size = new System.Drawing.Size(64, 12);
			this.lblPrecedence.TabIndex = 1;
			this.lblPrecedence.Text = "Precedence";
			// 
			// lblDelay
			// 
			this.lblDelay.AutoSize = true;
			this.lblDelay.Location = new System.Drawing.Point(60, 111);
			this.lblDelay.Name = "lblDelay";
			this.lblDelay.Size = new System.Drawing.Size(34, 12);
			this.lblDelay.TabIndex = 1;
			this.lblDelay.Text = "Delay";
			// 
			// txtTosDelay
			// 
			this.txtTosDelay.Location = new System.Drawing.Point(141, 108);
			this.txtTosDelay.Name = "txtTosDelay";
			this.txtTosDelay.Size = new System.Drawing.Size(50, 19);
			this.txtTosDelay.TabIndex = 2;
			// 
			// lblThroughput
			// 
			this.lblThroughput.AutoSize = true;
			this.lblThroughput.Location = new System.Drawing.Point(60, 136);
			this.lblThroughput.Name = "lblThroughput";
			this.lblThroughput.Size = new System.Drawing.Size(62, 12);
			this.lblThroughput.TabIndex = 1;
			this.lblThroughput.Text = "Throughput";
			// 
			// txtTosThroughput
			// 
			this.txtTosThroughput.Location = new System.Drawing.Point(141, 133);
			this.txtTosThroughput.Name = "txtTosThroughput";
			this.txtTosThroughput.Size = new System.Drawing.Size(50, 19);
			this.txtTosThroughput.TabIndex = 2;
			// 
			// lblReliability
			// 
			this.lblReliability.AutoSize = true;
			this.lblReliability.Location = new System.Drawing.Point(60, 161);
			this.lblReliability.Name = "lblReliability";
			this.lblReliability.Size = new System.Drawing.Size(56, 12);
			this.lblReliability.TabIndex = 1;
			this.lblReliability.Text = "Reliability";
			// 
			// txtTosReliability
			// 
			this.txtTosReliability.Location = new System.Drawing.Point(141, 158);
			this.txtTosReliability.Name = "txtTosReliability";
			this.txtTosReliability.Size = new System.Drawing.Size(50, 19);
			this.txtTosReliability.TabIndex = 2;
			// 
			// lblReserved
			// 
			this.lblReserved.AutoSize = true;
			this.lblReserved.Location = new System.Drawing.Point(60, 186);
			this.lblReserved.Name = "lblReserved";
			this.lblReserved.Size = new System.Drawing.Size(53, 12);
			this.lblReserved.TabIndex = 1;
			this.lblReserved.Text = "Reserved";
			// 
			// txtTosReserved
			// 
			this.txtTosReserved.Location = new System.Drawing.Point(141, 183);
			this.txtTosReserved.Name = "txtTosReserved";
			this.txtTosReserved.Size = new System.Drawing.Size(50, 19);
			this.txtTosReserved.TabIndex = 2;
			// 
			// lblTotalLength
			// 
			this.lblTotalLength.AutoSize = true;
			this.lblTotalLength.Location = new System.Drawing.Point(12, 211);
			this.lblTotalLength.Name = "lblTotalLength";
			this.lblTotalLength.Size = new System.Drawing.Size(65, 12);
			this.lblTotalLength.TabIndex = 1;
			this.lblTotalLength.Text = "TotalLength";
			// 
			// txtTotalLength
			// 
			this.txtTotalLength.Location = new System.Drawing.Point(93, 208);
			this.txtTotalLength.Name = "txtTotalLength";
			this.txtTotalLength.Size = new System.Drawing.Size(100, 19);
			this.txtTotalLength.TabIndex = 2;
			// 
			// lblIdentification
			// 
			this.lblIdentification.AutoSize = true;
			this.lblIdentification.Location = new System.Drawing.Point(12, 236);
			this.lblIdentification.Name = "lblIdentification";
			this.lblIdentification.Size = new System.Drawing.Size(71, 12);
			this.lblIdentification.TabIndex = 1;
			this.lblIdentification.Text = "Identification";
			// 
			// txtIdentification
			// 
			this.txtIdentification.Location = new System.Drawing.Point(93, 233);
			this.txtIdentification.Name = "txtIdentification";
			this.txtIdentification.Size = new System.Drawing.Size(100, 19);
			this.txtIdentification.TabIndex = 2;
			// 
			// cmbTosPrecedence
			// 
			this.cmbTosPrecedence.DisplayMember = "Value";
			this.cmbTosPrecedence.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cmbTosPrecedence.FormattingEnabled = true;
			this.cmbTosPrecedence.Location = new System.Drawing.Point(141, 82);
			this.cmbTosPrecedence.Name = "cmbTosPrecedence";
			this.cmbTosPrecedence.Size = new System.Drawing.Size(144, 20);
			this.cmbTosPrecedence.TabIndex = 3;
			this.cmbTosPrecedence.ValueMember = "Key";
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(955, 592);
			this.Controls.Add(this.cmbTosPrecedence);
			this.Controls.Add(this.txtTosReserved);
			this.Controls.Add(this.txtTosReliability);
			this.Controls.Add(this.txtTosThroughput);
			this.Controls.Add(this.lblReserved);
			this.Controls.Add(this.txtTosDelay);
			this.Controls.Add(this.lblReliability);
			this.Controls.Add(this.lblThroughput);
			this.Controls.Add(this.lblTos);
			this.Controls.Add(this.lblDelay);
			this.Controls.Add(this.txtIdentification);
			this.Controls.Add(this.txtTotalLength);
			this.Controls.Add(this.lblIdentification);
			this.Controls.Add(this.txtHeaderLength);
			this.Controls.Add(this.lblTotalLength);
			this.Controls.Add(this.lblPrecedence);
			this.Controls.Add(this.lblHeaderLength);
			this.Controls.Add(this.txtVersion);
			this.Controls.Add(this.lblVersion);
			this.Controls.Add(this.button1);
			this.Name = "Form1";
			this.Text = "Form1";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.Label lblVersion;
		private System.Windows.Forms.TextBox txtVersion;
		private System.Windows.Forms.Label lblHeaderLength;
		private System.Windows.Forms.TextBox txtHeaderLength;
		private System.Windows.Forms.Label lblTos;
		private System.Windows.Forms.Label lblPrecedence;
		private System.Windows.Forms.Label lblDelay;
		private System.Windows.Forms.TextBox txtTosDelay;
		private System.Windows.Forms.Label lblThroughput;
		private System.Windows.Forms.TextBox txtTosThroughput;
		private System.Windows.Forms.Label lblReliability;
		private System.Windows.Forms.TextBox txtTosReliability;
		private System.Windows.Forms.Label lblReserved;
		private System.Windows.Forms.TextBox txtTosReserved;
		private System.Windows.Forms.Label lblTotalLength;
		private System.Windows.Forms.TextBox txtTotalLength;
		private System.Windows.Forms.Label lblIdentification;
		private System.Windows.Forms.TextBox txtIdentification;
		private System.Windows.Forms.ComboBox cmbTosPrecedence;
	}
}

