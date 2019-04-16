using System.Windows;
using CarManagementSystem.ViewModels;

namespace CarManagementSystem.Views
{
	public partial class EditCustomerView : Window, IView
	{
		public EditCustomerView()
		{
			InitializeComponent();
			this.DataContext = new EditCustomerViewModel { View = this };
		}
	}
}
