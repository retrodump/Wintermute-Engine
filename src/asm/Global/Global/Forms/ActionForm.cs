// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using DeadCode.WME.Global.Actions;
using System.Windows.Forms;

namespace DeadCode.WME.Global
{
	public class ActionForm : FormBase
	{
		//////////////////////////////////////////////////////////////////////////
		public ActionForm()
		{
			_ActContext = new ActionContext(this);
			_ActContext.AttachIdleEvent();
			FormClosed += new FormClosedEventHandler(OnFormClosed);
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnFormClosed(object sender, FormClosedEventArgs e)
		{
			ActContext.DetachIdleEvent();
		}

		private ActionContext _ActContext;
		//////////////////////////////////////////////////////////////////////////
		public ActionContext ActContext
		{
			get
			{
				return _ActContext;
			}
		}


	}	
}
