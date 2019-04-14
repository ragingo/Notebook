using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Management;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;

namespace ClassSelector
{
	public partial class MainWindow : Window
	{
		private bool isCancel = false;
		private ICollection<string> classNames = new ObservableCollection<string>();
		private ICollection<string> instances = new ObservableCollection<string>();

		public MainWindow()
		{
			InitializeComponent();
			treeView.ItemsSource = new []{ new ManagementPath("root") };

			cmbClassName.ItemsSource = classNames;
			var viewSource = CollectionViewSource.GetDefaultView(cmbClassName.ItemsSource);
			var listView = (ListCollectionView)viewSource;
			listView.CustomSort = StringComparer.CurrentCulture;

			dataGrid.ItemsSource = instances;
		}

		private void GetWmiMetaClasses(ManagementScope scope, string condition)
		{
			scope.Options.Authentication = AuthenticationLevel.Default;
			scope.Options.Impersonation = ImpersonationLevel.Impersonate;
			var observer = new ManagementOperationObserver();
			observer.ObjectReady += MetaClassObserver_Ready;
			var searcher = new ManagementObjectSearcher(scope, new SelectQuery("meta_class", condition));
			searcher.Get(observer);
		}

		private void GetWmiInstances(ManagementScope scope, ManagementPath path, string condition)
		{
			scope.Options.Authentication = AuthenticationLevel.Default;
			scope.Options.Impersonation = ImpersonationLevel.Impersonate;
			var observer = new ManagementOperationObserver();
			observer.ObjectReady += InstanceObserver_Ready;
			var searcher = new ManagementObjectSearcher(scope, new SelectQuery(path.ClassName, condition));
			searcher.Get(observer);
		}

		private void MetaClassObserver_Ready(object sender, ObjectReadyEventArgs e)
		{
			var obj = e.NewObject as ManagementObject;
			this.Dispatcher.BeginInvoke((Action)(() =>
			{
				classNames.Add(obj.ClassPath.ClassName);
			}));
			Thread.Sleep(1);
		}

		private void InstanceObserver_Ready(object sender, ObjectReadyEventArgs e)
		{
			var obj = e.NewObject as ManagementObject;
			this.Dispatcher.BeginInvoke((Action)(() =>
			{
				instances.Add(
					string.Join(", ",
						obj.Properties
							.OfType<PropertyData>()
							.Select(x => x.Name + " : " + x.Value)
							.ToArray()
					)
				);
			}));
			Thread.Sleep(1);

			if (isCancel)
			{
				(sender as ManagementOperationObserver).Cancel();
				isCancel = false;
			}
		}

		private void treeView_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
		{
			classNames.Clear();
			instances.Clear();
			GetWmiMetaClasses(
				new ManagementScope(treeView.SelectedValue as ManagementPath), 
				null);
		}

		private void cmbClassName_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			if (cmbClassName.SelectedValue == null)
			{
				return;
			}
			instances.Clear();
			GetWmiInstances(
				new ManagementScope(treeView.SelectedValue as ManagementPath), 
				new ManagementPath((string)cmbClassName.SelectedValue),
				null);
		}

		private void dataGrid_MouseDoubleClick(object sender, MouseButtonEventArgs e)
		{
			if (dataGrid.SelectedValue == null)
			{
				return;
			}
			string value = dataGrid.SelectedValue.ToString();
			value = value.Replace(", ", Environment.NewLine).TrimStart('{').TrimEnd('}').Trim();
			MessageBox.Show(value, "Ctrl + C でコピー出来ます！");
		}

		private void btnCancel_Click(object sender, RoutedEventArgs e)
		{
			isCancel = true;
		}
	}

}
