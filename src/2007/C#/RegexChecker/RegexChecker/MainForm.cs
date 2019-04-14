using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows.Forms;

namespace RegexChecker
{
	public partial class MainForm : Form
	{
		public MainForm()
		{
			InitializeComponent();

			txtInput.Enter += new EventHandler(txtInput_Enter);
			txtInput.Leave += new EventHandler(txtInput_Leave);
			txtInput.TextChanged += new EventHandler(txtInput_TextChanged);

			txtPattern.Enter += new EventHandler(txtPattern_Enter);
			txtPattern.Leave += new EventHandler(txtPattern_Leave);
			txtPattern.TextChanged += new EventHandler(txtPattern_TextChanged);

			txtInput.ScrollBars = ScrollBars.Both;
			txtPattern.ScrollBars = ScrollBars.Both;
			txtResult.ScrollBars = ScrollBars.Both;

			txtResult.ReadOnly = true;

			txtInput.Text = "対象文字列";
			txtInput.ForeColor = Color.Gray;
			txtInput.WordWrap = false;

			txtPattern.Text = "正規表現";
			txtPattern.ForeColor = Color.Gray;
			txtPattern.WordWrap = false;

			txtResult.WordWrap = false;
		}

		void txtInput_Enter(object sender, EventArgs e)
		{
			if(txtInput.Text.Equals("対象文字列"))
			{
				txtInput.Clear();
				txtInput.ForeColor = Color.Black;
			}
		}

		void txtInput_Leave(object sender, EventArgs e)
		{
			if(txtInput.Text.Length == 0)
			{
				txtInput.Text = "対象文字列";
				txtInput.ForeColor = Color.Gray;
			}
		}

		void txtInput_TextChanged(object sender, EventArgs e)
		{
			try
			{
				Match match = Regex.Match(@txtInput.Text, @txtPattern.Text);
				txtResult.Text = match.Value;
				if(match.Value.Equals(@txtInput.Text))
				{
					slblResult.Text = "結果：一致";
				}
				else
				{
					slblResult.Text = "結果：不一致";
				}
			}
			catch
			{
			}
		}

		void txtPattern_Enter(object sender, EventArgs e)
		{
			if(txtPattern.Text.Equals("正規表現"))
			{
				txtPattern.Clear();
				txtPattern.ForeColor = Color.Black;
			}
		}

		void txtPattern_Leave(object sender, EventArgs e)
		{
			if(txtPattern.Text.Length == 0)
			{
				txtPattern.Text = "正規表現";
				txtPattern.ForeColor = Color.Gray;
			}
		}

		void txtPattern_TextChanged(object sender, EventArgs e)
		{
			try
			{
				Match match = Regex.Match(@txtInput.Text, @txtPattern.Text);
				txtResult.Text = match.Value;
				if(match.Value.Equals(@txtInput.Text))
				{
					slblResult.Text = "結果：一致";
				}
				else
				{
					slblResult.Text = "結果：不一致";
				}
			}
			catch
			{
			}
		}

		private void tsmiFontSetting_Click(object sender, EventArgs e)
		{
			using(FontDialog dlg = new FontDialog())
			{
				dlg.Color = txtResult.ForeColor;
				dlg.Font = txtResult.Font;

				if(dlg.ShowDialog() == DialogResult.OK)
				{
					txtInput.ForeColor = txtPattern.ForeColor = txtResult.ForeColor = dlg.Color;
					txtInput.Font = txtPattern.Font = txtResult.Font = dlg.Font;
				}
			}
		}

		private void tsmiAllSelect_Click(object sender, EventArgs e)
		{
			if(txtInput.Focused)
			{
				txtInput.SelectAll();
			}
			else if(txtPattern.Focused)
			{
				txtPattern.SelectAll();
			}
		}

	}
}