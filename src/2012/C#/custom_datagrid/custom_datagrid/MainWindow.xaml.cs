using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace WpfApplication1
{
	public partial class MainWindow : Window
	{
		public MainWindow()
		{
			InitializeComponent();

			this.DataContext = new[]{
				new object[]{
					new{}
				},
				new [] {
					new {Name="a1"},
					new {Name="a2"},
					new {Name="a3"},
					new {Name="a4"},
					new {Name="a5"},
					new {Name="a6"},
				}
			};
		}

		private void ColumnHeaderCell_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
		{
			int col = Grid.GetColumn(sender as UIElement) - 1;
			if (datagrid.SelectionUnit != DataGridSelectionUnit.FullRow)
			{
				datagrid.SelectedCells.Clear();
				foreach (var row in datagrid.Items)
				{
					datagrid.SelectedCells.Add(new DataGridCellInfo(row, datagrid.Columns[col]));
				}
				e.Handled = true;
			}
		}
	}
}
