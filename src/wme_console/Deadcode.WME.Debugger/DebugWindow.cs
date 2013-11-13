// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using DeadCode.WME.Global;
using System.Media;
using Microsoft.Win32;

namespace DeadCode.WME.Debugger
{
	public partial class DebugWindow : FormBase
	{
		private WatchProvider Watch;

		//////////////////////////////////////////////////////////////////////////
		public DebugWindow()
		{
			Watch = new WatchProvider();
			InitializeComponent();
			DisplayScript(null);
			AppMgr = new ApplicationMgr("DebugConsole");			

			GridWatch.SelectedObject = Watch;
		}

		private DebugClient _Client = null;
		//////////////////////////////////////////////////////////////////////////
		public DebugWindow(DebugClient Client) : this()
		{
			this._Client = Client;
		}

		//////////////////////////////////////////////////////////////////////////
		public void AddLogLine(string Line)
		{
			int Start = LogText.Text.Length;
			LogText.SelectionLength = 0;
			LogText.AppendText(Line + "\n");

			LogText.SelectionStart = Start;
			LogText.SelectionLength = LogText.Text.Length - Start;

			if(Line.ToLower().Contains("error")) LogText.SelectionColor = Color.Red;
			else if (Line.ToLower().Contains("warning")) LogText.SelectionColor = Color.Purple;
			
			
			LogText.SelectionStart = LogText.Text.Length;
			LogText.SelectionLength = 0;
			LogText.ScrollToCaret();
		}

		//////////////////////////////////////////////////////////////////////////
		public void ClearLog()
		{
			LogText.Text = "";
		}

		public void DisplayVars(ScriptScope Scope)
		{
			GridGlobals.SelectedObject = Scope;
			Watch.Refresh(Scope);
		}

		public void RefreshVars()
		{
			GridVars.Refresh();
			GridGlobals.Refresh();
			GridWatch.Refresh();
		}


		class ScriptPath : IComparable<ScriptPath>
		{
			public string Path;
			public List<Script> Scripts;

			public ScriptPath(string Path)
			{
				this.Path = Path;
				Scripts = new List<Script>();
			}

			public int CompareTo(ScriptPath o)
			{
				return string.Compare(this.Path, o.Path);
			}

		};
		//////////////////////////////////////////////////////////////////////////
		public void RefreshScripts(Script[] Scripts)
		{
			List<ScriptPath> Paths = new List<ScriptPath>();

			foreach(Script Scr in Scripts)
			{
				if (Scr.Parent != null) continue;

				ScriptPath Path = new ScriptPath(System.IO.Path.GetDirectoryName(Scr.Filename.ToLower()));
				int Index = Paths.BinarySearch(Path);
				if(Index < 0)
				{
					Paths.Add(Path);
					Paths.Sort();
				}
				else Path = Paths[Index];
				Path.Scripts.Add(Scr);
			}
			TreeScripts.BeginUpdate();

			int NodeIndex = 0;
			foreach(ScriptPath Path in Paths)
			{
				TreeNode Node = null;
				while (Node == null && NodeIndex < TreeScripts.Nodes.Count)
				{
					Node = TreeScripts.Nodes[NodeIndex];
					if (Node.Text != Path.Path)
					{
						if (string.Compare(Node.Text, Path.Path) < 0)
						{
							TreeScripts.Nodes.RemoveAt(NodeIndex);
							Node = null;
						}
						else
						{
							Node = null;
							break;
						}
					}
					else NodeIndex++;
				}
				if (Node == null)
				{
					Node = TreeScripts.Nodes.Insert(NodeIndex, Path.Path);
					Node.ImageIndex = 1;
					Node.SelectedImageIndex = 0;
					NodeIndex++;
				}
				UpdateScriptPathNode(Node, Path);
			}
			for (int i = NodeIndex; i < TreeScripts.Nodes.Count; i++)
				TreeScripts.Nodes.RemoveAt(NodeIndex);

			TreeScripts.EndUpdate();

			Watch.Refresh();
			GridWatch.Refresh();
		}

		//////////////////////////////////////////////////////////////////////////
		private void UpdateScriptPathNode(TreeNode ParentNode, ScriptPath Path)
		{
			Path.Scripts.Sort(new Script.ScriptFilenameComparer());

			int NodeIndex = 0;
			foreach (Script Scr in Path.Scripts)
			{
				string NodeName = System.IO.Path.GetFileName(Scr.Filename);
				TreeNode Node = null;
				while (Node == null && NodeIndex < ParentNode.Nodes.Count)
				{
					Node = ParentNode.Nodes[NodeIndex];
					if (Node.Tag != Scr)
					{
						if (string.Compare(Node.Text, NodeName) < 0)
						{
							ParentNode.Nodes.RemoveAt(NodeIndex);
							Node = null;
						}
						else
						{
							Node = null;
							break;
						}
					}
					else NodeIndex++;
				}
				if (Node == null)
				{
					Node = ParentNode.Nodes.Insert(NodeIndex, NodeName);					
					Node.Tag = Scr;
					NodeIndex++;
				}
				Node.ImageIndex = Node.SelectedImageIndex = ScriptStateToImage(Scr);
				UpdateScriptNode(Node, Scr);
			}
			for (int i = NodeIndex; i < ParentNode.Nodes.Count; i++)
				ParentNode.Nodes.RemoveAt(NodeIndex);

		}

