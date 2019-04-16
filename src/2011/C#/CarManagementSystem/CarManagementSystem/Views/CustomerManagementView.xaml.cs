using System.Windows;
using CarManagementSystem.ViewModels;

namespace CarManagementSystem.Views
{
	public partial class CustomerManagementView : Window, IView
	{
		public CustomerManagementView()
		{
			InitializeComponent();
			this.DataContext = new CustomerManagementViewModel() { View = this };
		}
	}
}
