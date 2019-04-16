using System;
using System.ComponentModel.DataAnnotations;
using CarManagementSystem.Models;

namespace CarManagementSystem.ViewModels
{
	[Serializable]
	public class CarViewModel : ViewModelBase
	{
		private Car _car = new Car();

		[Required]
		public string Number
		{
			get
			{
				return _car.Number;
			}
			set
			{
				if (_car.Number != value)
				{
					ValidateProperty("Number", value);
					_car.Number = value;
					RaisePropertyChanged(() => _car.Number);
				}
			}
		}
		[Required]
		public DateTime StoredDate
		{
			get
			{
				return _car.StoredDate;
			}
			set
			{
				if (_car.StoredDate != value)
				{
					ValidateProperty("StoredDate", value);
					_car.StoredDate = value;
					RaisePropertyChanged(() => _car.StoredDate);
				}
			}
		}
		public string Name
		{
			get
			{
				return _car.Name;
			}
			set
			{
				if (_car.Name != value)
				{
					ValidateProperty("Name", value);
					_car.Name = value;
					RaisePropertyChanged(() => _car.Name);
				}
			}
		}
		public string Color
		{
			get
			{
				return _car.Color;
			}
			set
			{
				if (_car.Color != value)
				{
					ValidateProperty("Color", value);
					_car.Color = value;
					RaisePropertyChanged(() => _car.Color);
				}
			}
		}
		public string Memo
		{
			get
			{
				return _car.Memo;
			}
			set
			{
				if (_car.Memo != value)
				{
					ValidateProperty("Memo", value);
					_car.Memo = value;
					RaisePropertyChanged(() => _car.Memo);
				}
			}
		}

		public static CarViewModel ToViewModel(Car model)
		{
			var viewModel = new CarViewModel();
			return viewModel;
		}

		public static Car ToModel(CarViewModel viewModel)
		{
			var model = new Car();
			return model;
		}
	}
}