		//////////////////////////////////////////////////////////////////////////
		private void UpdateScriptNode(TreeNode ParentNode, Script ParentScript)
		{
			ParentScript.Children.Sort(new Script.ScriptThreadNameComparer());

			int NodeIndex = 0;
			foreach (Script Scr in ParentScript.Children)
			{
				string NodeName = Scr.ThreadName;
				TreeNode Node = null;
				while (Node == null && NodeIndex < ParentNode.Nodes.Count)
				{
					Node = ParentNode.Nodes[NodeIndex];
					if (Node.Tag != Scr)
					{
						if (string.Compare(Node.Text, NodeName) < 0)
						{
							ParentNode.Nodes.RemoveAt(NodeIndex);
							Node = null;
						}
						else
						{
							Node = null;
							break;
						}
					}
					else NodeIndex++;
				}
				if (Node == null)
				{
					Node = ParentNode.Nodes.Insert(NodeIndex, NodeName);					
					Node.Tag = Scr;
					NodeIndex++;
				}
				Node.ImageIndex = Node.SelectedImageIndex = ScriptStateToImage(Scr);
				UpdateScriptNode(Node, Scr);
			}
			for (int i = NodeIndex; i < ParentNode.Nodes.Count; i++)
				ParentNode.Nodes.RemoveAt(NodeIndex);

			if (ParentNode.Nodes.Count > 0) ParentNode.Expand();
		}

