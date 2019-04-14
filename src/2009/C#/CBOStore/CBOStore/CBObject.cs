using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Collections.ObjectModel;

namespace CBOStore
{
	class CBObject : INotifyPropertyChanged
	{
		private string _title = string.Empty;
		public string Title
		{
			get
			{
				return _title;
			}
			set
			{
				_title = value == null ? string.Empty : value;
				OnPropertyChanged("Title");
			}
		}

		private string _text = string.Empty;
		public string Text
		{
			get
			{
				return _text;
			}
			set
			{
				_text = value == null ? string.Empty : value;
				OnPropertyChanged("Text");
			}
		}

		//private object _object = null;
		//public object Object
		//{
		//    get
		//    {
		//        return _object;
		//    }
		//    set
		//    {
		//        _object = value;
		//        OnPropertyChanged("Object");
		//    }
		//}

		public event PropertyChangedEventHandler PropertyChanged;
		protected void OnPropertyChanged(string propertyName)
		{
			if (PropertyChanged != null)
			{
				PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
			}
		}
	}

	class CBObjectCollection : ObservableCollection<CBObject>
	{
	}
}
