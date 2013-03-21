using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using DeadCode.WME.Global;

namespace Integrator
{
	public partial class MainForm : FormBase
	{
		//////////////////////////////////////////////////////////////////////////
		public MainForm()
		{
			InitializeComponent();
			AppMgr = new ApplicationMgr("Integrator");
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnBrowseProject(object sender, EventArgs e)
		{
			OpenFileDialog dlg = new OpenFileDialog();
			if (File.Exists(TxtProjectFile.Text)) dlg.FileName = TxtProjectFile.Text;
			dlg.Filter = "WME projects (*.wpr)|*.wpr|All files (*.*)|*.*";
			dlg.RestoreDirectory = true;
			dlg.CheckFileExists = true;

			if (dlg.ShowDialog() == DialogResult.OK) TxtProjectFile.Text = dlg.FileName;
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnLoad(object sender, EventArgs e)
		{
			this.MinimumSize = this.Size;

			AddModule("SciTE", new ModSciTE());
			AddModule("UltraEdit", new ModUltraEdit());
			AddModule("UEStudio", new ModUeStudio());
			AddModule("PSPad", new ModPsPad());
			AddModule("GeSHi", new ModGeshi());

			AppMgr.Settings.LoadFromXmlFile();
			LoadLayout(AppMgr.Settings);
			LoadSettings(AppMgr.Settings);
			TxtProjectFile.Focus();

			OnTabSizeChanged(sender, new EventArgs());
		}

		private List<IntegratorModule> modules;
		//////////////////////////////////////////////////////////////////////////
		private void AddModule(string caption, IntegratorModule module)
		{
			if (modules == null) modules = new List<IntegratorModule>();

			modules.Add(module);
			module.ParentForm = this;

			TabPage Page = new TabPage(caption);
			Page.SizeChanged += new EventHandler(OnTabSizeChanged);
			Page.Controls.Add(module);

			TabModules.TabPages.Add(Page);
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnFormClosed(object sender, FormClosedEventArgs e)
		{
			SaveLayout(AppMgr.Settings);
			SaveSettings(AppMgr.Settings);
			AppMgr.Settings.SaveToXmlFile();
		}

		//////////////////////////////////////////////////////////////////////////
		private void SaveSettings(SettingsNode RootNode)
		{
			SettingsNode Section = RootNode.GetNode(this.Name, true, true);
			if (Section == null) return;

			Section.Clear();

			Section.SetValue("ProjectFile", TxtProjectFile.Text);
			Section.SetValue("ScriptExtensions", TxtExtensions.Text);

			foreach(IntegratorModule Mod in modules)
			{
				Mod.SaveSettings(Section);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void LoadSettings(SettingsNode RootNode)
		{
			SettingsNode Section = RootNode.GetNode(this.Name, true);
			if (Section != null)
			{
				TxtProjectFile.Text = Section.GetString("ProjectFile");
				TxtExtensions.Text = Section.GetString("ScriptExtensions");

				foreach (IntegratorModule Mod in modules)
				{
					Mod.LoadSettings(Section);
				}
			}
			if (TxtExtensions.Text == "") TxtExtensions.Text = "script inc";
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnTabSizeChanged(object sender, EventArgs e)
		{
			if(TabModules.SelectedTab!=null && TabModules.SelectedTab.Controls.Count > 0)
			{
				IntegratorModule Mod = TabModules.SelectedTab.Controls[0] as IntegratorModule;
				if (Mod != null)
				{
					Mod.Size = TabModules.SelectedTab.Size;
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnTabSelected(object sender, TabControlEventArgs e)
		{
			OnTabSizeChanged(sender, new EventArgs());
		}

		//////////////////////////////////////////////////////////////////////////
		public string[] GetExtensions()
		{
			return TxtExtensions.Text.Split(new char[] { ' ', ',', ';' }, StringSplitOptions.RemoveEmptyEntries);
		}
	}
}