using System;
using System.Collections.ObjectModel;
using System.Data.Entity;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Windows;
using System.Windows.Data;
using System.Windows.Input;
using CarManagementSystem.Common;
using CarManagementSystem.Models;
using CarManagementSystem.Views;
using Microsoft.Practices.Prism.Commands;

namespace CarManagementSystem.ViewModels
{
	public class CustomerManagementViewModel : ViewModelBase, IViewModel
	{
		public IView View
		{
			get;
			set;
		}

		private ObservableCollection<Customer> _customers = new ObservableCollection<Customer>();
		private ObservableCollection<Car> _cars = new ObservableCollection<Car>();
		private ObservableCollection<WorkHistory> _workHistories = new ObservableCollection<WorkHistory>();

		public ObservableCollection<Customer> Customers
		{
			get { return _customers; }
			set
			{
				_customers = value;
				RaisePropertyChanged(() => this.Customers);
			}
		}
		public ObservableCollection<Car> Cars
		{
			get { return _cars; }
			set
			{
				_cars = value;
				RaisePropertyChanged(() => this.Cars);
			}
		}
		public ObservableCollection<WorkHistory> WorkHistories
		{
			get { return _workHistories; }
			set
			{
				_workHistories = value;
				RaisePropertyChanged(() => this.WorkHistories);
			}
		}

		private Customer _selectedCustomer;
		public Customer SelectedCustomer
		{
			get { return _selectedCustomer; }
			set
			{
				_selectedCustomer = value;
				RaisePropertyChanged(() => this.SelectedCustomer);

				LoadCars();
				LoadWorkHistories();
			}
		}

		public ICommand AddCommand
		{
			get;
			private set;
		}

		public ICommand ModifyCommand
		{
			get;
			private set;
		}

		public ICommand DeleteCommand
		{
			get;
			private set;
		}

		public CustomerManagementViewModel()
		{
			LoadCustomers();

			AddCommand = new DelegateCommand(AddCustomer);
			ModifyCommand = new DelegateCommand<Customer>(ModifyCustomer);
			DeleteCommand = new DelegateCommand<Customer>(DeleteCustomer);
		}

		private void LoadCustomers()
		{
			using (var ctx = new CmsDbContext())
			{
				foreach (var c in ctx.Customers.AsNoTracking())
				{
					_customers.Add(c);
				}
			}
		}

		private void LoadCars()
		{
			_cars.Clear();
			using (var ctx = new CmsDbContext())
			{
				var customer =
					ctx.Customers
						.Where(c => c.Id == SelectedCustomer.Id)
						.Include(c => c.Cars)
						.AsNoTracking()
						.First();
				foreach (var c in customer.Cars)
				{
					_cars.Add(c);
				}
			}
		}

		private void LoadWorkHistories()
		{
			_workHistories.Clear();
			using (var ctx = new CmsDbContext())
			{
				var car =
					ctx.Cars
						.Where(c => c.CustomerId == SelectedCustomer.Id)
						.Include(c => c.WorkHistories)
						.AsNoTracking()
						.First();
				foreach (var wh in car.WorkHistories)
				{
					_workHistories.Add(wh);
				}
			}
		}

		private void AddCustomer()
		{
			var view = new EditCustomerView();
			var viewModel = (view.DataContext as EditCustomerViewModel);
			viewModel.Customer = new Customer();
			var result = view.ShowDialog();
			if (result.Value)
			{
				this.Customers.Add(viewModel.Customer);
			}
		}

		private void ModifyCustomer(Customer customer)
		{
			if (customer == null)
			{
				return;
			}
			var view = new EditCustomerView();
			var viewModel = (view.DataContext as EditCustomerViewModel);
			viewModel.Customer = ObjectUtility.DeepCopy(customer);
			var result = view.ShowDialog();
			if (result.Value)
			{
				this.Customers.Remove(customer);
				this.Customers.Add(viewModel.Customer);
			}
		}

		private void DeleteCustomer(Customer customer)
		{
			if (customer == null)
			{
				return;
			}
			var result = 
				MessageBox.Show("削除すると元に戻せなくなりますが、宜しいですか？", "警告", MessageBoxButton.YesNo, MessageBoxImage.Warning);
			if (result != MessageBoxResult.Yes)
			{
				return;
			}
			using (var ctx = new CmsDbContext())
			{
				var c = ctx.Customers.Find(customer.Id);
				ctx.Customers.Remove(c);
				ctx.SaveChanges();
			}
			this.Customers.Remove(customer);
		}
	}

	public class CustomerValueConverter : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			var customer = value as Customer;
			if (customer == null)
			{
				return null;
			}
			var asm = Assembly.GetExecutingAssembly();
			using(var reader = new StreamReader(asm.GetManifestResourceStream(Constants.Path.GoogleMapJs)))
			{
				return
					reader.ReadToEnd()
						.Replace("@address", customer.Address);
			}
		}

		public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			throw new NotImplementedException();
		}
	}

}
