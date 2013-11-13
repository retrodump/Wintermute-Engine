// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using DeadCode.WME.DocMaker;
using Microsoft.Win32;
using System.IO;

namespace DeadCode.WME.Debugger
{
	//////////////////////////////////////////////////////////////////////////
	public class DebugClient : IDisposable
	{
		private Dictionary<IntPtr, Script> Scripts = new Dictionary<IntPtr, Script>();
		public readonly IDebugServer Server;
		private ScriptScope GlobalScope = new ScriptScope();

		DebugWindow DebugWindow;
		//////////////////////////////////////////////////////////////////////////
		public DebugClient(IDebugServer Server)
		{
			this.Server = Server;

			DebugWindow = new DebugWindow(this);
			DebugWindow.Show();
		}

		//////////////////////////////////////////////////////////////////////////
		public bool GameInit()
		{
			//DebugWindow.AddLine("Init");
			Server.QueryData();
			DebugWindow.DisplayVars(GlobalScope);

			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		public bool GameShutdown()
		{
			DebugWindow.ClearLog();
			//DebugWindow.AddLine("Shutdown");
			DebugWindow.Shutdown();
			Scripts.Clear();
			GlobalScope = new ScriptScope();
			return true;
		}

		private bool RefreshVars = false;
		//////////////////////////////////////////////////////////////////////////
		public bool GameTick()
		{
			if(RefreshVars)
			{
				DebugWindow.RefreshVars();
				RefreshVars = false;
			}
			DebugWindow.Tick();

			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		public bool Log(int ErrorCode, string Text)
		{
			DebugWindow.AddLogLine(DateTime.Now.ToString("T") + ":  " + Text);
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		private void RefreshScripts()
		{
			List<Script> ScrList = new List<Script>();
			
			foreach(Script Scr in Scripts.Values)
			{
				ScrList.Add(Scr);
			}
			DebugWindow.RefreshScripts(ScrList.ToArray());
		}

		//////////////////////////////////////////////////////////////////////////
		public bool ScriptInit(IWmeScript NativeScript)
		{
			if (Scripts.ContainsKey(NativeScript.NativeID)) return true;

			Scripts.Add(NativeScript.NativeID, new Script(NativeScript));
			RefreshScripts();

			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		public bool ScriptEventThreadInit(IWmeScript NativeScript, IntPtr ParentID, string EventName)
		{
			if(Scripts.ContainsKey(NativeScript.NativeID)) return true;

			if(Scripts.ContainsKey(ParentID))
			{
				Script Parent = Scripts[ParentID];
				Script Thread = new Script(NativeScript, Script.ScriptType.EventThread, Parent, EventName);
				Scripts.Add(NativeScript.NativeID, Thread);
				Parent.Children.Add(Thread);
				RefreshScripts();
				return true;
			}
			else return false;
		}

		//////////////////////////////////////////////////////////////////////////
		public bool ScriptMethodThreadInit(IWmeScript NativeScript, IntPtr ParentID, string MethodName)
		{
			if (Scripts.ContainsKey(NativeScript.NativeID)) return true;

			if (Scripts.ContainsKey(ParentID))
			{
				Script Parent = Scripts[ParentID];
				Script Thread = new Script(NativeScript, Script.ScriptType.MethodThread, Parent, MethodName);
				Scripts.Add(NativeScript.NativeID, Thread);
				Parent.Children.Add(Thread);
				RefreshScripts();
				return true;
			}
			else return false;
		}

		//////////////////////////////////////////////////////////////////////////
		public bool ScriptShutdown(IntPtr ScriptID)
		{
			if (!Scripts.ContainsKey(ScriptID)) return false;

			Script Scr = Scripts[ScriptID];
			if (Scr.Parent != null) Scr.Parent.Children.Remove(Scr);
			Scripts.Remove(ScriptID);
			RefreshScripts();

			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		public bool ScriptChangeLine(IntPtr ScriptID, int Line)
		{
			if (!Scripts.ContainsKey(ScriptID)) return false;

			Scripts[ScriptID].OnChangeLine(Line);
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		public bool ScriptChangeScope(IntPtr ScriptID, int ScopeID)
		{
			if (!Scripts.ContainsKey(ScriptID)) return false;

			Scripts[ScriptID].CurrentScope = ScopeID;
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		public bool ScriptShutdownScope(IntPtr ScriptID, int ScopeID)
		{
			if (!Scripts.ContainsKey(ScriptID)) return false;

			Scripts[ScriptID].RemoveScope(ScopeID);
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		public bool ScriptHitBreakpoint(IntPtr ScriptID, int Line)
		{
			if (!Scripts.ContainsKey(ScriptID)) return false;

			Script Scr = Scripts[ScriptID];
			
			//DebugWindow.AddLine("Breakpoint hit: " + Scr.Filename + ", line " + Line.ToString());
			DebugWindow.SelectedScript = Scr;
			DebugWindow.RefreshVars();
			DebugWindow.Tick();
			DebugWindow.HighlightScriptLine();

			DebugWindow.Visible = true;
			DebugWindow.BringToFront();
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		public bool GlobalVariableInit(IWmeValue Variable)
		{
			GlobalScope.AddVar(Variable);

			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		public bool ScriptVariableInit(IWmeValue Variable, IntPtr ScriptID)
		{
			if (!Scripts.ContainsKey(ScriptID)) return false;
			Scripts[ScriptID].AddGlobalVar(Variable);

			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		public bool ScopeVariableInit(IWmeValue Variable, IntPtr ScriptID, int ScopeID)
		{
			if (!Scripts.ContainsKey(ScriptID)) return false;
			Scripts[ScriptID].AddScopeVar(Variable, ScopeID);

			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		public bool VariableChangeValue(IWmeValue Variable, IWmeValue Value)
		{
			ScriptScope Scope;
			Script Script;
			GetVarScope(Variable, out Script, out Scope);
			if (Scope == GlobalScope || (Script != null && Script == DebugWindow.SelectedScript))
			{
				RefreshVars = true;
			}
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		private void GetVarScope(IWmeValue Variable, out Script Script, out ScriptScope Scope)
		{
			if (GlobalScope.Vars.ContainsKey(Variable))
			{
				Scope = GlobalScope;
				Script = null;
				return;
			}
			else
			{
				foreach (KeyValuePair<IntPtr, Script> kvp in Scripts)
				{
					ScriptScope ScrScope = kvp.Value.GetVarScope(Variable);
					if (ScrScope != null)
					{
						Scope = ScrScope;
						Script = kvp.Value;
						return;
					}
				}
				Script = null;
				Scope = null;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public void Dispose()
		{
			if (DebugWindow != null && DebugWindow.Visible) DebugWindow.Close();
		}


		private static Dictionary<string, string[]> ClassProps = null;
		//////////////////////////////////////////////////////////////////////////
		public static string[] GetClassProperties(string NativeClass)
		{
			if(ClassProps==null)
			{
				ClassProps = new Dictionary<string, string[]>();
				try
				{
					string XmlDocsPath = "";
					using (RegistryKey Key = Registry.CurrentUser.OpenSubKey(@"Software\DEAD:CODE\Wintermute Tools\Settings"))
					{
						XmlDocsPath = Key.GetValue("ToolsPath", "").ToString();
					};
					XmlDocsPath = Path.Combine(XmlDocsPath, "wme_docs_core_en.xml");
					if (File.Exists(XmlDocsPath))
					{
						ScriptInfo Info = new ScriptInfo();
						if (Info.ReadXml(XmlDocsPath))
						{
							foreach (ScriptObject Obj in Info.Objects)
							{
								if (Obj.NativeClass == null || Obj.NativeClass == string.Empty) continue;

								List<string> Attrs = new List<string>(Obj.Attributes.Count);
								foreach (ScriptAttribute Attr in Obj.Attributes)
								{
									Attrs.Add(Attr.Name);
								}
								ClassProps[Obj.NativeClass] = Attrs.ToArray();
							}
						}
					}
				}
				catch
				{
				}
			}
			if (ClassProps.ContainsKey(NativeClass)) return ClassProps[NativeClass];
			else return null;
		}

	}
}
