using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.IO;
using DeadCode.WME.DefinitionFileParser;
using DeadCode.WME.ScriptParser;

namespace DeadCode.WME.StringTableMgr
{
	public class StringLocation
	{
		public string Value;
		public string Filename;
		public int Line;
		public StringType Type = StringType.Unknown;

		public enum StringType
		{
			DefinitionFile, ScriptFile, Unknown
		};

		public StringLocation(string Value, string Filename, int Line, StringType Type)
		{
			this.Value = Value;
			this.Filename = Filename;
			this.Line = Line;
			this.Type = Type;
		}
	};


	public class StringExtractor
	{
		private string[] ScriptMasks;
		private string[] DefMasks;

		public List<StringLocation> Strings;

		private int NumFiles = 0;
		private StringTableMgr StringTableMgr;

		//////////////////////////////////////////////////////////////////////////
		public StringExtractor(StringTableMgr StringTableMgr)
		{
			this.StringTableMgr = StringTableMgr;
		}

		//////////////////////////////////////////////////////////////////////////
		public bool ExtractProjectStrings(string ProjectFilename, string ScriptMasks, string DefMasks)
		{
			this.ScriptMasks = ScriptMasks.Split(new char[] { ';' });
			this.DefMasks = DefMasks.Split(new char[] { ';' });

			Strings = new List<StringLocation>();

			WmeProject Project = new WmeProject(ProjectFilename);
			if (Project.Packages.Length == 0)
			{
				AddLog(LogSeverity.Warning, "No packages found in project");
				return true;
			}

			// count files			
			NumFiles = 0;
			ReportProgress(0, "Scanning project...");
			foreach (ProjectPackage Package in Project.Packages)
			{
				CountFiles(Package.FullPath, ref NumFiles);
			}


			// scan files
			int CurrentFile = 0;
			foreach(ProjectPackage Package in Project.Packages)
			{
				ScanDirectory(Package.FullPath, ref CurrentFile);
			}
			ReportProgress(0, "Done");

			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		private void CountFiles(string Dir, ref int NumFiles)
		{
			for (int i = 0; i < ScriptMasks.Length; i++)
			{
				string[] ScriptFiles = Directory.GetFiles(Dir, ScriptMasks[i]);
				NumFiles += ScriptFiles.Length;
			}

			for (int i = 0; i < DefMasks.Length; i++)
			{
				string[] DefFiles = Directory.GetFiles(Dir, DefMasks[i]);
				NumFiles += DefFiles.Length;
			}

			string[] Subdirs = Directory.GetDirectories(Dir);
			for (int i = 0; i < Subdirs.Length; i++)
			{
				CountFiles(Subdirs[i], ref NumFiles);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		bool ScanDirectory(string Dir, ref int CurrentFile)
		{
			// scan scripts
			for(int i=0; i<ScriptMasks.Length; i++)
			{
				string[] ScriptFiles = Directory.GetFiles(Dir, ScriptMasks[i]);
				for (int j = 0; j < ScriptFiles.Length; j++)
				{
					ReportProgress(++CurrentFile, ScriptFiles[j]);
					ScanScript(ScriptFiles[j]);
				}
			}

			// scan definition files
			for (int i = 0; i < DefMasks.Length; i++)
			{
				string[] DefFiles = Directory.GetFiles(Dir, DefMasks[i]);
				for (int j = 0; j < DefFiles.Length; j++)
				{
					ReportProgress(++CurrentFile, DefFiles[j]);
					ScanDefinition(DefFiles[j]);
				}
			}

			// go on with subdirs
			string[] Subdirs = Directory.GetDirectories(Dir);
			for (int i = 0; i < Subdirs.Length; i++)
			{
				ScanDirectory(Subdirs[i], ref CurrentFile);
			}

			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		private void ReportProgress(int CurrentFile, string Filename)
		{
			if (StringTableMgr != null)
				StringTableMgr.ReportProgress(CurrentFile, NumFiles, Filename);
		}

		//////////////////////////////////////////////////////////////////////////
		private void ScanDefinition(string DefFilename)
		{
			DefinitionFile DefFile = new DefinitionFile();
			if(!DefFile.ParseFile(DefFilename))
			{
				AddLog(LogSeverity.Error, "Error parsing file '" + DefFilename + "'");
				return;
			}
			ScanDefinition(DefFilename, DefFile);
		}

		//////////////////////////////////////////////////////////////////////////
		private void ScanDefinition(string DefFilename, DefinitionFileItem DefFileItem)
		{
			if (DefFileItem.Children.Count > 0)
			{
				for (int i = 0; i < DefFileItem.Children.Count; i++)
				{
					ScanDefinition(DefFilename, DefFileItem.Children[i]);
				}
			}
			else if (DefFileItem.Value!=string.Empty)
			{
				string[] ItemsToScan = new string[] { "CAPTION", "TITLE", "TEXT" };
				foreach(string Item in ItemsToScan)
				{
					if(DefFileItem.Name.ToUpper() == Item)
					{
						Strings.Add(new StringLocation(DefFileItem.Value, DefFilename, DefFileItem.LineNum, StringLocation.StringType.DefinitionFile));
					}
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void ScanScript(string ScriptFile)
		{
			try
			{
				FileStream fs = new FileStream(ScriptFile, FileMode.Open, FileAccess.Read, FileShare.Read);
				ScriptTokenizer Tokenizer = new ScriptTokenizer(fs, false);
				Tokenizer.GetTokens();

				for (int i = 0; i < Tokenizer.Tokens.Count; i++)
				{
					ScriptToken PrevToken = i > 0 ? Tokenizer.Tokens[i - 1] : null;
					ScriptToken Token = Tokenizer.Tokens[i];

					// some quick checks
					if (!Token.IsString) continue;
					if (PrevToken != null)
					{
						if ((PrevToken.IsKeyword && PrevToken.Value == "on") ||
							(PrevToken.IsKeyword && PrevToken.Value == "extern") ||
							(PrevToken.IsIdentifier && PrevToken.Value == "#include"))
						{
							continue;
						}
					}

					string Str = Token.Value.Substring(1, Token.Value.Length - 2);
					if (Str == string.Empty) continue;

					Strings.Add(new StringLocation(Str, ScriptFile, Token.StartLine, StringLocation.StringType.ScriptFile));
				}
			}
			catch
			{
				AddLog(LogSeverity.Error, "Error parsing script '" + ScriptFile + "'");
			}
		}


		//////////////////////////////////////////////////////////////////////////
		private void AddLog(LogSeverity Severity, string Message)
		{
			if (StringTableMgr != null) StringTableMgr.AddLog(Severity, Message, true);
		}


	}
}
