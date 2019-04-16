using System.Collections.ObjectModel;
using System.Linq;
using System.Data.Entity;
using System.Windows.Input;
using CarManagementSystem.Models;
using CarManagementSystem.Views;
using Microsoft.Practices.Prism.Commands;

namespace CarManagementSystem.ViewModels
{
	public class CarManagementViewModel : ViewModelBase, IViewModel
	{
		public IView View
		{
			get;
			set;
		}

		private Customer _selectedCustomer;
		public Customer SelectedCustomer
		{
			get { return _selectedCustomer; }
			set
			{
				_selectedCustomer = value;
				RaisePropertyChanged(() => this.SelectedCustomer);
			}
		}

		private ObservableCollection<Car> _cars =
			new ObservableCollection<Car>();

		public ObservableCollection<Car> Cars
		{
			get { return _cars; }
			set
			{
				_cars = value;
				RaisePropertyChanged(() => this.Cars);
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

		public ICommand SelectCustomerCommand
		{
			get;
			private set;
		}

		public CarManagementViewModel()
		{
			AddCommand = new DelegateCommand(AddCar);
			ModifyCommand = new DelegateCommand<Car>(ModifyCar);
			DeleteCommand = new DelegateCommand<Car>(DeleteCar);
			SelectCustomerCommand = new DelegateCommand(SelectCustomer);
		}

		private void LoadCars()
		{
			_cars.Clear();
			using (var ctx = new CmsDbContext())
			{
				var cars =
					ctx.Customers
						.Where(x => x.Id == SelectedCustomer.Id)
						.Include(x => x.Cars)
						.Single().Cars;
				foreach (var car in cars)
				{
					_cars.Add(car);
				}
			}
		}

		private void AddCar()
		{
		}

		private void ModifyCar(Car car)
		{
		}

		private void DeleteCar(Car car)
		{
		}

		private void SelectCustomer()
		{
			SelectedCustomer = new Customer { Id = 1, Name = "田中太郎" };
			LoadCars();
		}
	}
}
