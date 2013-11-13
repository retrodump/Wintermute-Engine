// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;

namespace DeadCode.WME.Debugger
{
	public interface IWmeObject
	{
		string NativeClass { get; }
		IWmeValue GetProperty(string Name);
	}
}
