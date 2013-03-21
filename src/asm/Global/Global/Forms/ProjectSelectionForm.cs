// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace DeadCode.WME.Global
{
	public partial class ProjectSelectionForm : FormBase
	{
		//////////////////////////////////////////////////////////////////////////
		public ProjectSelectionForm()
		{
			InitializeComponent();			
		}

		private List<string> _RecentFiles;
		//////////////////////////////////////////////////////////////////////////
		public List<string> RecentFiles
		{
			get
			{
				return _RecentFiles;
			}
			set
			{
				_RecentFiles = value;
				ListRecent.Items.Clear();
				if(_RecentFiles != null)
				{
					foreach(string Filename in _RecentFiles)
					{
						ListRecent.Items.Add(Filename);
					}
					if (ListRecent.Items.Count > 0) ListRecent.SelectedIndex = 0;
				}
			}
		}

		private string _FileName;
		//////////////////////////////////////////////////////////////////////////
		public string FileName
		{
			get
			{
				return _FileName;
			}
			set
			{
				_FileName = value;
				TxtProject.Text = _FileName;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnLoad(object sender, EventArgs e)
		{
			this.MinimumSize = this.Size;
			if (AppMgr != null) LoadLayout(AppMgr.Settings);


			SetState();
			if (TxtProject.Enabled) TxtProject.Focus();
			else if (ListRecent.Enabled) ListRecent.Focus();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnFormClosed(object sender, FormClosedEventArgs e)
		{
			if (AppMgr != null) SaveLayout(AppMgr.Settings);
		}

		//////////////////////////////////////////////////////////////////////////
		private void SetState()
		{
			if (ListRecent.Items.Count == 0)
			{
				RadRecent.Enabled = false;
				RadOther.Checked = true;
			}

			if(RadRecent.Checked)
			{
				ListRecent.Enabled = true;
				TxtProject.Enabled = false;
				BtnBrowseProject.Enabled = false;

				BtnOk.Enabled = ListRecent.SelectedItem != null && File.Exists(ListRecent.SelectedItem.ToString());
			}
			else
			{
				ListRecent.Enabled = false;
				TxtProject.Enabled = true;
				BtnBrowseProject.Enabled = true;

				BtnOk.Enabled = File.Exists(TxtProject.Text);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnRadRecent(object sender, EventArgs e)
		{
			SetState();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnRadOther(object sender, EventArgs e)
		{
			SetState();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnOtherChanged(object sender, EventArgs e)
		{
			SetState();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnRecentChanged(object sender, EventArgs e)
		{
			SetState();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnBrowseProject(object sender, EventArgs e)
		{
			OpenFileDialog dlg = new OpenFileDialog();
			dlg.Filter = "WME projects (*.wpr)|*.wpr|All files (*.*)|*.*";
			dlg.RestoreDirectory = true;
			dlg.CheckFileExists = true;

			if (dlg.ShowDialog() == DialogResult.OK)
			{
				TxtProject.Text = dlg.FileName;
			}

		}

		//////////////////////////////////////////////////////////////////////////
		private void OnOK(object sender, EventArgs e)
		{
			if (RadRecent.Checked)
			{
				_FileName = ListRecent.SelectedItem.ToString().Trim();
			}
			else
			{
				_FileName = TxtProject.Text.Trim();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnRecentDblClick(object sender, EventArgs e)
		{
			if (BtnOk.Enabled) BtnOk.PerformClick();
		}
	}
}