using System;
using System.Web.Mvc;
using YakinikuClub.ViewModels;
using YakinikuClub.ViewModels.ClubActivity;

namespace YakinikuClub.Binders
{
	public class ManagementViewModelBinder : IModelBinder
	{
		public object BindModel(ControllerContext controllerContext, ModelBindingContext bindingContext)
		{
			var values = bindingContext.ValueProvider;

			if (bindingContext.ModelType == typeof(ClubActivityViewModel))
			{
				var vm = new ClubActivityViewModel();
				vm.Times = int.Parse(values.GetValue("ClubActivityRegister.Times").AttemptedValue);
				vm.StartDate = DateTime.Parse(values.GetValue("ClubActivityRegister.StartDate").AttemptedValue);
				vm.EndDate = DateTime.Parse(values.GetValue("ClubActivityRegister.EndDate").AttemptedValue);
				vm.Memo = values.GetValue("ClubActivityRegister.Memo").AttemptedValue;
				string key = values.GetValue("ClubActivityRegister.Shops").AttemptedValue;
				string[] arr = key.Split('-');
				vm.Shop = new ShopViewModel();
				vm.Shop.ShopCode = int.Parse(arr[0]);
				vm.Shop.LocationCode = int.Parse(arr[1]);
				return vm;
			}
			return null;
		}
	}
}