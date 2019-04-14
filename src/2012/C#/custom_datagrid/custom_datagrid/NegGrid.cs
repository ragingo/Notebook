using System.Collections.ObjectModel;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Collections;

namespace WpfApplication1
{
	class NegDataGridColumnHeaderColumn : FrameworkElement
	{
		public object Header { get; set; }
		public int ColumnSpan { get; set; }
		public int RowSpan { get; set; }
		public NegDataGridColumnHeaderColumn()
		{
			RowSpan = 1;
			ColumnSpan = 1;
		}
	}

	class NegDataGridColumnHeaderColumnCollection : ObservableCollection<NegDataGridColumnHeaderColumn>
	{
	}

	class NegDataGridColumnHeaderRow
	{
		public GridLength Height { get; set; }

		public NegDataGridColumnHeaderColumnCollection Columns
		{
			get;
			set;
		}
		
		public NegDataGridColumnHeaderRow()
		{
			Columns = new NegDataGridColumnHeaderColumnCollection();
		}
	}

	class NegDataGridColumnHeaderRowCollection : ObservableCollection<NegDataGridColumnHeaderRow>
	{
	}

	class NegDataGridColumnHeaderCell : Border
	{
		private Label _label;

		public object Value
		{
			get { return _label.Content; }
			set { _label.Content = value; }
		}
		public NegDataGridColumnHeaderCell()
		{
			_label = new Label();
			_label.VerticalAlignment = VerticalAlignment.Stretch;
			_label.VerticalContentAlignment = VerticalAlignment.Center;
			_label.HorizontalAlignment = HorizontalAlignment.Center;
			Child = _label;
			Style = FindResource("DataGridColumnHeaderCellStyle") as Style;
		}
	}

	class NegDataGridColumn : FrameworkElement
	{
		public static readonly DependencyProperty BindingProperty = DependencyProperty.Register("Binding", typeof(BindingBase), typeof(NegDataGridColumn));
		public BindingBase Binding
		{
			get { return (BindingBase)GetValue(BindingProperty); }
			set { SetValue(BindingProperty, value); }
		}
		public NegDataGridColumn()
		{
		}
	}

	class NegDataGridColumnCollection : ObservableCollection<NegDataGridColumn>
	{
		public NegDataGridColumnCollection()
		{
		}
	}

	class NegDataGrid : Grid
	{
		public static readonly DependencyProperty ItemsSourceProperty = DependencyProperty.Register("ItemsSource", typeof(IEnumerable), typeof(NegDataGrid), new PropertyMetadata(ItemsSourceChanged));
		public IEnumerable ItemsSource
		{
			get { return (IEnumerable)GetValue(ItemsSourceProperty); }
			set { SetValue(ItemsSourceProperty, value); }
		}

		private DataGrid _dataGrid;

		public NegDataGridColumnCollection Columns { get; set; }
		public NegDataGridColumnHeaderRowCollection ColumnHeaderRows { get; set; }

		public NegDataGrid()
		{
			ColumnHeaderRows = new NegDataGridColumnHeaderRowCollection();
			Columns = new NegDataGridColumnCollection();
			_dataGrid = new DataGrid();
			_dataGrid.HeadersVisibility = DataGridHeadersVisibility.None;
			_dataGrid.AutoGenerateColumns = false;
			Children.Add(_dataGrid);
		}

		public override void EndInit()
		{
			InitializeNegGrid();
			base.EndInit();
		}

		private void InitializeNegGrid()
		{
			int rowIdx = 0;
			for (int colIdx = 0; colIdx < Columns.Count; colIdx++)
			{
				_dataGrid.Columns.Add(new DataGridTextColumn());
				ColumnDefinitions.Add(new ColumnDefinition());
			}
			foreach (var header in ColumnHeaderRows)
			{
				var row = new RowDefinition();
				row.Height = header.Height;
				RowDefinitions.Add(row);

				int colIdx = 0;
				foreach (var col in header.Columns)
				{
					if (col.Visibility == Visibility.Visible)
					{
						var cell = new NegDataGridColumnHeaderCell();
						Children.Add(cell);
						cell.MouseLeftButtonDown += border_MouseLeftButtonDown;
						cell.Value = col.Header;
						cell.SetValue(Grid.RowProperty, rowIdx);
						cell.SetValue(Grid.ColumnProperty, colIdx);
						cell.SetValue(Grid.ColumnSpanProperty, col.ColumnSpan);
						cell.SetValue(Grid.RowSpanProperty, col.RowSpan);

						var splitter = new GridSplitter();
						splitter.SetValue(Grid.RowProperty, rowIdx);
						splitter.SetValue(Grid.ColumnProperty, colIdx);
						splitter.SetValue(Grid.ColumnSpanProperty, col.ColumnSpan);
						splitter.SetValue(Grid.RowSpanProperty, col.RowSpan);
						splitter.Margin = new Thickness(0, 1, 0, 0);
						splitter.Width = 2;
						splitter.HorizontalAlignment = HorizontalAlignment.Left;
						Children.Add(splitter);

						if (Columns.Count >= colIdx + 1)
						{
							ColumnDefinitions[colIdx].SetBinding(ColumnDefinition.WidthProperty, new Binding("Width")
							{
								Converter = new DataGridLengthToGridLengthConverter(),
								Source = _dataGrid.Columns[colIdx],
								Mode = BindingMode.TwoWay,
								UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged,
								NotifyOnSourceUpdated = true,
								NotifyOnTargetUpdated = true,
							});
							ColumnDefinitions[colIdx].SetValue(ColumnDefinition.WidthProperty, new GridLength(Columns[colIdx].Width));
						}
					}
					colIdx += col.ColumnSpan;
				}
				rowIdx++;
			}
			RowDefinitions.Add(new RowDefinition() { Height = new GridLength(1.0, GridUnitType.Star) });

			_dataGrid.SetValue(Grid.RowProperty, ColumnHeaderRows.Count);
			_dataGrid.SetValue(Grid.ColumnProperty, 0);
			_dataGrid.SetValue(Grid.ColumnSpanProperty, ColumnHeaderRows.Last().Columns.Count);
			_dataGrid.HorizontalScrollBarVisibility = ScrollBarVisibility.Hidden;
		}

		private void border_MouseLeftButtonDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
		{
			int col = Grid.GetColumn(sender as UIElement); // 行ヘッダがあれば さらに -1 してやる
			if (_dataGrid.SelectionUnit != DataGridSelectionUnit.FullRow)
			{
				_dataGrid.SelectedCells.Clear();
				foreach (var row in _dataGrid.Items)
				{
					_dataGrid.SelectedCells.Add(new DataGridCellInfo(row, _dataGrid.Columns[col]));
				}
				e.Handled = true;
			}
		}

		private static void ItemsSourceChanged(DependencyObject obj, DependencyPropertyChangedEventArgs e)
		{
			var negDataGrid = obj as NegDataGrid;
			negDataGrid._dataGrid.ItemsSource = (IEnumerable)e.NewValue;
		}
	}
}
