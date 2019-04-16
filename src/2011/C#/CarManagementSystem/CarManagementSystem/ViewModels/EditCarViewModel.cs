using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CarManagementSystem.Views;

namespace CarManagementSystem.ViewModels
{
	class EditCarViewModel : ViewModelBase, IViewModel
	{
		public IView View
		{
			get;
			set;
		}
	}
}
