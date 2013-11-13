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
using System.Diagnostics;
using System.Media;
using System.IO;

namespace DeadCode.WME.Global
{
	public partial class ScriptsForm : FormBase
	{
		//////////////////////////////////////////////////////////////////////////
		public ScriptsForm()
		{
			InitializeComponent();
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

		private WGame _Game;
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

		private List<string> _Scripts = new List<string>();
		//////////////////////////////////////////////////////////////////////////
		public List<string> Scripts
		{
			get
			{
				return _Scripts;
			}
			set
			{
				_Scripts = value;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnLoad(object sender, EventArgs e)
		{
			this.MinimumSize = this.Size;

			if (AppMgr == null)
			{
				FormBase MainForm = ParentForm as FormBase;
				if (MainForm != null) AppMgr = MainForm.AppMgr;
			}
			if (AppMgr != null) LoadLayout(AppMgr.Settings);

			ListScripts.Items.Clear();
			foreach(string Scr in Scripts)
			{
				ListScripts.Items.Add(Scr);
			}
			if (ListScripts.Items.Count > 0) ListScripts.SelectedIndex = 0;
			
			SetState();
		}

		//////////////////////////////////////////////////////////////////////////
		private void SetState()
		{
			BtnUp.Enabled = ListScripts.SelectedItem != null && ListScripts.SelectedIndex > 0;
			BtnDown.Enabled = ListScripts.SelectedItem != null && ListScripts.SelectedIndex < ListScripts.Items.Count - 1;

			BtnEdit.Enabled = ListScripts.SelectedItem != null;
			BtnDelete.Enabled = ListScripts.SelectedItem != null;
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnFormClosed(object sender, FormClosedEventArgs e)
		{
			if (AppMgr != null) SaveLayout(AppMgr.Settings);
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnOK(object sender, EventArgs e)
		{
			Scripts.Clear();
			foreach(object Obj in ListScripts.Items)
			{
				if (Obj is string) Scripts.Add(Obj as string);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnUp(object sender, EventArgs e)
		{
			int i = ListScripts.SelectedIndex;

			object Temp = ListScripts.Items[i];
			ListScripts.Items[i] = ListScripts.Items[i - 1];
			ListScripts.Items[i - 1] = Temp;

			ListScripts.SelectedIndex--;
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnDown(object sender, EventArgs e)
		{
			int i = ListScripts.SelectedIndex;

			object Temp = ListScripts.Items[i];
			ListScripts.Items[i] = ListScripts.Items[i + 1];
			ListScripts.Items[i + 1] = Temp;

			ListScripts.SelectedIndex++;
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnAttach(object sender, EventArgs e)
		{
			OpenFileDialog dlg = new OpenFileDialog();
			dlg.Filter = "Scripts (*.script)|*.script|All files (*.*)|*.*";
			dlg.RestoreDirectory = true;
			dlg.CheckFileExists = true;
			if (dlg.ShowDialog() == DialogResult.OK)
			{
				string Filename = Game.MakeRelativePath(dlg.FileName);
				foreach(string Scr in ListScripts.Items)
				{
					if(string.Compare(Scr, Filename, true) == 0)
					{
						ListScripts.SelectedItem = Scr;
						return;
					}
				}

				ListScripts.Items.Add(Filename);
				ListScripts.SelectedItem = Filename;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnEdit(object sender, EventArgs e)
		{
			EditSelected();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnDetach(object sender, EventArgs e)
		{
			int i = ListScripts.SelectedIndex;
			ListScripts.Items.RemoveAt(i);

			if (i < ListScripts.Items.Count) ListScripts.SelectedIndex = i;
			else if (ListScripts.Items.Count > 0)
				ListScripts.SelectedIndex = ListScripts.Items.Count - 1;
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnListDblClick(object sender, EventArgs e)
		{
			EditSelected();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnSelectionChanged(object sender, EventArgs e)
		{
			SetState();
		}

		//////////////////////////////////////////////////////////////////////////
		private void EditSelected()
		{
			string Filename = ListScripts.SelectedItem as string;
			if (Filename == null) return;

			Filename = Game.MakeAbsolutePath(Filename);

			if (File.Exists(Filename))
				Process.Start(Filename);
			else
				SystemSounds.Asterisk.Play();
		}

	}
}