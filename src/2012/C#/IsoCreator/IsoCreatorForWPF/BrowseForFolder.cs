using System;
using System.Runtime.InteropServices;

namespace IsoCreatorForWPF
{
	public sealed class BrowseForFolder
	{
		public long Hwnd { get; set; }
		public string Title { get; set; }
		public int Options { get; set; }
		public string RootFolder { get; set; }
		public string SelectedPath { get; set; }

		public BrowseForFolder()
		{
			SelectedPath = "";
		}

		public void ShowDialog()
		{
			dynamic shell = Activator.CreateInstance(Type.GetTypeFromProgID("Shell.Application"));
			dynamic folder = shell.BrowseForFolder(Hwnd, Title, Options, RootFolder);
			if (folder != null)
			{
				dynamic folderItem = folder.Self;
				SelectedPath = folderItem.Path;
				Marshal.ReleaseComObject(folderItem);
				Marshal.ReleaseComObject(folder);
			}
			Marshal.ReleaseComObject(shell);
		}
	}
}
