using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.Reflection;
using System.IO;
using DeadCode.WME.Global;


namespace DeadCode.WME.StringTableMgr
{
	#region Delegates
	public delegate void ProgressChangeDelegate(object sender, ProgressChangeEventArgs e);
	public class ProgressChangeEventArgs : EventArgs
	{
		public readonly int CurrentItem;
		public readonly int TotalItems;
		public readonly string Description;

		public ProgressChangeEventArgs(int CurrentItem, int TotalItems, string Description)
		{
			this.CurrentItem = CurrentItem;
			this.TotalItems = TotalItems;
			this.Description = Description;
		}
	};

	public enum LogSeverity
	{
		Information, Warning, Error
	};

	public delegate void AddLogDelegate(object sender, AddLogEventArgs e);
	public class AddLogEventArgs : EventArgs
	{
		public readonly LogSeverity Severity;
		public readonly string Message;

		public AddLogEventArgs(LogSeverity Severity, string Message)
		{
			this.Severity = Severity;
			this.Message = Message;
		}
	};
	#endregion

	public class StringItem
	{
		public string ID;
		public string Value;
		public string Filename;
		public int Line;
		public bool Ignored;
		public IgnoreReason IgnoreReason;
		public string OriginalString;
		public bool IsScriptItem;
		public int Order;
		public bool SavedOk = false;


		//////////////////////////////////////////////////////////////////////////
		public static int CompareByLine(StringItem p1, StringItem p2)
		{
			if (p1 == null)
			{
				if (p2 == null) return 0;
				else return -1;
			}
			else
			{
				if (p2 == null) return 1;
				else
				{
					if (p1.Line < p2.Line) return -1;
					else if (p1.Line > p2.Line) return 1;
					else return 0;
				}
			}
		}

	};

	public enum IgnoreReason
	{
		InIgnoreList,
		AlreadyInTable,
		KnownCodePattern,
		IsFilename,
		SelectedByUser,
	}

	//////////////////////////////////////////////////////////////////////////
	public class StringTableMgr
	{
		public string ProjectFile;
		public string StringTableFile;
		public bool BackupOldFiles = true;
		public List<string> IgnoreList = new List<string>();


		//////////////////////////////////////////////////////////////////////////
		public StringTableMgr()
		{
			IgnoreList.Add("savegame:");
			IgnoreList.Add(",g");
		}
		
		public event ProgressChangeDelegate OnProgressChange;
		//////////////////////////////////////////////////////////////////////////
		public void ReportProgress(int CurrentItem, int TotalItems, string Description)
		{
			if (OnProgressChange != null)
				OnProgressChange(this, new ProgressChangeEventArgs(CurrentItem, TotalItems, Description));
		}

		public event AddLogDelegate OnAddLog;
		//////////////////////////////////////////////////////////////////////////
		public void AddLog(LogSeverity Severity, string Message, bool NewLine)
		{
			if (OnAddLog != null)
				OnAddLog(this, new AddLogEventArgs(Severity, Message + (NewLine?"\n":"")));
		}

		//////////////////////////////////////////////////////////////////////////
		public void AddLog(string Message, bool NewLine)
		{
			AddLog(LogSeverity.Information, Message, NewLine);
		}

		//////////////////////////////////////////////////////////////////////////
		public void AddLog(string Message)
		{
			AddLog(Message, true);
		}

