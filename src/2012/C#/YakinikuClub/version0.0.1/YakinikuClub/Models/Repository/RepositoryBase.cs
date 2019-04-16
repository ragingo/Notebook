using System;
using System.Data.Entity;
using System.Linq;

namespace YakinikuClub.Models.Repository
{
	public abstract class RepositoryBase<TContext, TEntity>
		where TContext : DbContext
		where TEntity : class, new()
	{
		protected TContext _ctx;

		public RepositoryBase(TContext ctx)
		{
			_ctx = ctx;
		}
		public virtual TEntity Create()
		{
			return (TEntity)_ctx.Set(typeof(TEntity)).Create();
		}
		public virtual TEntity Get(Func<TEntity, bool> filter)
		{
			return _ctx.Set(typeof(TEntity)).Cast<TEntity>().FirstOrDefault(filter);
		}
		public virtual DbSet<TEntity> GetAll()
		{
			return _ctx.Set(typeof(TEntity)).Cast<TEntity>();
		}
		protected virtual void Add(TEntity entity)
		{
			_ctx.Set(typeof(TEntity)).Add(entity);
		}
		protected virtual void Update(TEntity entity)
		{
			_ctx.Set(typeof(TEntity)).Attach(entity);
		}
		protected virtual void Remove(TEntity entity)
		{
			_ctx.Set(typeof(TEntity)).Remove(entity);
		}
	}
}