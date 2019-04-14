using System.Collections.ObjectModel;
using System.Collections.Generic;
using System.ComponentModel;

namespace IsoCreatorForWPF
{
	public class TreeViewViewModel : ViewModelBase
	{
		private ICollectionView _root;
		public ICollectionView Root
		{
			get { return _root; }
			set
			{
				Util.RaisePropertyChanged(this, nameof(Root), ref _root, ref value);
			}
		}
	}
}
