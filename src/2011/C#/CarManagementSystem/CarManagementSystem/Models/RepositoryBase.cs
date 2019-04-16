using System.Data.Entity;

namespace CarManagementSystem.Models
{
	public abstract class RepositoryBase<T> : DbContext
		where T : class
	{
		public virtual void Add(T obj)
		{
			this.Set<T>().Add(obj);
		}
		public virtual void Remove(T obj)
		{
			this.Set<T>().Remove(obj);
		}
		public virtual void Attach(T obj)
		{
			this.Set<T>().Attach(obj);
		}
		public virtual void Commit()
		{
			this.SaveChanges();
		}
	}
}
