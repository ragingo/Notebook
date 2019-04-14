using System;
using System.Collections.Generic;
using Explorer.Model.Common;

namespace Explorer.Model
{
	abstract class NodeBase<TNode, TContent> : ModelBase
		where TNode : NodeBase<TNode, TContent>
	{
		protected const int RootNodeLevel = 0;
		protected static readonly Dictionary<string, TNode> CacheNodes = new Dictionary<string, TNode>();

		private TContent _content;
		public event EventHandler ContentChanged;

		public string Name
		{
			get;
			set;
		}
		public TContent Content
		{
			get { return _content; }
			set
			{
				_content = value;
				if (ContentChanged != null)
				{
					ContentChanged(this, EventArgs.Empty);
				}
			}
		}
		public TNode Parent
		{
			get;
			set;
		}
		public ICollection<TNode> Children
		{
			get;
			private set;
		}
		public int Level
		{
			get;
			set;
		}
		public bool IsRoot
		{
			get
			{
				return Level == RootNodeLevel;
			}
		}
		public virtual string Identity
		{
			get
			{
				return Guid.NewGuid().ToString();
			}
		}
		public NodeBase()
		{
			if (!_initializing)
			{
				_initializing = true;
				InitializeChildren();
				_initializing = false;
			}
		}

		private static bool _initializing = false;

		protected virtual void InitializeChildren()
		{
			if (Children == null)
			{
				Children = CreateChildren();
			}
			else
			{
				Children.Clear();
			}
		}
		protected virtual ICollection<TNode> CreateChildren()
		{
			return new List<TNode>();
		}
		public virtual void AddChild(TNode child)
		{
			Children.Add(child);
			CacheNodes[child.Identity] = child;
		}
		public virtual void RemoveChild(TNode child)
		{
			Children.Remove(child);
			CacheNodes.Remove(child.Identity);
		}
		protected virtual TNode CreateNode(object obj)
		{
			return default(TNode);
		}
	}
}
