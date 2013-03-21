using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using DeadCode.WME.Global;
using System.IO;

namespace DeadCode.WME.StringTableMgr
{
	public partial class SettingsForm : FormBase
	{
		private StringTableMgr Mgr;

		//////////////////////////////////////////////////////////////////////////
		public SettingsForm()
		{
			InitializeComponent();
			AppMgr = new ApplicationMgr("StringTableMgr");
			Mgr = new StringTableMgr();
			Mgr.OnProgressChange += new ProgressChangeDelegate(OnProgressChange);
			Mgr.OnAddLog += new AddLogDelegate(OnAddLog);
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnLoad(object sender, EventArgs e)
		{
			this.MinimumSize = this.Size;
			
			AppMgr.Settings.LoadFromXmlFile();
			LoadLayout(AppMgr.Settings);
			Mgr.LoadSettings(AppMgr.Settings);
			DisplaySettings();

			SetState(AppState.Initial);
			TxtProjectFile.Focus();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnFormClosed(object sender, FormClosedEventArgs e)
		{			
			SaveLayout(AppMgr.Settings);
			AssignSettings();
			Mgr.SaveSettings(AppMgr.Settings);
			AppMgr.Settings.SaveToXmlFile();
		}

		//////////////////////////////////////////////////////////////////////////
		private void DisplaySettings()
		{
			TxtProjectFile.Text = Mgr.ProjectFile;
			TxtStringTable.Text = Mgr.StringTableFile;
			CheckBackup.Checked = Mgr.BackupOldFiles;
		}

		//////////////////////////////////////////////////////////////////////////
		private void AssignSettings()
		{
			Mgr.ProjectFile     = TxtProjectFile.Text;
			Mgr.StringTableFile = TxtStringTable.Text;
			Mgr.BackupOldFiles  = CheckBackup.Checked;
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnScanProject(object sender, EventArgs e)
		{
			if(!File.Exists(TxtProjectFile.Text))
			{
				MessageBox.Show("Please select an existing WME project file.", this.Text, MessageBoxButtons.OK, MessageBoxIcon.Warning);
				return;
			}

			if(TxtStringTable.Text.Trim()==string.Empty)
			{
				MessageBox.Show("Please select a string table file.", this.Text, MessageBoxButtons.OK, MessageBoxIcon.Warning);
				return;
			}

			if(CurrentState == AppState.Scanned)
			{
				if (MessageBox.Show("Scanning the project will discard any unsaved changes. Do you want to continue?", this.Text, MessageBoxButtons.YesNo, MessageBoxIcon.Question) != DialogResult.Yes) return;
			}

			TxtLog.Text = "";

			this.Enabled = false;
			AssignSettings();
			Mgr.ScanProject();
			this.Enabled = true;
			SetState(AppState.Scanned);
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnManageStrings(object sender, EventArgs e)
		{
			ClassifyForm Form = new ClassifyForm();
			Form.AppMgr = AppMgr;
			Form.Mgr = Mgr;

			Form.ShowDialog();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnSaveChanges(object sender, EventArgs e)
		{
			if (Mgr.ProjectStrings.Count == 0)
			{
				MessageBox.Show("There's nothing to save.", this.Text, MessageBoxButtons.OK, MessageBoxIcon.Information);
				return;
			}

			string Msg = "";
			foreach(StringItem StrItem in Mgr.ProjectStrings)
			{
				if(StrItem.ID == string.Empty)
				{
					Msg += "Warning, there are strings with no ID selected.\n\n";
					break;
				}
			}
			Msg += "Do you want to save changes to game files?\n(this action cannot be undone)";
			if (MessageBox.Show(Msg, this.Text, MessageBoxButtons.YesNo, MessageBoxIcon.Question) != DialogResult.Yes) return;

			TxtLog.Text = "";
			this.Enabled = false;
			AssignSettings();
			Mgr.SaveChanges();
			this.Enabled = true;
			SetState(AppState.Saved);
		}


		//////////////////////////////////////////////////////////////////////////
		private void OnProgressChange(object sender, ProgressChangeEventArgs e)
		{
			ProgScan.Maximum = e.TotalItems;
			ProgScan.Value = e.CurrentItem;
			LblProgress.Text = e.Description;

			Application.DoEvents();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnAddLog(object sender, AddLogEventArgs e)
		{
			TxtLog.SelectionLength = 0;
			TxtLog.AppendText(e.Message.Replace("\n", "\r\n"));

			TxtLog.SelectionStart = TxtLog.Text.Length;
			TxtLog.SelectionLength = 0;
			TxtLog.ScrollToCaret();
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
		private void OnBrowseTable(object sender, EventArgs e)
		{
			OpenFileDialog dlg = new OpenFileDialog();
			if (File.Exists(TxtStringTable.Text)) dlg.FileName = TxtStringTable.Text;
			dlg.Filter = "String tables (*.tab)|*.tab|All files (*.*)|*.*";
			dlg.RestoreDirectory = true;
			dlg.CheckFileExists = false;

			if (dlg.ShowDialog() == DialogResult.OK) TxtStringTable.Text = dlg.FileName;

		}

		//////////////////////////////////////////////////////////////////////////
		private void OnAbout(object sender, EventArgs e)
		{
			FormAbout Form = new FormAbout();
			Form.ShowDialog();
		}

		private enum AppState
		{
			Initial, Scanned, Saved
		}

		AppState CurrentState = AppState.Initial;
		//////////////////////////////////////////////////////////////////////////
		private void SetState(AppState State)
		{
			CurrentState = State;


			BtnScan.Enabled = true;
			BtnManage.Enabled = CurrentState == AppState.Scanned;
			BtnSave.Enabled = CurrentState == AppState.Scanned;

			TxtProjectFile.Enabled = CurrentState != AppState.Scanned;
			TxtStringTable.Enabled = CurrentState != AppState.Scanned;

			BtnBrowseProject.Enabled = TxtProjectFile.Enabled;
			BtnBrowseTable.Enabled = TxtStringTable.Enabled;
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnFormClosing(object sender, FormClosingEventArgs e)
		{
			if(CurrentState==AppState.Scanned)
			{
				if (MessageBox.Show("Closing the application will discard any unsaved changes. Do you want to continue?", this.Text, MessageBoxButtons.YesNo, MessageBoxIcon.Question) != DialogResult.Yes) e.Cancel = true;
			}
		}

	}
}