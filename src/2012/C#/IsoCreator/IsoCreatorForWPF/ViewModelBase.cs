using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;

namespace IsoCreatorForWPF
{
	public abstract class ViewModelBase : GalaSoft.MvvmLight.ViewModelBase
	{
		new public void RaisePropertyChanged(string propertyName)
		{
			base.RaisePropertyChanged(propertyName);
		}
	}
}
