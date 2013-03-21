// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;

namespace DeadCode.WME.Global
{
	public interface ILayoutAwareControl
	{
		void SaveControlLayout(SettingsNode RootNode);
		void LoadControlLayout(SettingsNode RootNode);
	}
}
