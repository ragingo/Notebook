using System;
using System.ComponentModel.DataAnnotations;

namespace CarManagementSystem.Models
{
	[Serializable]
	public class WorkHistory
	{
		[Key, Column(Order = 0)]
		public int CarCustomerId
		{
			get;
			set;
		}
		[Key, Column(Order = 1)]
		public string CarNumber
		{
			get;
			set;
		}
		[Key, Column(Order = 2)]
		public DateTime CarStoredDate
		{
			get;
			set;
		}
		[Key, Column(Order = 3)]
		[DatabaseGenerated(DatabaseGeneratedOption.Identity)]
		public int Id
		{
			get;
			set;
		}
		public string Work
		{
			get;
			set;
		}
		public string Size
		{
			get;
			set;
		}
		public decimal Quantity
		{
			get;
			set;
		}
		public decimal UnitCost
		{
			get;
			set;
		}
		public int DistanceTraveled
		{
			get;
			set;
		}
		public virtual Car Car
		{
			get;
			set;
		}
	}
}
