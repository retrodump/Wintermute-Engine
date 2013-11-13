// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace DeadCode.WME.Global.Utils
{
	public class HourGlass : IDisposable
	{
		public HourGlass()
		{
			Enabled = true;
		}
		public void Dispose()
		{
			Enabled = false;
		}
		public static bool Enabled
		{
			get { return Application.UseWaitCursor; }
			set
			{
				if (value == Application.UseWaitCursor) return;
				Application.UseWaitCursor = value;
				Form f = Form.ActiveForm;
				if (f != null && f.Handle != null)   // Send WM_SETCURSOR
					NativeMethods.SendMessage(f.Handle, 0x20, (int)f.Handle, (IntPtr)1);
			}
		}
	}

}
