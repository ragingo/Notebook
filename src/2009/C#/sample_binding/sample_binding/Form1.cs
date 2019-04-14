using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace sample_binding
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();

			int groupID = 0;
			comboBox1.Tag = ++groupID;
			comboBox2.Tag = ++groupID;
			comboBox3.Tag = ++groupID;
			comboBox4.Tag = ++groupID;
			comboBox1.SelectedValueChanged += new EventHandler(ComboBox_SelectedValueChanged);
			comboBox2.SelectedValueChanged += new EventHandler(ComboBox_SelectedValueChanged);
			comboBox3.SelectedValueChanged += new EventHandler(ComboBox_SelectedValueChanged);
			comboBox4.SelectedValueChanged += new EventHandler(ComboBox_SelectedValueChanged);
		}

		private void Form1_Load(object sender, EventArgs e)
		{
			// 品目グループマスタ
			DataTable dtGroups = new DataTable();
			dtGroups.Columns.Add("level", typeof(int));
			dtGroups.Columns.Add("code", typeof(int));
			dtGroups.Columns.Add("name", typeof(string));
			dtGroups.Rows.Add(1, 1, "階層１－Ａ");
			dtGroups.Rows.Add(1, 2, "階層１－Ｂ");
			dtGroups.Rows.Add(1, 3, "階層１－Ｃ");
			dtGroups.Rows.Add(1, 4, "階層１－Ｄ");
			dtGroups.Rows.Add(2, 5, "階層２－Ａ");
			dtGroups.Rows.Add(2, 6, "階層２－Ｂ");
			dtGroups.Rows.Add(2, 7, "階層２－Ｃ");
			dtGroups.Rows.Add(3, 8, "階層３－Ａ");
			dtGroups.Rows.Add(3, 9, "階層３－Ｂ");
			dtGroups.Rows.Add(3, 12, "階層３－Ｃ");
			dtGroups.Rows.Add(4, 10, "階層４－Ａ");
			dtGroups.Rows.Add(4, 11, "階層４－Ｂ");

			// グループの各階層データ抽出
			DataTable dtGrp1 = new DataView(dtGroups, "level = 1", "level", DataViewRowState.CurrentRows).ToTable();
			DataTable dtGrp2 = new DataView(dtGroups, "level = 2", "level", DataViewRowState.CurrentRows).ToTable();
			DataTable dtGrp3 = new DataView(dtGroups, "level = 3", "level", DataViewRowState.CurrentRows).ToTable();
			DataTable dtGrp4 = new DataView(dtGroups, "level = 4", "level", DataViewRowState.CurrentRows).ToTable();

			// 品目種別データ
			DataTable dtTypes = new DataTable("type_mst");
			dtTypes.Columns.Add("type_id", typeof(int));
			dtTypes.Columns.Add("grp1_code", typeof(int));
			dtTypes.Columns.Add("grp2_code", typeof(int));
			dtTypes.Columns.Add("grp3_code", typeof(int));
			dtTypes.Columns.Add("grp4_code", typeof(int));
			dtTypes.Rows.Add(1001, 1, 5, 8, 10);
			dtTypes.Rows.Add(1002, 2, 6, 9, 11);
			dtTypes.Rows.Add(1003, 1, 6, 9, 11);
			dtTypes.Rows.Add(1004, 2, 7, 9, 11);

			// 名前をつける
			dtGrp1.TableName = "group1";
			dtGrp2.TableName = "group2";
			dtGrp3.TableName = "group3";
			dtGrp4.TableName = "group4";

			// 関連付け
			DataSet ds = new DataSet();
			ds.Tables.Add(dtGrp1);
			ds.Tables.Add(dtGrp2);
			ds.Tables.Add(dtGrp3);
			ds.Tables.Add(dtGrp4);
			ds.Tables.Add(dtTypes);
			// これはサンプル
			//ds.Relations.Add("relation_grp1", dtGrp1.Columns["code"], dtTypes.Columns["grp1_code"]);
			//ds.Relations.Add("relation_grp2", dtGrp2.Columns["code"], dtTypes.Columns["grp2_code"]);
			//ds.Relations.Add("relation_grp3", dtGrp3.Columns["code"], dtTypes.Columns["grp3_code"]);
			//ds.Relations.Add("relation_grp4", dtGrp4.Columns["code"], dtTypes.Columns["grp4_code"]);
			//dtTypes.Columns.Add("grp1_name", typeof(string), "Parent(relation_grp1).name");
			//dtTypes.Columns.Add("grp2_name", typeof(string), "Parent(relation_grp2).name");
			//dtTypes.Columns.Add("grp3_name", typeof(string), "Parent(relation_grp3).name");
			//dtTypes.Columns.Add("grp4_name", typeof(string), "Parent(relation_grp4).name");

			// 先頭の空白行
			dtGrp1.Rows.InsertAt(dtGrp1.NewRow(), 0);
			dtGrp2.Rows.InsertAt(dtGrp2.NewRow(), 0);
			dtGrp3.Rows.InsertAt(dtGrp3.NewRow(), 0);
			dtGrp4.Rows.InsertAt(dtGrp4.NewRow(), 0);

			// コントロールとの連携
			dataGridView1.DataSource = dtTypes.DefaultView;

			comboBox1.DisplayMember = "name";
			comboBox1.ValueMember = "code";
			comboBox1.DataSource = dtGrp1.DefaultView;

			comboBox2.DisplayMember = "name";
			comboBox2.ValueMember = "code";
			comboBox2.DataSource = dtGrp2.DefaultView;

			comboBox3.DisplayMember = "name";
			comboBox3.ValueMember = "code";
			comboBox3.DataSource = dtGrp3.DefaultView;

			comboBox4.DisplayMember = "name";
			comboBox4.ValueMember = "code";
			comboBox4.DataSource = dtGrp4.DefaultView;

		}

		private void ComboBox_SelectedValueChanged(object sender, EventArgs e)
		{
			ComboBox cmb = sender as ComboBox;
			DataView dv = cmb.DataSource as DataView;
			DataView dv1, dv2, dv3, dv4;
			dv1 = comboBox1.DataSource as DataView;
			dv2 = comboBox2.DataSource as DataView;
			dv3 = comboBox3.DataSource as DataView;
			dv4 = comboBox4.DataSource as DataView;
			DataView source = dataGridView1.DataSource as DataView;

			if (cmb.SelectedValue == null || cmb.SelectedValue == DBNull.Value)
			{
				return;
			}

			switch ((int)cmb.Tag)
			{
			case 1:
				source.RowFilter = "grp1_code = " + cmb.SelectedValue;
				dv2.RowFilter = "code in (5,6)";
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			}
		}

	}
}
