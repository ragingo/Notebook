using System.Windows;
using CarManagementSystem.ViewModels;

namespace CarManagementSystem.Views
{
	public partial class CarManagementView : Window, IView
	{
		public CarManagementView()
		{
			InitializeComponent();
			this.DataContext = new CarManagementViewModel() { View = this };
		}
	}
}
