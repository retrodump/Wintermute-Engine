// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace DeadCode.WME.Global
{
	public class ApplicationMgr
	{
		public readonly string Name;

		//////////////////////////////////////////////////////////////////////////
		public ApplicationMgr(string Name)
		{
			this.Name = Name;
		}

		//////////////////////////////////////////////////////////////////////////
		private SettingsMgr _Settings;
		public SettingsMgr Settings
		{
			get
			{
				if(_Settings==null) _Settings = new SettingsMgr(this.Name);
				return _Settings;
			}
		}
	}
}
