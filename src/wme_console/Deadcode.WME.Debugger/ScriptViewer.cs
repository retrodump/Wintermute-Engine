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
using System.IO;

namespace DeadCode.WME.Debugger
{
	public partial class ScriptViewer : ListView
	{
		public ScriptViewer()
		{
			InitializeComponent();
		}

						
		private Script _Script = null;
		//////////////////////////////////////////////////////////////////////////
		public Script Script
		{
			get
			{
				return _Script;
			}
		}

		private DebugClient _Client = null;
		//////////////////////////////////////////////////////////////////////////
		public DebugClient Client
		{
			get
			{
				return _Client;
			}
		}

		private bool SourceValid = false;
		//////////////////////////////////////////////////////////////////////////
		public void LoadScript(DebugClient Client, Script Script)
		{
			_Client = Client;
			_Script = Script;


			this.BeginUpdate();
			this.Items.Clear();

			SourceValid = false;
			
			if (_Client != null && _Script != null)
			{
				string FullPath = _Client.Server.ResolveFilename(_Script.Filename);
				ReadLines(FullPath);
				SelectLine(Script.Line, true);
			}


			foreach (ColumnHeader Col in this.Columns)
			{
				Col.Width = -1;
			}
			this.EndUpdate();

			RefreshBreakpoints();
		}

		//////////////////////////////////////////////////////////////////////////
		private void ReadLines(string FullPath)
		{
			bool FileOK = false;

			if (FullPath != null && File.Exists(FullPath))
			{
				try
				{
					using (StreamReader sr = new StreamReader(FullPath, Encoding.Default, true))
					{
						string Line;
						int LineNum = 0;
						while ((Line = sr.ReadLine()) != null)
						{
							LineNum++;

							ListViewItem Item = Items.Add(LineNum.ToString("000"));
							Item.SubItems.Add(Line.Replace("\t", "  "));
						}
						LineNum++;
						Items.Add(LineNum.ToString("000"));
					}
					FileOK = true;
				}
				catch(IOException)
				{
					FileOK = false;
				}
			}

			if (!FileOK)
			{
				this.Items.Clear();
				this.Items.Add("Source code is not available");
				SourceValid = false;
				return;
			}
			else SourceValid = true;

		}

		private int SelectedLine = -1;
		//////////////////////////////////////////////////////////////////////////
		private void SelectLine(int LineNum, bool EnsureVisible)
		{
			SelectedLine = LineNum;

			LineNum--;
			foreach(ListViewItem Item in Items)
			{
				if (Item.Index == LineNum)
				{
					if (!Item.Font.Bold) Item.Font = new Font(Font, FontStyle.Bold);
					Item.BackColor = Color.LightYellow;
					Item.ForeColor = Color.Black;
				}
				else
				{
					if (Item.Font.Bold) Item.Font = new Font(Font, FontStyle.Regular);
					Item.BackColor = this.BackColor;
					Item.ForeColor = this.ForeColor;
				}
			}
			if (EnsureVisible && LineNum >= 0 && LineNum < Items.Count)
				this.EnsureVisible(LineNum);			
		}


		//////////////////////////////////////////////////////////////////////////
		public void RefreshLine()
		{
			if(_Script != null && _Script.Line != SelectedLine && SourceValid)
			{
				this.BeginUpdate();
				SelectLine(_Script.Line, true);
				this.EndUpdate();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public void HighlightLine()
		{
			if (_Script != null && SourceValid)
			{
				if (_Script.Line > 0 && _Script.Line <= this.Items.Count)
				{
					this.Items[_Script.Line - 1].Selected = true;
					this.Items[_Script.Line - 1].Focused = true;
					this.EnsureVisible(_Script.Line - 1);
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public void RefreshBreakpoints()
		{
			if(_Script != null && SourceValid)
			{
				int[] BreakpointLines = _Script.BreakpointLines;
				Array.Sort(BreakpointLines);

				this.BeginUpdate();
				foreach(ListViewItem Item in this.Items)
				{
					if (Array.BinarySearch(BreakpointLines, Item.Index + 1) >= 0) Item.ImageIndex = 0;
					else Item.ImageIndex = -1;
				}

				this.EndUpdate();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		internal bool CanSetBreakpoint()
		{
			return _Script != null && SourceValid && SelectedItems.Count > 0;
		}
	}
}
