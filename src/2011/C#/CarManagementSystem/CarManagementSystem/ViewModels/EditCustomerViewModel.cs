using System.ComponentModel.DataAnnotations;
using System.Windows;
using System.Windows.Input;
using CarManagementSystem.Common;
using CarManagementSystem.Models;
using CarManagementSystem.Views;
using Microsoft.Practices.Prism.Commands;

namespace CarManagementSystem.ViewModels
{
	public class EditCustomerViewModel : ViewModelBase, IViewModel
	{
		[Required(ErrorMessage = "必須項目です。")]
		[StringLength(50, ErrorMessage = "50文字以内で入力して下さい。")]
		public string Name
		{
			get { return _customer.Name; }
			set
			{
				if (_customer.Name != value)
				{
					ValidateProperty("CustomerName", value);
					_customer.Name = value;
					RaisePropertyChanged(() => _customer.Name);
				}
			}
		}

		[StringLength(50, ErrorMessage = "50文字以内で入力して下さい。")]
		public string Furigana
		{
			get { return _customer.Furigana; }
			set
			{
				if (_customer.Furigana != value)
				{
					ValidateProperty("Furigana", value);
					_customer.Furigana = value;
					RaisePropertyChanged(() => _customer.Furigana);
				}
			}
		}

		[RegularExpression(Constants.RegexPattern.ZipCode, ErrorMessage = "有効な形式ではありません。")]
		public string ZipCode
		{
			get { return _customer.ZipCode; }
			set
			{
				if (_customer.ZipCode != value)
				{
					ValidateProperty("ZipCode", value);
					_customer.ZipCode = value;
					RaisePropertyChanged(() => _customer.ZipCode);
				}
			}
		}

		[StringLength(50, ErrorMessage = "50文字以内で入力して下さい。")]
		public string Address
		{
			get { return _customer.Address; }
			set
			{
				if (_customer.Address != value)
				{
					ValidateProperty("Address", value);
					_customer.Address = value;
					RaisePropertyChanged(() => _customer.Address);
				}
			}
		}

		[DataType(DataType.PhoneNumber)]
		[RegularExpression(Constants.RegexPattern.PhoneNumber, ErrorMessage = "有効な形式ではありません。")]
		public string Phone
		{
			get { return _customer.Phone; }
			set
			{
				if (_customer.Phone != value)
				{
					ValidateProperty("Phone", value);
					_customer.Phone = value;
					RaisePropertyChanged(() => _customer.Phone);
				}
			}
		}

		[StringLength(100, ErrorMessage = "100文字以内で入力して下さい。")]
		public string Memo
		{
			get { return _customer.Memo; }
			set
			{
				if (_customer.Memo != value)
				{
					ValidateProperty("Memo", value);
					_customer.Memo = value;
					RaisePropertyChanged(() => _customer.Memo);
				}
			}
		}

		public IView View
		{
			get;
			set;
		}

		private Customer _customer;
		public Customer Customer
		{
			get { return _customer; }
			set
			{
				_customer = value;
				RaisePropertyChanged(() => this.Customer);
			}
		}

		public ICommand SaveCommand
		{
			get;
			private set;
		}

		public ICommand CancelCommand
		{
			get;
			private set;
		}

		public EditCustomerViewModel()
		{
			SaveCommand = new DelegateCommand(Save);
			CancelCommand = new DelegateCommand(Cancel);
		}

		private void Save()
		{
			using (var ctx = new CmsDbContext())
			{
				var customer = ctx.Customers.Find(_customer.Id);
				if (customer == null)
				{
					ctx.Customers.Add(_customer);
				}
				else
				{
					customer.Name = _customer.Name;
					customer.Furigana = _customer.Furigana;
					customer.ZipCode = _customer.ZipCode;
					customer.Address = _customer.Address;
					customer.Phone = _customer.Phone;
					customer.Memo = _customer.Memo;
				}
				ctx.SaveChanges();
			}
			SetDialogResult(true);
		}

		private void Cancel()
		{
			SetDialogResult(false);
		}

		private void SetDialogResult(bool? value)
		{
			var window = this.View as Window;
			if (window == null)
			{
				return;
			}
			window.DialogResult = value;
		}
	}
}
