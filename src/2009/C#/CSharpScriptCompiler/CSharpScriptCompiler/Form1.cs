using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Microsoft.CSharp;
using System.CodeDom.Compiler;
using System.Diagnostics;
using System.IO;

namespace CSharpScriptCompiler
{
	public partial class MainForm : Form
	{
		private List<string> assemblies;

		public MainForm()
		{
			InitializeComponent();

			assemblies = new List<string>();
			assemblies.Add("Microsoft.Build.Engine.dll");
			assemblies.Add("Microsoft.Build.Framework.dll");
			assemblies.Add("Microsoft.Build.Tasks.dll");
			assemblies.Add("Microsoft.Build.Utilities.dll");
			assemblies.Add("Microsoft.JScript.dll");
			assemblies.Add("Microsoft.VisualBasic.Compatibility.Data.dll");
			assemblies.Add("Microsoft.VisualBasic.Compatibility.dll");
			assemblies.Add("Microsoft.VisualBasic.dll");
			assemblies.Add("Microsoft.VisualBasic.Vsa.dll");
			assemblies.Add("Microsoft.VisualC.Dll");
			assemblies.Add("Microsoft.Vsa.dll");
			assemblies.Add("Microsoft.Vsa.Vb.CodeDOMProcessor.dll");
			assemblies.Add("Microsoft_VsaVb.dll");
			assemblies.Add("System.configuration.dll");
			assemblies.Add("System.Configuration.Install.dll");
			assemblies.Add("System.Data.dll");
			assemblies.Add("System.Data.SqlXml.dll");
			assemblies.Add("System.Deployment.dll");
			assemblies.Add("System.Design.dll");
			assemblies.Add("System.DirectoryServices.dll");
			assemblies.Add("System.DirectoryServices.Protocols.dll");
			assemblies.Add("System.dll");
			assemblies.Add("System.Drawing.Design.dll");
			assemblies.Add("System.Drawing.dll");
			assemblies.Add("System.EnterpriseServices.dll");
			assemblies.Add("System.EnterpriseServices.Wrapper.dll");
			assemblies.Add("System.Management.dll");
			assemblies.Add("System.Messaging.dll");
			assemblies.Add("System.Runtime.Remoting.dll");
			assemblies.Add("System.Runtime.Serialization.Formatters.Soap.dll");
			assemblies.Add("System.Security.dll");
			assemblies.Add("System.ServiceProcess.dll");
			assemblies.Add("System.Transactions.dll");
			assemblies.Add("System.Web.dll");
			assemblies.Add("System.Web.Mobile.dll");
			assemblies.Add("System.Web.RegularExpressions.dll");
			assemblies.Add("System.Web.Services.dll");
			assemblies.Add("System.Windows.Forms.dll");
			assemblies.Add("System.XML.dll");
			assemblies.Add("VsaVb7rt.dll");

			lstRefAsms.Items.AddRange(assemblies.ToArray());
		}

		private void MainForm_Load(object sender, EventArgs e)
		{

		}

		private void btnCompile_Click(object sender, EventArgs e)
		{
			CSharpCodeProvider provider = new CSharpCodeProvider();
			CompilerParameters parameters = new CompilerParameters();
			parameters.ReferencedAssemblies.AddRange(lstRefAsms.CheckedItems.OfType<string>().ToArray());
			parameters.GenerateExecutable = true;
			CompilerResults results = provider.CompileAssemblyFromSource(parameters, txtSource.Text);
			if (results.Errors.Count == 0)
			{
				string path = results.TempFiles.BasePath + ".exe";
				if (File.Exists(path))
				{
					Process.Start(new ProcessStartInfo(path));
				}
			}
			else
			{
				MessageBox.Show("compile error");
			}
		}
	}
}