		public List<StringItem> ProjectStrings;
		public List<StringItem> IgnoredStrings;
		public List<StringItem> TableStrings;
		//////////////////////////////////////////////////////////////////////////
		public bool ScanProject()
		{
			// load project strings
			AddLog("Scanning project...", false);

			ProjectStrings = new List<StringItem>();
			StringExtractor se = new StringExtractor(this);
			bool Ret = se.ExtractProjectStrings(this.ProjectFile, "*.script;*.inc", "*.scene;*.entity;*.actor;*.window;*.items;*.game");
			if (!Ret) return false;

			int Order = 0;
			foreach(StringLocation Str in se.Strings)
			{
				Order++;

				StringItem Item = new StringItem();
				ExtractStringParts(Str.Value, out Item.ID, out Item.Value);
				Item.Line = Str.Line;
				Item.Filename = Str.Filename;
				Item.OriginalString = Str.Value;
				Item.IsScriptItem = (Str.Type == StringLocation.StringType.ScriptFile);
				Item.Order = Order;

				Item.Value = Item.Value.Replace("~n", "|");
				Item.Value = Item.Value.Replace("~\"", "\"");

				ProjectStrings.Add(Item);
			}
			AddLog(ProjectStrings.Count.ToString() + " strings found");

			// load string table string
			AddLog("Reading string table...", false);
			LoadStringTable(StringTableFile);
			AddLog(TableStrings.Count.ToString() + " strings found");
			if (StringTableUsingUtf8) AddLog("String table is using UTF-8 format");

			// perform initial string classification
			ClassifyStrings();

			return true;
		}

