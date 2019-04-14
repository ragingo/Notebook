using System;
using System.Collections;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Forms.DataVisualization.Charting;

namespace chart_test2
{
	public partial class ChartBox : UserControl
	{
		public static readonly DependencyProperty MinDateTimeProperty =
			DependencyProperty.Register(
				"MinDateTime", typeof(DateTime), typeof(ChartBox),
				new FrameworkPropertyMetadata(DateTime.MinValue, OnMinDateTimeChanged));

		public static readonly DependencyProperty MaxDateTimeProperty =
			DependencyProperty.Register(
				"MaxDateTime", typeof(DateTime), typeof(ChartBox),
				new FrameworkPropertyMetadata(DateTime.MaxValue, OnMaxDateTimeChanged));

		public static readonly DependencyProperty AxisIntervalProperty =
			DependencyProperty.Register(
				"AxisInterval", typeof(double), typeof(ChartBox),
				new FrameworkPropertyMetadata(1.0, OnAxisIntervalChanged));

		public DateTime MinDateTime
		{
			get { return (DateTime)GetValue(MinDateTimeProperty); }
			set { SetValue(MinDateTimeProperty, value); }
		}

		public DateTime MaxDateTime
		{
			get { return (DateTime)GetValue(MaxDateTimeProperty); }
			set { SetValue(MaxDateTimeProperty, value); }
		}

		public double AxisInterval
		{
			get { return (double)GetValue(AxisIntervalProperty); }
			set { SetValue(AxisIntervalProperty, value); }
		}

		public ChartBox()
		{
			InitializeComponent();
			InitializeChartBox();
		}

		private void InitializeChartBox()
		{
			chart.ChartAreas.Add(new ChartArea());
			chart.ChartAreas[0].AxisY.MajorGrid.Interval = 1.0;
			//chart.ChartAreas[0].AxisY.MajorGrid.IntervalOffset = 1.0;
			//chart.ChartAreas[0].AxisY.MajorGrid.IntervalOffsetType = DateTimeIntervalType.Days;
			//chart.ChartAreas[0].AxisY.MajorGrid.IntervalType = DateTimeIntervalType.Days;
			chart.ChartAreas[0].AxisY.MinorGrid.Interval = 1.0;
			//chart.ChartAreas[0].AxisY.MinorGrid.IntervalOffset = 1.0;
			//chart.ChartAreas[0].AxisY.MinorGrid.IntervalOffsetType = DateTimeIntervalType.Days;
			//chart.ChartAreas[0].AxisY.MinorGrid.IntervalType = DateTimeIntervalType.Days;
			chart.ChartAreas[0].AxisY.LabelStyle.Angle = -45;
			chart.ChartAreas[0].AxisY.Interval = 1.0;
			chart.Series.Add(new Series());
			chart.Series[0].ChartType = SeriesChartType.RangeBar;
			chart.Series[0].YValueType = ChartValueType.DateTime;
			chart.Series[0].Palette = ChartColorPalette.Pastel;
			chart.Series[0].BackGradientStyle = GradientStyle.HorizontalCenter;
			chart.Series.Add(new Series());
			chart.Series[1].ChartType = SeriesChartType.RangeBar;
			chart.Series[1].YValueType = ChartValueType.DateTime;
			chart.Series[1].Palette = ChartColorPalette.Pastel;
			chart.Series[1].MarkerSize = 20;
			chart.Series[1].MarkerStyle = MarkerStyle.Star5;
		}

		protected override void OnPropertyChanged(DependencyPropertyChangedEventArgs e)
		{
			base.OnPropertyChanged(e);
			if (e.Property.Name == "DataContext")
			{
				OnDataContextChanged();
			}
		}

		protected virtual void OnDataContextChanged()
		{
			chart.Series[0].Points.DataBind(base.DataContext as IEnumerable, "Title", "From,To", "Label=RangeLabel");
			chart.Series[1].Points.DataBind(base.DataContext as IEnumerable, "Title", "Star,Star", "ToolTip=StarLabel");
		}

		private static void OnMinDateTimeChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
		{
			var control = d as ChartBox;
			control.chart.ChartAreas[0].AxisY.Minimum = ((DateTime)e.NewValue).ToOADate();
		}

		private static void OnMaxDateTimeChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
		{
			var control = d as ChartBox;
			control.chart.ChartAreas[0].AxisY.Maximum = ((DateTime)e.NewValue).ToOADate();
		}

		private static void OnAxisIntervalChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
		{
			var control = d as ChartBox;
			control.chart.ChartAreas[0].AxisY.MajorGrid.Interval = (double)e.NewValue;
			control.chart.ChartAreas[0].AxisY.MinorGrid.Interval = (double)e.NewValue;
		}
	}
}
