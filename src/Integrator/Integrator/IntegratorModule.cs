using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using DeadCode.WME.Global;
using System.ComponentModel;
using System.IO;

namespace Integrator
{
	public class IntegratorModule : UserControl
	{
		//////////////////////////////////////////////////////////////////////////
		virtual public void SaveSettings(SettingsNode RootNode)
		{
		}

		//////////////////////////////////////////////////////////////////////////
		virtual public void LoadSettings(SettingsNode RootNode)
		{
		}

		private MainForm parentForm;
		//////////////////////////////////////////////////////////////////////////
		[Browsable(false)]
		public new MainForm ParentForm
		{
			get
			{
				return parentForm;
			}
			set
			{
				parentForm = value;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		protected void BackupFile(string Filename)
		{
			if (!File.Exists(Filename)) return;

			string BackupFilename;
			int Counter = 0;
			do
			{
				BackupFilename = Filename + ".bak" + (Counter == 0 ? "" : Counter.ToString());
				Counter++;
			}
			while (File.Exists(BackupFilename));

			File.Copy(Filename, BackupFilename);
		}

		//////////////////////////////////////////////////////////////////////////
		protected void MakeAssocitations(string ExePath)
		{
			string IconExe = Path.Combine(WmeUtils.ToolsPath, "ProjectMan.exe");
			int IconIndex = 7;

			string[] Extensions = ParentForm.GetExtensions();
			foreach (string Ext in Extensions)
			{
				WmeUtils.MakeFileAssociation(Ext, "Wintermute.Script", "WME Script File", ExePath, IconExe, IconIndex);
			}
		}

	}
}