		private bool StringTableUsingUtf8 = true;
		//////////////////////////////////////////////////////////////////////////
		private bool LoadStringTable(string Filename)
		{
			TableStrings = new List<StringItem>();
			StringTableUsingUtf8 = true;

			try
			{
				using(FileStream Stream = new FileStream(Filename, FileMode.Open, FileAccess.Read, FileShare.Read))
				{
					StreamReader Reader = new StreamReader(Stream, Encoding.Default, true);

					string Line;
					int LineNum = 0;
					while ((Line = Reader.ReadLine()) != null)
					{
						LineNum++;

						if (Line.StartsWith(";")) continue;

						string[] LineSplit = Line.Split(new char[] { '\t' });
						if (LineSplit.Length != 2) continue;

						StringItem Item = new StringItem();
						Item.ID = LineSplit[0];
						Item.Value = LineSplit[1];
						Item.Filename = Filename;
						Item.Line = LineNum;

						TableStrings.Add(Item);
					}
					StringTableUsingUtf8 = (Reader.CurrentEncoding == Encoding.UTF8);

					return true;
				}
			}
			catch
			{
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void ClassifyStrings()
		{
			AddLog("Classifying strings...", false);

			IgnoredStrings = new List<StringItem>();
			
			int CurrentIndex = 0;
			foreach(StringItem Item in ProjectStrings)
			{
				CurrentIndex++;
				ReportProgress(CurrentIndex, ProjectStrings.Count, Item.Value);

				Item.Ignored = false;

				// remove strings already present in the string table
				if(Item.ID != string.Empty && IsIDInTable(Item.ID))
				{
					Item.Ignored = true;
					Item.IgnoreReason = IgnoreReason.AlreadyInTable;
					continue;
				}

				// keep strings with ID
				if (Item.ID != string.Empty) continue;

				// keep non-script strings
				if (!Item.IsScriptItem) continue;


				// remove strings in ignore list
				if(IsStringIgnored(Item.Value))
				{
					Item.Ignored = true;
					Item.IgnoreReason = IgnoreReason.InIgnoreList;
					continue;
				}

				// remove filenames
				WmeProject Project = new WmeProject(this.ProjectFile);
				if(Project.ContainsFile(Item.Value))
				{
					Item.Ignored = true;
					Item.IgnoreReason = IgnoreReason.IsFilename;
					continue;
				}

				// check one-word terms against well known code patterns
				if (Item.Value.IndexOf(' ') < 0 && IsWellKnownPattern(Item))
				{
					Item.Ignored = true;
					Item.IgnoreReason = IgnoreReason.KnownCodePattern;
					continue;
				}
			}

			for(int i=0; i<ProjectStrings.Count; i++)
			{
				StringItem Item = ProjectStrings[i];
				if(Item.Ignored)
				{
					IgnoredStrings.Add(Item);
					ProjectStrings.RemoveAt(i);
					i--;
				}
			}
			ReportProgress(0, 0, "");

			AddLog(IgnoredStrings.Count.ToString() + " strings automatically ignored");
		}

		private string[] ScriptPatterns = new string[]
		{
			".SelectedItem=\"%string%\"",
			".SelectedItem==\"%string%\"",
			".SelectedItem!=\"%string%\"",
			".CanHandleEvent(\"%string%\")",
			".CanHandleMethod(\"%string%\")",
			".ApplyEvent(\"%string%\")",
			".GetNode(\"%string%\")",
			".GetLayer(\"%string%\")",
			".GetControl(\"%string%\")",
			".GetWidget(\"%string%\")",
			".GetItem(\"%string%\")",
			".TakeItem(\"%string%\")",
			".DropItem(\"%string%\")",
			".QueryItem(\"%string%\")",
			".IsItemTaken(\"%string%\")",
			".HasItem(\"%string%\")",
			".Type==\"%string%\"",
			".Type!=\"%string%\"",
			".Name==\"%string%\"",
			".Name!=\"%string%\"",
			".PrevScene==\"%string%\"",
			".PrevScene!=\"%string%\"",
			"case\"%string%\":",
			".CreateEntity(\"%string%\")",
			".CreateWindow(\"%string%\")",
			".CreateButton(\"%string%\")",
			".CreateStatic(\"%string%\")",
			".CreateEditor(\"%string%\")",
			".StickToRegion(\"%string%\")",
			".RegReadString(\"%string%\"",
			".RegReadNumber(\"%string%\"",
			".RegWriteString(\"%string%\"",
			".RegWriteNumber(\"%string%\"",
			".PlayAnim(\"%string%\")",
			".PlayAnimAsync(\"%string%\")",
            "Game.StartDlgBranch(\"%string%\")",
            "Game.EndDlgBranch(\"%string%\")",
		};

		//////////////////////////////////////////////////////////////////////////
		private bool IsWellKnownPattern(StringItem Item)
		{
			string ScriptLine = GetFileLine(Item.Filename, Item.Line);
			ScriptLine = ScriptLine.ToUpper();
			ScriptLine = ScriptLine.Replace(" ", "");
			ScriptLine = ScriptLine.Replace("\t", "");

			foreach(string ScriptPattern in ScriptPatterns)
			{
				string Pattern = ScriptPattern.Replace("%string%", Item.OriginalString);
				Pattern = Pattern.ToUpper();

				if (ScriptLine.IndexOf(Pattern) >= 0) return true;
			}
			return false;
		}

		//////////////////////////////////////////////////////////////////////////
		private bool IsStringIgnored(string Str)
		{
			return IgnoreList.Contains(Str);
		}

		//////////////////////////////////////////////////////////////////////////
		private bool IsIDInTable(string ID)
		{
			foreach(StringItem Item in TableStrings)
			{
				if (string.Compare(ID, Item.ID, true)==0) return true;
			}
			return false;
		}


		//////////////////////////////////////////////////////////////////////////
		private void ExtractStringParts(string Str, out string ID, out string Value)
		{
			ID = "";
			Value = Str;
			if(Str.Length > 2 && Str.StartsWith("/"))
			{
				int i = Str.IndexOf('/', 1);
				if(i>=0)
				{
					ID = Str.Substring(1, i - 1);
					Value = Str.Substring(i + 1);
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private string GetFileLine(string Filename, int Line)
		{
			try
			{
				using (StreamReader sr = new StreamReader(Filename, Encoding.Default, true))
				{
					string ReadLine;
					int LineNum = 0;
					while ((ReadLine = sr.ReadLine()) != null)
					{
						LineNum++;
						if (LineNum == Line) return ReadLine;
					}
				}
				return "";
			}
			catch
			{
				return "";
			}
		}


		//////////////////////////////////////////////////////////////////////////
		public class StringID : IComparable<StringID>
		{
			public string ID;
			public int MaxNum;

			public StringID(string ID, int MaxNum)
			{
				this.ID = ID;
				this.MaxNum = MaxNum;
			}

			public int CompareTo(StringID o)
			{
				return string.Compare(this.ID, o.ID);
			}
		};

		//////////////////////////////////////////////////////////////////////////
		public StringID[] GetUsedIDs()
		{
			List<StringID> IdList = new List<StringID>();

			foreach(StringItem StrItem in ProjectStrings)
				AddID(IdList, StrItem);

			foreach (StringItem StrItem in TableStrings)
				AddID(IdList, StrItem);

			return IdList.ToArray();
		}

		//////////////////////////////////////////////////////////////////////////
		private void AddID(List<StringID> IdList, StringItem StrItem)
		{
			if (StrItem.ID == string.Empty) return;

			string StrID;
			int MaxNum;
			GetIDBase(StrItem.ID, out StrID, out MaxNum);
			if (StrID == string.Empty) return;

			StringID ID = new StringID(StrID, MaxNum);
			int Index = IdList.BinarySearch(ID);
			if (Index < 0)
			{
				IdList.Add(ID);
				IdList.Sort();
			}
			else IdList[Index].MaxNum = Math.Max(IdList[Index].MaxNum, MaxNum);
		}

		//////////////////////////////////////////////////////////////////////////
		public void GetIDBase(string ID, out string IdBase, out int MaxNum)
		{
			IdBase = "";
			string Num = "";

			bool InId = true;
			for (int i = 0; i < ID.Length; i++)
			{
				char c = ID[i];
				if (InId)
				{
					if (char.IsLetter(c)) IdBase += c;
					else
					{
						InId = false;
						if (char.IsDigit(c)) Num += c;
					}
				}
				else
				{
					if (char.IsDigit(c)) Num += c;
				}
			}
			IdBase = IdBase.ToLower();

			if (Num != string.Empty) MaxNum = int.Parse(Num);
			else MaxNum = 0;
		}


		private class FileToChange : IComparable<FileToChange>
		{
			public string Filename;
			public List<StringItem> Items;

			public FileToChange(string Filename)
			{
				this.Filename = Filename;
				this.Items = new List<StringItem>();
			}

			public int CompareTo(FileToChange o)
			{
				return string.Compare(this.Filename, o.Filename, true);
			}

		};

		//////////////////////////////////////////////////////////////////////////
		public bool SaveChanges()
		{
			bool Ret = true;

			List<FileToChange> Files = new List<FileToChange>();

			AddLog("Preparing file list...");
			ReportProgress(0, 0, "Preparing file list...");
			foreach(StringItem StrItem in ProjectStrings)
			{
				if (StrItem.ID == string.Empty) continue;

				FileToChange File;

				int Index = Files.BinarySearch(new FileToChange(StrItem.Filename));
				if (Index < 0)
				{
					File = new FileToChange(StrItem.Filename);
					Files.Add(File);
					Files.Sort();
				}
				else File = Files[Index];

				File.Items.Add(StrItem);
			}

			AddLog("Modifying files...");
			int CurrentFile = 0;
			foreach(FileToChange File in Files)
			{
				CurrentFile++;
				ReportProgress(CurrentFile, Files.Count, File.Filename);
				if (!ProcessFile(File)) Ret = false;
			}

			AddLog("Adding new items to string table...");
			ReportProgress(0, 0, "");
			if (!ProcessStringTable()) Ret = false;


			if(Ret) AddLog("Done.");
			else AddLog("Done with errors.");
			ReportProgress(0, 0, "");

			return Ret;
		}

		//////////////////////////////////////////////////////////////////////////
		private bool ProcessStringTable()
		{
			try
			{
				if (this.BackupOldFiles) BackupFile(StringTableFile);

				using (StreamWriter sw = new StreamWriter(StringTableFile, true, StringTableUsingUtf8?Encoding.UTF8:Encoding.Default))
				{
					foreach (StringItem StrItem in ProjectStrings)
					{
						if (!StrItem.SavedOk) continue;
						if (StrItem.ID == string.Empty) continue;
						sw.WriteLine(StrItem.ID + "\t" + StrItem.Value);
					}
				}
				return true;
			}
			catch(Exception e)
			{
				AddLog("Error modifying string table '" + StringTableFile + "':");
				AddLog(e.Message);
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private bool ProcessFile(FileToChange File)
		{
			try
			{
				if (this.BackupOldFiles) BackupFile(File.Filename);

				Encoding FileEncoding = Encoding.Default;
				List<string> Lines = new List<string>();

				// read original file
				using (StreamReader sr = new StreamReader(File.Filename, Encoding.Default, true))
				{
					string ReadLine;
					int LineNum = 0;
					while ((ReadLine = sr.ReadLine()) != null)
					{
						LineNum++;
						Lines.Add(ProcessLine(LineNum, ReadLine, File));
					}
					FileEncoding = sr.CurrentEncoding;
				}

				// save new file
				using (StreamWriter sw = new StreamWriter(File.Filename, false, FileEncoding))
				{
					foreach(string Line in Lines)
					{
						sw.WriteLine(Line);
					}
				}

				// mark items as saved ok
				foreach(StringItem Str in File.Items)
				{
					Str.SavedOk = true;
				}

				return true;
			}
			catch(Exception e)
			{
				AddLog("Error modifying file '" + File.Filename + "':");
				AddLog(e.Message);

				// mark items as NOT saved ok
				foreach (StringItem Str in File.Items)
				{
					Str.SavedOk = false;
				}

				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private string ProcessLine(int LineNum, string Line, FileToChange File)
		{
			string OutLine = Line;
			foreach(StringItem StrItem in File.Items)
			{
				if(StrItem.Line == LineNum)
				{
					string OldString = "\"" + StrItem.OriginalString + "\"";

					string NewString = StrItem.Value;
					NewString = NewString.Replace("|", "~n");
					NewString = NewString.Replace("\"", "~\"");
					NewString = "\"/" + StrItem.ID + "/" + NewString + "\"";

					if(OutLine.IndexOf(OldString) < 0)
					{
						AddLog("Cannot find original string " + OldString);
						AddLog("(file: " + File.Filename + ", line: " + LineNum.ToString() + ")");
					}
					else OutLine = OutLine.Replace(OldString, NewString);
				}
			}
			return OutLine;
		}

		//////////////////////////////////////////////////////////////////////////
		private void BackupFile(string Filename)
		{
			if (!File.Exists(Filename)) return;

			string BackupFilename;
			int Counter = 0;
			do
			{
				BackupFilename = Filename + ".bak" + (Counter == 0 ? "" : Counter.ToString());
				Counter++;
			}
			while (File.Exists(BackupFilename));

			File.Copy(Filename, BackupFilename);
		}


		#region Settings loading/saving
		//////////////////////////////////////////////////////////////////////////
		public void SaveSettings(SettingsNode RootNode)
		{
			SettingsNode Section = RootNode.GetNode("StringTableMgr", true, true);
			if (Section == null) return;

			Section.Clear();

			Section.SetValue("ProjectFile", ProjectFile);
			Section.SetValue("StringTableFile", StringTableFile);
			Section.SetValue("BackupOldFiles", BackupOldFiles);

			SettingsNode IgnoreItems = new SettingsNode("IgnoreList");
			foreach (string Item in IgnoreList)
			{
				IgnoreItems.Children.Add(new SettingsNode("Item", Item));
			}
			Section.Children.Add(IgnoreItems);
		}

		//////////////////////////////////////////////////////////////////////////
		public void LoadSettings(SettingsNode RootNode)
		{
			SettingsNode Section = RootNode.GetNode("StringTableMgr", true);
			if (Section == null) return;

			ProjectFile = Section.GetString("ProjectFile");
			StringTableFile = Section.GetString("StringTableFile");
			BackupOldFiles = Section.GetBool("BackupOldFiles");

			IgnoreList.Clear();
			SettingsNode IgnoreItems = Section.GetNode("IgnoreList");
			if (IgnoreItems != null)
			{
				foreach (SettingsNode Node in IgnoreItems.Children)
				{
					IgnoreList.Add(Node.GetString());
				}
			}
		}
		#endregion

	}
}