		//////////////////////////////////////////////////////////////////////////
		private int ScriptStateToImage(Script Scr)
		{
			switch(Scr.State)
			{
				case EWmeScriptState.Running:
					return 2;
				case EWmeScriptState.Persistent:
					return 3;
				case EWmeScriptState.WaitingForObject:
				case EWmeScriptState.WaitingForScript:
				case EWmeScriptState.Sleeping:
					return 5;
				case EWmeScriptState.Error:
					return 6;
				default:
					return 4;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnScriptSelection(object sender, TreeViewEventArgs e)
		{
			DisplayScript(SelectedScript);
		}

		//////////////////////////////////////////////////////////////////////////
		public Script SelectedScript
		{
			get
			{
				if (TreeScripts.SelectedNode != null) return TreeScripts.SelectedNode.Tag as Script;
				else return null;
			}
			set
			{
				if (value == null) TreeScripts.SelectedNode = null;
				else
				{
					if (!SelectScriptByTag(TreeScripts.Nodes, value)) TreeScripts.SelectedNode = null;
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private bool SelectScriptByTag(TreeNodeCollection Nodes, Script Scr)
		{
			foreach(TreeNode Node in Nodes)
			{
				if (Node.Tag as Script == Scr)
				{
					TreeScripts.SelectedNode = Node;
					return true;
				}
				else if (SelectScriptByTag(Node.Nodes, Scr))
				{
					Node.Expand();
					return true;
				}
			}
			return false;
		}

		//////////////////////////////////////////////////////////////////////////
		private void DisplayScript(Script Scr)
		{
			if (Scr != null)
			{
				GridVars.SelectedObject = Scr.VarProvider;				
				ScrViewer.LoadScript(_Client, Scr);
				Watch.Refresh(Scr);
				GridWatch.Refresh();
			}
			else
			{
				GridVars.SelectedObject = null;
				ScrViewer.LoadScript(_Client, null);
				Watch.Refresh(Scr);
				GridWatch.Refresh();
			}
			RefreshScriptInfo(ScrViewer.Script);

		}

		//////////////////////////////////////////////////////////////////////////
		private void RefreshScriptInfo(Script Scr)
		{
			if (Scr != null)
			{
				LblScriptInfo.Text = Scr.Filename + ", line " + Scr.Line.ToString();
			}
			else
			{
				LblScriptInfo.Text = "";
			}			
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnLoad(object sender, EventArgs e)
		{
			AppMgr.Settings.LoadFromXmlFile();
			LoadLayout(AppMgr.Settings);

			SettingsNode Node = AppMgr.Settings.GetNode("Layout\\" + this.Name);
			if (Node != null) this.TopMost = Node.GetBool("FormTopMost", false);
			BtnAlwaysOnTop.Checked = this.TopMost;

			// load watches from project's registry
			try
			{
				string RegPath = _Client.Server.GetPropString("RegistryPath");
				RegPath += @"\Debug";
				using (RegistryKey Key = Registry.CurrentUser.OpenSubKey(RegPath))
				{
					int NumWatches = int.Parse(Key.GetValue("NumWatches", 0).ToString());
					for(int i=0; i<NumWatches; i++)
					{
						string WatchName = Key.GetValue("Watch" + (i + 1).ToString(), "").ToString();
						if (WatchName != string.Empty) Watch.AddWatch(WatchName);
					}
				}
			}
			catch
			{

			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnFormClosing(object sender, FormClosingEventArgs e)
		{
			SettingsNode Node = AppMgr.Settings.GetNode("Layout\\" + this.Name, false, true);
			if (Node != null) Node.SetValue("FormTopMost", this.TopMost);

			SaveLayout(AppMgr.Settings);
			AppMgr.Settings.SaveToXmlFile();

			// save watches to project's registry
			try
			{
				string RegPath = _Client.Server.GetPropString("RegistryPath");
				RegPath += @"\Debug";
				using (RegistryKey Key = Registry.CurrentUser.OpenSubKey(RegPath, true))
				{
					int OrigNumWatches = int.Parse(Key.GetValue("NumWatches", 0).ToString());
					
					string[] WatchNames = Watch.GetWatchNames();
					int i = 1;
					foreach(string WatchName in WatchNames)
					{
						Key.SetValue("Watch" + i.ToString(), WatchName);
						i++;
					}
					Key.SetValue("NumWatches", WatchNames.Length);
					for (i = WatchNames.Length + 1; i <= OrigNumWatches; i++)
					{
						Key.DeleteValue("Watch" + i.ToString(), false);
					}
				}
			}
			catch
			{

			}

			this.Visible = false;
			e.Cancel = true;
		}


		//////////////////////////////////////////////////////////////////////////
		public void Tick()
		{
			RefreshScriptInfo(ScrViewer.Script);
			ScrViewer.RefreshLine();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnAlwaysOnTop(object sender, EventArgs e)
		{
			this.TopMost = BtnAlwaysOnTop.Checked;
		}

		
		//////////////////////////////////////////////////////////////////////////
		internal void Shutdown()
		{
			TreeScripts.SelectedNode = null;
			DisplayVars(null);
			DisplayScript(null);
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnToggleBreak(object sender, EventArgs e)
		{
			if (ScrViewer.CanSetBreakpoint())
			{
				int Line = ScrViewer.SelectedIndices[0] + 1;

				if (ScrViewer.Script.BreakpointAtLine(Line))
					_Client.Server.RemoveBreakpoint(ScrViewer.Script.Filename, Line);
				else
					_Client.Server.AddBreakpoint(ScrViewer.Script.Filename, Line);

				ScrViewer.RefreshBreakpoints();
			}
			else SystemSounds.Exclamation.Play();
		}

		//////////////////////////////////////////////////////////////////////////
		internal void HighlightScriptLine()
		{
			ScrViewer.HighlightLine();
			ScrViewer.Focus();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnKeyDown(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.F9)
			{
				if (BtnToggleBreak.Enabled) BtnToggleBreak.PerformClick();
			}
			else if (e.KeyCode == Keys.F5)
			{
				if (BtnContinue.Enabled) BtnContinue.PerformClick();
			}
			// simulate both step-into and step-over shortcuts known from VS
			else if (e.KeyCode == Keys.F10 || e.KeyCode == Keys.F11) 
			{
				if (BtnStep.Enabled) BtnStep.PerformClick();
			}

			else if (e.KeyCode == Keys.Escape) _Client.Server.ContinueExecution();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnContinue(object sender, EventArgs e)
		{
			if (ScrViewer.Script != null) ScrViewer.Script.TracingMode = false;
			_Client.Server.ContinueExecution();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnStep(object sender, EventArgs e)
		{
			if (ScrViewer.Script != null) ScrViewer.Script.TracingMode = true;
			_Client.Server.ContinueExecution();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnMenuWatch(object sender, CancelEventArgs e)
		{
			MenuWatch.Items.Clear();
			MenuWatch.Items.Add("Add watch", null, new EventHandler(OnAddWatch));

			if (GridWatch.SelectedGridItem != null && GridWatch.SelectedGridItem.GridItemType != GridItemType.Root)
				MenuWatch.Items.Add("Remove watch", null, new EventHandler(OnRemoveWatch));

			e.Cancel = false;
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnAddWatch(object sender, EventArgs e)
		{
			FormWatch Dlg = new FormWatch();
			if(Dlg.ShowDialog()==DialogResult.OK)
			{
				Watch.AddWatch(Dlg.TxtName.Text);
				GridWatch.Refresh();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnRemoveWatch(object sender, EventArgs e)
		{
			GridItem SelItem = GridWatch.SelectedGridItem;
			if (SelItem != null && SelItem.GridItemType != GridItemType.Root)
			{
				Watch.RemoveWatch(SelItem.Label);
				GridWatch.Refresh();
			}
		}

	}
}