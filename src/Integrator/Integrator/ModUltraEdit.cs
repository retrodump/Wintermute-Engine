using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using DeadCode.WME.Global;
using Microsoft.Win32;
using System.IO;
using System.Text.RegularExpressions;
using DeadCode.WME.DocMaker;
using DeadCode.WME.ScriptParser;

namespace Integrator
{
	public partial class ModUltraEdit : IntegratorModule
	{
		protected virtual string ExeName
		{
			get
			{
				return "uedit32.exe";
			}
		}

		protected virtual string IniEnvVar
		{
			get
			{
				return "UEINIDIR";
			}
		}

		protected virtual string IniName
		{
			get
			{
				return "uedit32.ini";
			}
		}

		protected virtual string AppDataPath
		{
			get
			{
				return @"IDMComp\UltraEdit\";
			}
		}

		
		//////////////////////////////////////////////////////////////////////////
		public ModUltraEdit()
		{
			InitializeComponent();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnDownloadLink(object sender, LinkLabelLinkClickedEventArgs e)
		{
			System.Diagnostics.Process.Start("http://www.ultraedit.com");
		}


		//////////////////////////////////////////////////////////////////////////
		protected void OnAssociate(object sender, EventArgs e)
		{
			string UeExe;
			UeExe = Path.Combine(TxtUeInstallDir.Text, ExeName);
			if (!File.Exists(UeExe))
			{
				MessageBox.Show("The '" + ExeName + "' file was not found in the specified location. Please specify a valid UltraEdit directory.", ParentForm.Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
				return;
			}
			MakeAssocitations(UeExe);
		}


		//////////////////////////////////////////////////////////////////////////
		protected void OnLoad(object sender, EventArgs e)
		{
			// find ultraedit
			string UePath = "";
			try
			{
				using (RegistryKey Key = Registry.LocalMachine.OpenSubKey(@"SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\" + ExeName))
				{
					UePath = Key.GetValue("").ToString();
					UePath = Path.GetDirectoryName(UePath);
				};
			}
			catch
			{
				UePath = "";
			}
			TxtUeInstallDir.Text = UePath;
			LoadInfo();
		}

		protected string UeIniPath = null;
		protected string UeWordFilePath = null;
		//////////////////////////////////////////////////////////////////////////
		protected void LoadInfo()
		{
			int LanguageIndex = ListLanguages.SelectedIndex;
			int ToolIndex = ListTools.SelectedIndex;

			ListLanguages.Items.Clear();
			ListTools.Items.Clear();

			if (Directory.Exists(TxtUeInstallDir.Text))
			{
				try
				{
					// try to find uedit32.ini										

					// first check the environment variable
					UeIniPath = Environment.GetEnvironmentVariable(IniEnvVar);
					if (UeIniPath != null)
					{
						UeIniPath = Path.Combine(UeIniPath, IniName);
						if (!File.Exists(UeIniPath)) UeIniPath = null;
					}

					// next check the app dir
					if (UeIniPath == null)
					{
						UeIniPath = Path.Combine(TxtUeInstallDir.Text, IniName);
						if (!File.Exists(UeIniPath)) UeIniPath = null;
					}

					// next look in windows dir
					if (UeIniPath == null)
					{
						UeIniPath = Environment.GetEnvironmentVariable("WINDIR");
						if (UeIniPath != null)
						{
							UeIniPath = Path.Combine(UeIniPath, IniName);
							if (!File.Exists(UeIniPath)) UeIniPath = null;
						}
					}

					// finally look in %appdata%					
					if (UeIniPath == null)
					{
						UeIniPath = Environment.GetEnvironmentVariable("APPDATA");
						if (UeIniPath != null)
						{
							UeIniPath = Path.Combine(UeIniPath, AppDataPath + IniName);
							if (!File.Exists(UeIniPath)) UeIniPath = null;
						}
					}

					if (UeIniPath != null) LoadInfoFromIni(UeIniPath);
				}
				catch
				{
					UeIniPath = null;
				}
			}

			if (LanguageIndex >= 0 && LanguageIndex < ListLanguages.Items.Count)
				ListLanguages.SelectedIndex = LanguageIndex;

			if (ToolIndex >= 0 && ToolIndex < ListTools.Items.Count)
				ListTools.SelectedIndex = ToolIndex;

			SetState();
		}

		//////////////////////////////////////////////////////////////////////////
		protected void LoadInfoFromIni(string UeIniPath)
		{
			if (!File.Exists(UeIniPath)) return;

			IniFile Ini = new IniFile(UeIniPath);

			// read word file
			UeWordFilePath = Ini.ReadString("Settings", "Language File");
			LoadWordFile();

			for (int i = 0; i < 20; i++)
			{
				WordFileItem Item = GetWordFileItem(i + 1);
				if (Item == null) ListLanguages.Items.Add("<empty slot>");
				else ListLanguages.Items.Add(Item.Name);
			}

			// read tools
			for (int i = 0; i < 10; i++)
			{
				string Val = Ini.ReadString("Tools", "Tool Menu" + i.ToString(), "");
				if (Val == null || Val == string.Empty)
				{
					Val = "<empty slot>";
					ListTools.Items.Add(Val);
					break;
				}
				else ListTools.Items.Add(Val);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void SetState()
		{
			BtnAddLanguage.Enabled = ListLanguages.SelectedItem != null;
			BtnAddTool.Enabled = ListTools.SelectedItem != null;
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnBrowseDir(object sender, EventArgs e)
		{
			FolderBrowserDialog Dlg = new FolderBrowserDialog();
			Dlg.Description = "Select UltraEdit installation directory.";
			if (Directory.Exists(TxtUeInstallDir.Text)) Dlg.SelectedPath = TxtUeInstallDir.Text;

			if (Dlg.ShowDialog() == DialogResult.OK)
			{
				TxtUeInstallDir.Text = Dlg.SelectedPath;
				LoadInfo();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnDirLeave(object sender, EventArgs e)
		{
			LoadInfo();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnSelectionChanged(object sender, EventArgs e)
		{
			SetState();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnAddTool(object sender, EventArgs e)
		{
			if (ListTools.SelectedIndex < 0 || ListTools.SelectedIndex > 9) return;
			if (!File.Exists(UeIniPath)) return;

			if (DialogResult.Yes != MessageBox.Show("Do you really want to replace '" +
				ListTools.SelectedItem.ToString() + "' with WME check syntax command?", ParentForm.Text,
				MessageBoxButtons.YesNo, MessageBoxIcon.Question)) return;

			BackupFile(UeIniPath);

			
			string Caption = "WME Check Syntax";
			if(File.Exists(ParentForm.TxtProjectFile.Text)) 
				Caption += " (" + Path.GetFileNameWithoutExtension(ParentForm.TxtProjectFile.Text) + ")";
			string Command = "\"" + WmeUtils.CompilerPath + "\" -script \"%F\"";
			if(File.Exists(ParentForm.TxtProjectFile.Text))
				Command += " -project \"" +	ParentForm.TxtProjectFile.Text + "\"";

			string Suffix = ListTools.SelectedIndex.ToString();

			IniFile Ini = new IniFile(UeIniPath);
			Ini.WriteString("Tools", "Tool Menu" + Suffix, "\"" + Caption + "\"");
			Ini.WriteString("Tools", "Tool Cmd" + Suffix, "\"" + Command + "\"");
			Ini.WriteString("Tools", "Tool Dir" + Suffix, "\"\"");
			Ini.WriteString("Tools", "Capture" + Suffix, "1");
			Ini.WriteString("Tools", "Capture Mode" + Suffix, "3");
			Ini.WriteString("Tools", "WinProg" + Suffix, "0");
			Ini.WriteString("Tools", "SaveAllFiles" + Suffix, "0");
			Ini.WriteString("Tools", "Save Active File" + Suffix, "1");
			Ini.WriteString("Tools", "Show DOS Window" + Suffix, "0");
			Ini.WriteString("Tools", "Replace Type" + Suffix, "0");
			Ini.WriteString("Tools", "Bitmap Path" + Suffix, "\"\"");

			LoadInfo();
		}

		//////////////////////////////////////////////////////////////////////////
		class WordFileItem
		{
			public string Name;
			public int Order;
			
			private List<string> lines;
			public List<string> Lines
			{
				get
				{
					if (lines == null) lines = new List<string>();
					return lines;
				}
			}
		};

		List<WordFileItem> WordFileItems;
		//////////////////////////////////////////////////////////////////////////
		private void LoadWordFile()
		{
			WordFileItems = new List<WordFileItem>();

			if (!File.Exists(UeWordFilePath)) return;
			
			WordFileItem CurrItem = null;
			using (StreamReader sr = new StreamReader(UeWordFilePath, Encoding.Default, true))
			{
				Regex R = new Regex("/L[0-9]+\".*\".*");
				string Line;
				while ((Line = sr.ReadLine()) != null)
				{
					if(R.IsMatch(Line))
					{
						if (CurrItem != null) WordFileItems.Add(CurrItem);

						CurrItem = new WordFileItem();
						CurrItem.Lines.Add(Line);

						ScanLanguageLine(Line, out CurrItem.Order, out CurrItem.Name);
					}
					else
					{
						if (CurrItem != null) CurrItem.Lines.Add(Line);
					}
				}
				if (CurrItem != null) WordFileItems.Add(CurrItem);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void ScanLanguageLine(string line, out int order, out string name)
		{
			int Index1 = line.IndexOf('\"');
			int Index2 = line.IndexOf('\"', Index1+1);

			name = line.Substring(Index1 + 1, Index2 - Index1 - 1);

			string OrderStr = line.Substring(2, Index1 - 2);
			order = int.Parse(OrderStr);
		}

		//////////////////////////////////////////////////////////////////////////
		private WordFileItem GetWordFileItem(int Index)
		{
			foreach(WordFileItem Item in WordFileItems)
			{
				if (Item.Order == Index) return Item;
			}
			return null;
		}

		//////////////////////////////////////////////////////////////////////////
		private void SaveWordFile()
		{
			try
			{
				BackupFile(UeWordFilePath);

				using (StreamWriter sw = new StreamWriter(UeWordFilePath, false, Encoding.Default))
				{
					foreach (WordFileItem Item in WordFileItems)
					{
						foreach(string Line in Item.Lines)
						{
							sw.WriteLine(Line);
						}
					}
				}
			}
			catch(Exception e)
			{
				MessageBox.Show("Error saving changes.\n\n" + e.Message, ParentForm.Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
			}

		}

		//////////////////////////////////////////////////////////////////////////
		private void OnAddLanguage(object sender, EventArgs e)
		{
			if (ListLanguages.SelectedIndex < 0 || ListLanguages.SelectedIndex >= 20) return;
			if (!File.Exists(UeWordFilePath)) return;

			if (DialogResult.Yes != MessageBox.Show("Do you really want to replace '" +
				ListLanguages.SelectedItem.ToString() + "' with WME scripts?", ParentForm.Text,
				MessageBoxButtons.YesNo, MessageBoxIcon.Question)) return;

			WordFileItem Item = GetWordFileItem(ListLanguages.SelectedIndex + 1);
			if(Item==null)
			{
				Item = new WordFileItem();
				WordFileItems.Add(Item);
			}
			FillWordFileEntry(ListLanguages.SelectedIndex + 1, Item);

			SaveWordFile();
			LoadInfo();
		}

		//////////////////////////////////////////////////////////////////////////
		private void FillWordFileEntry(int Order, WordFileItem Item)
		{
			Item.Order = Order;
			Item.Name = "WME Script";
			Item.Lines.Clear();

			// language definition
			string LangLine = "";
			LangLine += "/L" + Order.ToString() + "\"WME Script\"";
			LangLine += " Line Comment = //";
			LangLine += " Block Comment On = /* Block Comment Off = */ NestBlockComments";
			LangLine += " Line Comment = //";
			LangLine += " Escape Char = ~ String Chars = \"";

			string[] Extensions = ParentForm.GetExtensions();
			LangLine += " File Extensions =";
			foreach(string Ext in Extensions)
			{
				LangLine += " " + Ext;
			}
			Item.Lines.Add(LangLine);

			Item.Lines.Add("/Delimiters = ~!@%^&*()-+=|\\/{}[]:;\"'<> ,	.?");
			Item.Lines.Add("/Function String = \"%[ ^t]++function^(*(*)^)\"");
			Item.Lines.Add("/Function String 1 = \"%[ ^t]++method^(*(*)^)\"");
			Item.Lines.Add("/Indent Strings = \"{\" \"else\"");
			Item.Lines.Add("/Unindent Strings = \"}\"");
			Item.Lines.Add("/Open Brace Strings =  \"{\" \"(\" \"[\" \"<\"");
			Item.Lines.Add("/Close Brace Strings = \"}\" \")\" \"]\" \">\"");
			Item.Lines.Add("/Open Fold Strings = \"{\" \"#region\"");
			Item.Lines.Add("/Close Fold Strings = \"}\" \"#endregion\"");

			WordHolder wh = new WordHolder();

			// keywords
			Item.Lines.Add("/C1\"Keywords\" STYLE_KEYWORD");
			foreach(string Keyword in ScriptTokenizer.Keywords)
			{
				wh.AddWord(Keyword);
			}
			Item.Lines.Add(wh.GetWords());


			// read XML docs
			ScriptInfo Info = new ScriptInfo();
			Info.ReadXml(WmeUtils.XmlDocsPath);

			// methods
			Item.Lines.Add("/C2\"Methods\" STYLE_METHOD");
			wh = new WordHolder();
			foreach(ScriptObject Obj in Info.Objects)
			{
				foreach(ScriptMethod Method in Obj.Methods)
				{
					//if (Method.IsGlobal) continue;
					foreach(string Header in Method.Headers)
					{
						int Brace = Header.IndexOf("(");
						if(Brace >= 0)
						{
							wh.AddWord(Header.Substring(0, Brace).Trim());
						}						
					}					
				}
			}
			Item.Lines.Add(wh.GetWords());

			// attributes
			Item.Lines.Add("/C3\"Properties\" STYLE_ATTRIBUTE");
			wh = new WordHolder();
			foreach (ScriptObject Obj in Info.Objects)
			{
				foreach (ScriptAttribute Attr in Obj.Attributes)
				{
					if (Attr.Name.StartsWith("[")) continue;
					wh.AddWord(Attr.Name);
				}
			}
			Item.Lines.Add(wh.GetWords());

			// preprocessor
			Item.Lines.Add("/C4\"Preprocessor\" STYLE_EXTENSION");
			Item.Lines.Add("#endregion #include #region");
		}

		//////////////////////////////////////////////////////////////////////////
		private class WordHolder
		{
			Dictionary<string, List<string>> Words = new Dictionary<string, List<string>>();

			//////////////////////////////////////////////////////////////////////////
			public void AddWord(string Word)
			{
				if (Word == null || Word == string.Empty) return;

				string Key = Word.Substring(0, 1).ToUpper();
				if (!Words.ContainsKey(Key)) Words[Key] = new List<string>();
				if (!Words[Key].Contains(Word)) Words[Key].Add(Word);

			}

			//////////////////////////////////////////////////////////////////////////
			public string GetWords()
			{
				string Ret = "";
				for(char ch=' '; ch<='Z'; ch++)
				{
					string Key = new string(ch, 1);
					if (!Words.ContainsKey(Key)) continue;

					Words[Key].Sort();
					foreach(string Str in Words[Key])
					{
						Ret += Str + " ";
					}
					Ret += "\n";
				}
				return Ret;
			}
		};

	}
}
