using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data.Entity;

namespace CarManagementSystem.Models
{
	//public class CustomerRepository : RepositoryBase<Customer>
	//{
	//    private DbSet<Customer> Customers
	//    {
	//        get;
	//        set;
	//    }
	//    public IEnumerable<Customer> GetCustomers(Func<Customer, bool> predicate)
	//    {
	//        var customers = this.Customers.Where(predicate);
	//        return customers;
	//    }
	//    public IEnumerable<Car> GetCars(Customer obj)
	//    {
	//        var cars =
	//            this.Customers
	//                .Include("Cars")
	//                .FirstOrDefault(x => x.Id == obj.Id)
	//                .Cars;
	//        return cars;
	//    }
	//}
}
