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
using DeadCode.WME.Core;
using System.IO;
using System.Diagnostics;

namespace DeadCode.WME.Global
{
	public partial class FileAssignForm : FormBase
	{
		//////////////////////////////////////////////////////////////////////////
		public FileAssignForm()
		{
			InitializeComponent();
			SetState();
		}

		private WGame _Game = null;
		//////////////////////////////////////////////////////////////////////////
		public WGame Game
		{
			get
			{
				return _Game;
			}
			set
			{
				_Game = value;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public string Filename
		{
			get
			{
				return TxtFilename.Text;
			}
			set
			{
				TxtFilename.Text = value;
			}
		}

		private string _Filter = "";
		//////////////////////////////////////////////////////////////////////////
		public string Filter
		{
			get
			{
				return _Filter;
			}
			set
			{
				_Filter = value;
			}
		}

		private Form _ParentForm;
		//////////////////////////////////////////////////////////////////////////
		public new Form ParentForm
		{
			get
			{
				return _ParentForm;
			}
			set
			{
				_ParentForm = value;
			}
		}


		//////////////////////////////////////////////////////////////////////////
		private void OnLoad(object sender, EventArgs e)
		{
			this.MinimumSize = this.Size;

			if(AppMgr == null)
			{
				FormBase MainForm = ParentForm as FormBase;
				if (MainForm != null) AppMgr = MainForm.AppMgr;
			}

			if (AppMgr != null) LoadLayout(AppMgr.Settings);
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnFormClosed(object sender, FormClosedEventArgs e)
		{
			if (AppMgr != null) SaveLayout(AppMgr.Settings);
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnTextChanged(object sender, EventArgs e)
		{
			SetState();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnBrowseFile(object sender, EventArgs e)
		{
			string OrigFile = TxtFilename.Text;
			if (OrigFile != "") OrigFile = Game.MakeAbsolutePath(OrigFile);

			OpenFileDialog dlg = new OpenFileDialog();
			dlg.Filter = Filter + (Filter == "" ? "" : "|") + "All files (*.*)|*.*";
			dlg.RestoreDirectory = true;
			dlg.CheckFileExists = true;

			if (File.Exists(OrigFile)) dlg.FileName = OrigFile;

			if (dlg.ShowDialog() == DialogResult.OK)
			{
				string RelFile = Game.MakeRelativePath(dlg.FileName);
				TxtFilename.Text = RelFile;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		protected virtual void SetState()
		{
			string Filename = TxtFilename.Text;
			if (Filename != "") Filename = Game.MakeAbsolutePath(Filename);
			
			if(File.Exists(Filename))
			{
				LabelFullPath.Text = "";
				BtnOK.Enabled = true;
				BtnEdit.Enabled = true;
			}
			else
			{
				LabelFullPath.Text = "Invalid file name";
				BtnOK.Enabled = false;
				BtnEdit.Enabled = false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		protected virtual void OnEdit(object sender, EventArgs e)
		{
			Process.Start(Game.MakeAbsolutePath(TxtFilename.Text));
		}

	}
}