// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;

namespace DeadCode.WME.Debugger
{
	public enum EWmeScriptState { Running, WaitingForObject, Sleeping, Finished, Persistent, Error, Paused, WaitingForScript, ThreadFinished };

	public interface IWmeScript
	{
		IntPtr NativeID { get; }
		int Line { get; }
		string Filename { get; }
		EWmeScriptState State { get; }
		int[] BreakpointLines { get; }
		bool TracingMode { get; set; }
	}
}
