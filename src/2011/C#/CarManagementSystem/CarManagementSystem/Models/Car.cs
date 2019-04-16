using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace CarManagementSystem.Models
{
	[Serializable]
	public class Car
	{
		[Key, Column(Order = 0)]
		public int CustomerId
		{
			get;
			set;
		}
		[Key, Column(Order = 1)]
		public string Number
		{
			get;
			set;
		}
		[Key, Column(Order = 2)]
		public DateTime StoredDate
		{
			get;
			set;
		}
		public string Name
		{
			get;
			set;
		}
		public string Color
		{
			get;
			set;
		}
		public string Memo
		{
			get;
			set;
		}
		public virtual Customer Customer
		{
			get;
			set;
		}
		public virtual ICollection<WorkHistory> WorkHistories
		{
			get;
			set;
		}
	}
}
