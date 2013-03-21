// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace DeadCode.WME.Global
{
	public class TiledImgAssignForm : FileAssignForm
	{
		//////////////////////////////////////////////////////////////////////////
		protected override void SetState()
		{
			base.SetState();
			BtnEdit.Enabled = true;
		}

		//////////////////////////////////////////////////////////////////////////
		protected override void OnEdit(object sender, EventArgs e)
		{
			TiledImgEditorForm dlg = new TiledImgEditorForm();
			dlg.Game = this.Game;
			dlg.AppMgr = this.AppMgr;
			if (BtnOK.Enabled) dlg.OrigFilename = TxtFilename.Text;

			if(dlg.ShowDialog() == DialogResult.OK)
			{
				if (dlg.Filename != "" && dlg.Filename != null)
					TxtFilename.Text = dlg.Filename;
			}
		}
	}
}
