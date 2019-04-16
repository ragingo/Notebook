using CarManagementSystem.Views;

namespace CarManagementSystem.ViewModels
{
	public interface IViewModel
	{
		IView View
		{
			get;
			set;
		}
	}
}
