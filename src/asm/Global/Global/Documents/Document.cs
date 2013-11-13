// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using DeadCode.WME.Core;
using System.IO;
using System.Windows.Forms;

namespace DeadCode.WME.Global
{
	public class Document
	{
		public enum DocumentOpenResult
		{
			Ok, Cancel, Error
		};

		private string _FileName = "";
		//////////////////////////////////////////////////////////////////////////
		public string FileName
		{
			get
			{
				return _FileName;
			}
			set
			{
				_FileName = value;
				OnFileNameChanged();
				OnCaptionChanged();
			}
		}

		private bool _IsDirty = false;
		//////////////////////////////////////////////////////////////////////////
		public bool IsDirty
		{
			get
			{
				return _IsDirty;
			}
			set
			{
				_IsDirty = value;
				OnCaptionChanged();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public string GetDocumentCaption()
		{
			return GetDocumentCaption("");
		}

		//////////////////////////////////////////////////////////////////////////
		public string GetDocumentCaption(string AppName)
		{
			string Ret = "";
			if (AppName != "") Ret += AppName + " - [";

			if (FileName != "") Ret += Path.GetFileName(FileName);
			else Ret += "untitled";

			if(IsDirty) Ret += "*";
			if(AppName != "") Ret += "]";

			return Ret;
		}

		//////////////////////////////////////////////////////////////////////////
		public event EventHandler CaptionChanged;		
		protected void OnCaptionChanged()
		{
			if (CaptionChanged != null) CaptionChanged(this, new EventArgs());
		}

		//////////////////////////////////////////////////////////////////////////
		public event EventHandler FileNameChanged;
		protected void OnFileNameChanged()
		{
			if (FileNameChanged != null) FileNameChanged(this, new EventArgs());
		}


		#region Undo system
		private int _MaxUndoLevel = 100;
		//////////////////////////////////////////////////////////////////////////
		public int MaxUndoLevel
		{
			get
			{
				return _MaxUndoLevel;
			}
			set
			{
				_MaxUndoLevel = value;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private class UndoState
		{
			public UndoState(string Name, byte[] State, bool IsCompressed, int OrigSize)
			{
				this.Name = Name;
				this.State = State;
				this.IsCompressed = IsCompressed;
				this.OrigSize = OrigSize;
			}
			public string Name;
			public byte[] State;
			public bool IsCompressed;
			public int OrigSize;
		}

		private List<UndoState> UndoStates = new List<UndoState>();
		private int UndoPointer = -1;
		//////////////////////////////////////////////////////////////////////////
		public bool SaveUndoState(string Name)
		{
			IsDirty = true;

			string StrState = GetCurrentStateForUndo();
			if (StrState == null) return false;

			UndoPointer++;

			while (UndoStates.Count > UndoPointer)
				UndoStates.RemoveAt(UndoStates.Count - 1);

			try
			{
				System.Text.UTF8Encoding enc = new System.Text.UTF8Encoding();
				byte[] OrigBuf = enc.GetBytes(StrState);
				byte[] CompBuf = WUtils.CompressBuffer(OrigBuf);

				UndoState State;
				if (CompBuf != null)
					State = new UndoState(Name, CompBuf, true, OrigBuf.Length);
				else
					State = new UndoState(Name, OrigBuf, false, OrigBuf.Length);

				UndoStates.Add(State);

				if(UndoStates.Count > MaxUndoLevel)
				{
					UndoStates.RemoveAt(0);
					UndoPointer--;
				}

				return true;
			}
			catch
			{
				UndoPointer--;
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public virtual string GetCurrentStateForUndo()
		{
			return null;
		}

		//////////////////////////////////////////////////////////////////////////
		public string GetUndo()
		{			
			if (UndoPointer >= 0)
			{
				if (UndoPointer == UndoStates.Count - 1)
				{
					if (SaveUndoState("")) UndoPointer--;
				}

				UndoPointer--;
				return GetUndoState(UndoPointer + 1);
			}
			else return null;
		}

		//////////////////////////////////////////////////////////////////////////
		public string GetRedo()
		{
			if (UndoPointer < UndoStates.Count - 2)
			{
				UndoPointer++;
				return GetUndoState(UndoPointer + 1);
			}
			else return null;
		}

		//////////////////////////////////////////////////////////////////////////
		public void KillUndo()
		{
			UndoStates.Clear();
			UndoPointer = -1;
		}

		//////////////////////////////////////////////////////////////////////////
		public bool UndoAvailable
		{
			get
			{
				return UndoPointer >= 0;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public string UndoName
		{
			get
			{
				if (UndoPointer >= 0 && UndoPointer < UndoStates.Count)
					return UndoStates[UndoPointer].Name;
				else
					return "";
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public string RedoName
		{
			get
			{
				if (UndoPointer < UndoStates.Count - 2)
					return UndoStates[UndoPointer+1].Name;
				else
					return "";
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public bool RedoAvailable
		{
			get
			{
				return UndoPointer < UndoStates.Count - 2;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private string GetUndoState(int Pos)
		{
			if (Pos < 0 || Pos >= UndoStates.Count) return null;

			UndoState State = UndoStates[Pos];
			byte[] StrBuf;
			if (State.IsCompressed) StrBuf = WUtils.DecompressBuffer(State.State, State.OrigSize);
			else StrBuf = State.State;

			System.Text.UTF8Encoding enc = new System.Text.UTF8Encoding();
			return enc.GetString(StrBuf);
		}
		#endregion


		#region Project selection
		//////////////////////////////////////////////////////////////////////////
		protected string GetProjectFile(string DefinitionFile, FormBase ParentForm)
		{
			if (DefinitionFile == null) return GetProjectFile(ParentForm);

			try
			{
				string ProjectPath = "";
				if (File.Exists(DefinitionFile))
				{
					using (StreamReader sr = new StreamReader(DefinitionFile, Encoding.Default, true))
					{
						string Line;
						string Magic = "; $EDITOR_PROJECT_ROOT_DIR$ ";
						while ((Line = sr.ReadLine()) != null)
						{
							if (Line.StartsWith(Magic))
							{
								ProjectPath = Line.Substring(Magic.Length).Trim();
								break;
							}
						}
					}
				}
				if (ProjectPath != "")
				{
					string FilePath = Path.GetDirectoryName(DefinitionFile);
					ProjectPath = Path.GetFullPath(Path.Combine(FilePath, ProjectPath));
					if (Directory.Exists(ProjectPath))
					{
						string[] ProjectFiles = Directory.GetFiles(ProjectPath, "*.wpr");
						if (ProjectFiles.Length > 0)
						{
							Document.AddRecentProject(ProjectFiles[0]);
							return ProjectFiles[0];
						}
					}
				}

				// not found, we'll scan all the directories in file path
				string TestPath = Path.GetDirectoryName(DefinitionFile);
				while (true)
				{
					string[] ProjectFiles = Directory.GetFiles(TestPath, "*.wpr");
					if (ProjectFiles.Length > 0)
					{
						Document.AddRecentProject(ProjectFiles[0]);
						return ProjectFiles[0];
					}

					int i = TestPath.LastIndexOfAny(new char[] { '\\', '/' });
					if (i < 0) break;

					TestPath = TestPath.Substring(0, i);
				}

				// still not found, ask user
				return GetProjectFile(ParentForm);
			}
			catch (IOException)
			{
				return GetProjectFile(ParentForm);
			}
			catch (UnauthorizedAccessException)
			{
				return GetProjectFile(ParentForm);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		protected string GetProjectFile(FormBase ParentForm)
		{
			ProjectSelectionForm dlg = new ProjectSelectionForm();
			
			if (ParentForm != null)
				dlg.AppMgr = ParentForm.AppMgr;

			dlg.RecentFiles = Document.RecentProjects;
			if (dlg.ShowDialog() == DialogResult.OK)
			{
				Document.AddRecentProject(dlg.FileName);
				return dlg.FileName;
			}
			else return null;
		}


		private static List<string> _RecentProjects;
		//////////////////////////////////////////////////////////////////////////
		public static List<string> RecentProjects
		{
			get
			{
				if (_RecentProjects == null) _RecentProjects = new List<string>();
				return _RecentProjects;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		public static void AddRecentProject(string ProjectFile)
		{
			StringComparer Comparer = StringComparer.InvariantCultureIgnoreCase;

			if (RecentProjects.BinarySearch(ProjectFile, Comparer) < 0)
			{
				RecentProjects.Add(ProjectFile);
				RecentProjects.Sort(Comparer);
			}
		}
		#endregion
	
	}
}
