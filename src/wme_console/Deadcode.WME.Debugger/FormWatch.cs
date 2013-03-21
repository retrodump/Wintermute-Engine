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

namespace DeadCode.WME.Debugger
{
	public partial class FormWatch : Form
	{
		public FormWatch()
		{
			InitializeComponent();
			SetState();
		}

		private void OnTextChanged(object sender, EventArgs e)
		{
			SetState();
		}

		private void SetState()
		{
			BtnOK.Enabled = TxtName.Text != string.Empty;
		}

		private void OnOK(object sender, EventArgs e)
		{
			this.DialogResult = DialogResult.OK;
			this.Close();
		}
	}
}