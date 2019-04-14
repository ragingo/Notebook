using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Management;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows.Forms;

namespace WindowsInformation
{
	public partial class MainForm : Form
	{
		private const string windowTitle = "Windows Information ";
		public MainForm()
		{
			InitializeComponent();

			this.Shown += new EventHandler(MainForm_Shown);
			this.tvClassList.NodeMouseClick += new TreeNodeMouseClickEventHandler(tvClassList_NodeMouseClick);
		}

		void MainForm_Shown(object sender, EventArgs e)
		{
			this.Cursor = Cursors.WaitCursor;
			SetTreeNodes();
			this.Cursor = Cursors.Arrow;
		}

		void tvClassList_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
		{
			this.Cursor = Cursors.WaitCursor;
			tvClassList.Enabled = false;

			dgvDetail.DataSource = LoadInformation(e.Node.Text);
			TreeNode parent = e.Node.Parent;
			if(parent == null)
			{
				this.Text = string.Format("{0} - {1}", windowTitle, e.Node.Text);
			}
			else
			{
				this.Text = string.Format("{0} - {1} - {2}", windowTitle, parent.Text, e.Node.Text);
			}

			tvClassList.Enabled = true;
			this.Cursor = Cursors.Arrow;
		}

		private void SetTreeNodes()
		{
			List<string> classNames = new List<string>(10000);
			using(StreamReader reader = new StreamReader("WMIClasses.txt", Encoding.GetEncoding("Shift_JIS")))
			{
				while(reader.EndOfStream == false)
				{
					string className = reader.ReadLine();
					if(Regex.IsMatch(className, @"\[*\]"))
					{
						tvClassList.Nodes.Add(className);
					}
					else
					{
						tvClassList.Nodes[tvClassList.Nodes.Count - 1].Nodes.Add(className);
					}
				}
			}
		}

		private DataTable LoadInformation(string wmiClassName)
		{
			DataTable table = new DataTable();
			table.Columns.Add("Name", typeof(string));
			table.Columns.Add("Description", typeof(object));
			table.Columns.Add("Qualifiers", typeof(string));
			table.Columns.Add("Value", typeof(string));

			if(Regex.IsMatch(wmiClassName, @"\[*\]"))
			{
				return table.Copy();
			}

			using(ManagementClass management = new ManagementClass(wmiClassName))
			{
				management.Options.UseAmendedQualifiers = true;
				PropertyDataCollection propCollection;

				try
				{
					propCollection = management.Properties;
				}
				catch(ManagementException e)
				{
					MessageBox.Show(e.Message);
					management.Dispose();
					return table.Copy();
				}

				foreach(PropertyData prop in propCollection)
				{
					object descript = null;
					try
					{
						descript = prop.Qualifiers["Description"].Value;
					}
					catch
					{
					}

					string qualifierName = string.Empty;
					foreach(QualifierData data in prop.Qualifiers)
					{
						qualifierName += data.Name + ", \r\n";
					}

					string value = string.Empty;
					foreach(ManagementObject manageObj in management.GetInstances())
					{
						object obj = manageObj.Properties[prop.Name].Value;

						if(obj != null)
						{
							if(obj.GetType().IsArray)
							{
								string[] arrayData = (string[])manageObj.Properties[prop.Name].Value;
								foreach(string s in arrayData)
								{
									value += s + ", \r\n";
								}
							}
							else
							{
								value = Convert.ToString(manageObj.Properties[prop.Name].Value);
							}
						}
					}

					table.Rows.Add(prop.Name, descript, qualifierName, value);
				}
			}

			return table.Copy();
		}
	}
}