using System;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Media.Imaging;
using System.Windows.Controls;

namespace PictureMetaDataViewer
{
	public partial class MainWindow : Window
	{
		private ObservableCollection<Node> _result = new ObservableCollection<Node>();
		public MainWindow()
		{
			InitializeComponent();
			this.DataContext = _result;

			textBox2.TextChanged += textBox2_TextChanged;
		}

		private void textBox2_TextChanged(object sender, TextChangedEventArgs e)
		{
		}

		private static void CreateTree(Node parent, BitmapMetadata metadata)
		{
			foreach (string query in metadata)
			{
				object value = null;
				try
				{
					value = metadata.GetQuery(query);
				}
				catch (ArgumentException)
				{
					continue;
				}
				var node = new Node();
				node.Parent = parent;
				node.Path = parent.Path;
				node.Query = query;
				parent.Children.Add(node);
				if (value is BitmapMetadata)
				{
					CreateTree(node, value as BitmapMetadata);
				}
				else
				{
					node.Value = ConvertValue(value);
				}
				node.Type = value.GetType();
			}
		}

		private static object ConvertValue(object value)
		{
			object v = null;
			if (value == null)
			{
				v = value = string.Empty;
			}
			else if (value is string || value is int || value is uint || value is long || value is ulong || value is ushort)
			{
				v = value;
			}
			else if (value is ushort[])
			{
				string str = string.Join(",", (value as ushort[]).Select(x => x.ToString()));
				v = str;
			}
			else if (value is ulong[])
			{
				string str = string.Join(",", (value as ulong[]).Select(x => x.ToString()));
				v = str;
			}
			else if (value is byte[])
			{
				string str = Encoding.Default.GetString(value as byte[]);
				v = str;
			}
			else if (value is BitmapMetadataBlob)
			{
				var blob = value as BitmapMetadataBlob;
				v = string.Join(" ", blob.GetBlobValue().Select(x => x.ToString("X2")));
			}
			else
			{
				v = value;
			}
			return v;
		}

		private void button1_Click(object sender, RoutedEventArgs e)
		{
			_result.Clear();

			var files = new DirectoryInfo(textBox1.Text).EnumerateFiles("*.jpg", SearchOption.AllDirectories);
			foreach (var file in files)
			{
				var frame = BitmapFrame.Create(file.OpenRead());
				var metadata = frame.Metadata as BitmapMetadata;
				var root = new Node { Path = file.FullName };
				_result.Add(root);
				CreateTree(root, metadata);
			}
		}
	}
}
