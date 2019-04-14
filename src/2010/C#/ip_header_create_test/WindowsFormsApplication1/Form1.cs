using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Linq.Expressions;

namespace WindowsFormsApplication1
{
	public partial class Form1 : Form
	{
		private IPHeader2 _ipHeader = new IPHeader2();

		sealed class BindInfo
		{
			public string SourcePropertyName
			{
				get;
				private set;
			}
			public Control TargetControl
			{
				get;
				private set;
			}
			public string TargetPropertyName
			{
				get;
				private set;
			}
			public BindInfo(string sourcePropertyName, Control targetControl, string targetPropertyName)
			{
				SourcePropertyName = sourcePropertyName;
				TargetControl = targetControl;
				TargetPropertyName = targetPropertyName;
			}
		}

		public Form1()
		{
			InitializeComponent();

			cmbTosPrecedence.DataSource = 
				Enum.GetValues(typeof(PacketPriority))
					.OfType<PacketPriority>()
					.ToKeyValuePairArray(x => x);

			Func<Expression<Func<object>>, string> f = (x) => GetPropName(x);

			var infos = new BindInfo[]{
				new BindInfo(f(() => _ipHeader.Version), txtVersion, f(() => txtVersion.Text)),
				new BindInfo(f(() => _ipHeader.HeaderLength), txtHeaderLength, f(() => txtHeaderLength.Text)),
				new BindInfo(f(() => _ipHeader.TosPrecedence), cmbTosPrecedence, f(() => cmbTosPrecedence.SelectedValue)),
				new BindInfo(f(() => _ipHeader.TosDelay), txtTosDelay, f(() => txtTosDelay.Text)),
			};

			// ip header だけでなく、他のヘッダでも通用する様、IPHeader, IPHeader2 の基底クラスを考える

			foreach (var info in infos)
			{
				info.TargetControl.DataBindings.Add(info.TargetPropertyName, _ipHeader, info.SourcePropertyName);
			}
		}

		private string GetPropName(Expression<Func<object>> expression)
		{
			var expr = expression as LambdaExpression;
			if (expr.Body is MemberExpression)
			{
				return (expr.Body as MemberExpression).Member.Name;
			}
			else if (expr.Body is UnaryExpression)
			{
				return ((expr.Body as UnaryExpression).Operand as MemberExpression).Member.Name;
			}
			return string.Empty;
		}
	}
}
