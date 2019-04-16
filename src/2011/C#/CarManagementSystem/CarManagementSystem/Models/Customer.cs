using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace CarManagementSystem.Models
{
	[Serializable]
	public class Customer
	{
		[Key]
		[DatabaseGenerated(DatabaseGeneratedOption.Identity)] 
		public int Id
		{
			get;
			set;
		}
		[Required]
		public string Name
		{
			get;
			set;
		}
		public string Furigana
		{
			get;
			set;
		}
		public string ZipCode
		{
			get;
			set;
		}
		public string Address
		{
			get;
			set;
		}
		public string Phone
		{
			get;
			set;
		}
		public string Memo
		{
			get;
			set;
		}
		public virtual ICollection<Car> Cars
		{
			get;
			set;
		}
	}
}
