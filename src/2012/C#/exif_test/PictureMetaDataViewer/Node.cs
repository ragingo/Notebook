using System;
using System.Collections.Generic;

namespace PictureMetaDataViewer
{
	class Node
	{
		public bool IsRoot
		{
			get
			{
				return Parent == null;
			}
		}
		public bool IsLeaf
		{
			get
			{
				return Children.Count == 0;
			}
		}
		public string Text
		{
			get
			{
				return IsRoot ? FileName : Query;
			}
		}
		public string Path { get; set; }
		public string FileName { get { return System.IO.Path.GetFileName(Path); } }
		public Node Parent { get; set; }
		public string Query { get; set; }
		public string FullQuery
		{
			get
			{
				return IsRoot ? string.Empty : Parent.FullQuery + Query;
			}
		}
		public object Value { get; set; }
		public int Level
		{
			get
			{
				return IsRoot ? 1 : Parent.Level + 1;
			}
		}
		public Type Type { get; set; }
		public List<Node> Children { get; set; }
		public Node()
		{
			Children = new List<Node>();
		}
		public override string ToString()
		{
			return string.Format("{{ Query = {0}, Value = {1}, Children = {2} }}", FullQuery, Value, Children.Count);
		}
	}
}
