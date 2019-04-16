using System;
using System.ComponentModel.DataAnnotations;
using System.Text;
using Microsoft.Practices.Prism.ViewModel;

namespace CarManagementSystem.ViewModels
{
	[Serializable]
	public abstract class ViewModelBase : NotificationObject
	{
		protected virtual void ValidateProperty(string propertyName, object value)
		{
			Validator.ValidateProperty(
				value,
				new ValidationContext(this, null, null) { MemberName = propertyName }
			);
		}
		public override string ToString()
		{
			int count = 0;
			var sb = new StringBuilder();
			sb.Append("{");
			foreach (var prop in this.GetType().GetProperties())
			{
				if (count++ > 0)
				{
					sb.Append(", ");
				}
				sb.AppendFormat("{0}={1}", prop.Name, prop.GetValue(this, null));
			}
			sb.Append("}");
			return sb.ToString();
		}
	}
}
