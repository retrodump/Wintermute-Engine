// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;

namespace DeadCode.WME.Debugger
{
	public interface IDebugServer
	{
		bool QueryData();

		int GetPropInt(string PropName);
		double GetPropFloat(string PropName);
		bool GetPropBool(string PropName);
		string GetPropString(string PropName);
		
		bool SetProp(string PropName, int PropValue);
		bool SetProp(string PropName, double PropValue);
		bool SetProp(string PropName, string PropValue);
		bool SetProp(string PropName, bool PropValue);

		string ResolveFilename(string RelativeFilename);

		bool AddBreakpoint(string Filename, int Line);
		bool RemoveBreakpoint(string Filename, int Line);

		bool ContinueExecution();
	}
}
