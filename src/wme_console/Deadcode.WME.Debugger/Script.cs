// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;

namespace DeadCode.WME.Debugger
{
	public class Script
	{
		public List<Script> Children = new List<Script>();
		private IWmeScript NativeScript = null;
		
		public Script Parent = null;
		public ScriptType Type = ScriptType.Normal;
		public string ThreadName = "";
		public ScriptScope GlobalScope = new ScriptScope();
		public Dictionary<int, ScriptScope> Scopes = new Dictionary<int, ScriptScope>();
		
		private int _CurrentScope = -1;
		[Browsable(false)]
		public int CurrentScope
		{
			get
			{
				return _CurrentScope;
			}
			set
			{
				if(!Scopes.ContainsKey(value))
				{
					Scopes.Add(value, new ScriptScope());
				}
				_CurrentScope = value;
			}
		}

		public enum ScriptType
		{
			Normal, EventThread, MethodThread
		};


		//////////////////////////////////////////////////////////////////////////
		public Script(IWmeScript NativeScript)
		{
			this.NativeScript = NativeScript;
			this.Type = ScriptType.Normal;
		}

		//////////////////////////////////////////////////////////////////////////
		public Script(IWmeScript NativeScript, ScriptType Type, Script Parent, string ThreadName)
		{
			this.NativeScript = NativeScript;
			this.Type = Type;
			this.Parent = Parent;
			this.ThreadName = ThreadName;
		}

		//////////////////////////////////////////////////////////////////////////
		public void OnChangeLine(int Line)
		{

		}
		
		//////////////////////////////////////////////////////////////////////////
		public void RemoveScope(int ScopeID)
		{
			if (Scopes.ContainsKey(ScopeID)) Scopes.Remove(ScopeID);
		}

		//////////////////////////////////////////////////////////////////////////
		public void AddGlobalVar(IWmeValue Var)
		{
			GlobalScope.AddVar(Var, this.Line);
		}

		//////////////////////////////////////////////////////////////////////////
		public void AddScopeVar(IWmeValue Var, int ScopeID)
		{
			if (!Scopes.ContainsKey(ScopeID)) Scopes.Add(ScopeID, new ScriptScope());
			Scopes[ScopeID].AddVar(Var, this.Line);
		}

		//////////////////////////////////////////////////////////////////////////
		public ScriptScope GetCurrentScope()
		{
			if (!Scopes.ContainsKey(CurrentScope)) return null;
			else return Scopes[CurrentScope];
		}

		//////////////////////////////////////////////////////////////////////////
		public string Filename
		{
			get
			{
				return NativeScript.Filename;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public int Line
		{
			get
			{
				return NativeScript.Line;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public EWmeScriptState State
		{
			get
			{
				return NativeScript.State;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		[Browsable(false)]
		public int[] BreakpointLines
		{
			get
			{
				return NativeScript.BreakpointLines;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public bool TracingMode
		{
			get
			{
				return NativeScript.TracingMode;
			}
			set
			{
				NativeScript.TracingMode = value;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public bool BreakpointAtLine(int Line)
		{
			int[] Lines = this.BreakpointLines;
			Array.Sort(Lines);
			return Array.BinarySearch(Lines, Line) >= 0;

		}

		//////////////////////////////////////////////////////////////////////////
		[Browsable(false)]
		public IntPtr NativeID
		{
			get
			{
				return NativeScript.NativeID;
			}
		}

		private ScriptVariableProvider _VarProvider = null;
		//////////////////////////////////////////////////////////////////////////
		[Browsable(false)]
		public ScriptVariableProvider VarProvider
		{
			get
			{
				if (_VarProvider == null) _VarProvider = new ScriptVariableProvider(this);
				return _VarProvider;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public ScriptScope GetVarScope(IWmeValue Variable)
		{
			if (GlobalScope.Vars.ContainsKey(Variable)) return GlobalScope;

			foreach(KeyValuePair<int, ScriptScope> kvp in Scopes)
			{
				if (kvp.Value.Vars.ContainsKey(Variable)) return kvp.Value;
			}
			return null;
		}

		public class ScriptFilenameComparer : IComparer<Script>
		{
			public int Compare(Script x, Script y)
			{
				return string.Compare(x.Filename, y.Filename);
			}
		};

		public class ScriptThreadNameComparer : IComparer<Script>
		{
			public int Compare(Script x, Script y)
			{
				return string.Compare(x.ThreadName, y.ThreadName);
			}
		};

	
	}
}
