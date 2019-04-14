using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace chart_test
{
	public partial class Form1 : Form
	{
		private Chart chart = new Chart();

		public Form1()
		{
			InitializeComponent();
			panel.Controls.Add(chart);
			chart.Dock = DockStyle.Fill;

			chart.ChartAreas.Add(new ChartArea("area1"));
			chart.ChartAreas[0].AxisY.LabelStyle.Angle = -45;
			chart.Series.Add(new Series("series1-1"));
			chart.Series[0].ChartType = SeriesChartType.RangeBar;
			chart.Series[0].YValueType = ChartValueType.DateTime;
			chart.Series[0].BackGradientStyle = GradientStyle.HorizontalCenter;
			chart.Series.Add(new Series("series1-2"));
			chart.Series[1].ChartType = SeriesChartType.RangeBar;
			chart.Series[1].YValueType = ChartValueType.DateTime;
			chart.Series[1].MarkerSize = 20;
			chart.Series[1].MarkerStyle = MarkerStyle.Star5;

			chart.Series[0].Points.DataBind(GetData(), "Title", "From,To", "Label=RangeLabel");
			chart.Series[1].Points.DataBind(GetData(), "Title", "Star,Star", "ToolTip=StarLabel");
		}

		private void dateTimePickerFrom_ValueChanged(object sender, EventArgs e)
		{
			if (dateTimePickerFrom.Value.ToOADate() > chart.ChartAreas[0].AxisY.Maximum)
			{
				chart.ChartAreas[0].AxisY.Minimum = GetData().Max(x => x.From).ToOADate();
			}
			else
			{
				chart.ChartAreas[0].AxisY.Minimum = dateTimePickerFrom.Value.ToOADate();
			}
		}

		private void dateTimePickerTo_ValueChanged(object sender, EventArgs e)
		{
			chart.ChartAreas[0].AxisY.Maximum = dateTimePickerTo.Value.ToOADate();
		}

		public IEnumerable<SampleData> GetData()
		{
			var data = new[] {
				new SampleData{ Title = "A", From = DateTime.Parse("2011/04/01"), To = DateTime.Parse("2011/04/05"), Star = DateTime.Parse("2011/03/31") },
				new SampleData{ Title = "B", From = DateTime.Parse("2011/04/10"), To = DateTime.Parse("2011/04/11"), Star = DateTime.Parse("2011/04/04") },
				new SampleData{ Title = "C", From = DateTime.Parse("2011/04/15"), To = DateTime.Parse("2011/04/16"), Star = DateTime.Parse("2011/04/13") },
			};
			return data;
		}
	}

	public class SampleData
	{
		public string Title { get; set; }
		public DateTime From { get; set; }
		public DateTime To { get; set; }
		public DateTime Star { get; set; }
		public string RangeLabel
		{
			get
			{
				return string.Format("{0:MM/dd}-{1:MM/dd}", From, To);
			}
		}
		public string StarLabel
		{
			get
			{
				return string.Format("{0:MM/dd}", Star);
			}
		}
	}
}
