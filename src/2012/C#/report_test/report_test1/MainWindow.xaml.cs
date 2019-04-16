using System;
using System.Linq;
using System.Windows;
using Microsoft.Reporting.WinForms;
using System.IO;

namespace WpfApplication1
{
	public partial class MainWindow : Window
	{
		public MainWindow()
		{
			InitializeComponent();

			ReportDataSource source = new ReportDataSource();
			source.Name = "DataSet2";
			source.Value =
				Enumerable.Range(1, 31)
					.Select(x => new RosterReportInfo
					{
						Day = x.ToString(),
						DayOfWeek = new DateTime(2012,12,x).ToString("ddd"),
						StartWorkTime = "09:00",
						EndWorkTime = "20:00",
						OverTimeSpan = "18:00 ～ 20:00"
					});

			rptViewer.LocalReport.DataSources.Add(source);
			rptViewer.LocalReport.ReportPath = "./Report1.rdlc";
			rptViewer.RefreshReport();
		}

		private void button1_Click(object sender, RoutedEventArgs e)
		{
			Warning[] warnings;
			string[] streamids;
			string mimeType;
			string encoding;
			string filenameExtension;

			LocalReport localRpt = new LocalReport();
			localRpt.ReportPath = rptViewer.LocalReport.ReportPath;
			localRpt.DataSources.Add(rptViewer.LocalReport.DataSources["DataSet2"]);

			byte[] bytes = 
				localRpt.Render(
					"PDF", null, out mimeType, out encoding, out filenameExtension,
					out streamids, out warnings
				);

			using (var fs = new FileStream("f:\\output.pdf", FileMode.Create))
			{
				fs.Write(bytes, 0, bytes.Length);
			}
		}
	}
}
