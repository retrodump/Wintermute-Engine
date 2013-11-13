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
using DeadCode.WME.Global;

namespace DeadCode.WME.WindowEdit
{
	public partial class DocumentTypeForm : FormBase
	{
		//////////////////////////////////////////////////////////////////////////
		public DocumentTypeForm()
		{
			InitializeComponent();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnLoad(object sender, EventArgs e)
		{
			this.MinimumSize = this.Size;
			if (AppMgr != null) LoadLayout(AppMgr.Settings);
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnFormClosed(object sender, FormClosedEventArgs e)
		{
			if (AppMgr != null) SaveLayout(AppMgr.Settings);
		}

		public string SelectedType = "";
		//////////////////////////////////////////////////////////////////////////
		private void OnOK(object sender, EventArgs e)
		{
			if (RadInvBox.Checked) SelectedType = "InvBox";
			else if (RadRespBox.Checked) SelectedType = "RespBox";
			else SelectedType = "Window";
		}
	}
}