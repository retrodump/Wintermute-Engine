using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.IO;
using Microsoft.Win32;
using DeadCode.WME.Global;
using DeadCode.WME.ScriptParser;
using DeadCode.WME.DocMaker;

namespace Integrator
{
	public partial class ModPsPad : IntegratorModule
	{
		//////////////////////////////////////////////////////////////////////////
		public ModPsPad()
		{
			InitializeComponent();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnDownloadLink(object sender, EventArgs e)
		{
			System.Diagnostics.Process.Start("http://www.pspad.com/");
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnAssociate(object sender, EventArgs e)
		{
			string Exe;
			Exe = Path.Combine(TxtPsPadInstallDir.Text, "PSPad.exe");
			if (!File.Exists(Exe))
			{
				MessageBox.Show("The 'PSPad.exe' file was not found in the specified location. Please specify a valid PSPad directory.", ParentForm.Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
				return;
			}
			MakeAssocitations(Exe);
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnBrowseDir(object sender, EventArgs e)
		{
			FolderBrowserDialog Dlg = new FolderBrowserDialog();
			Dlg.Description = "Select PSPad installation directory.";
			if (Directory.Exists(TxtPsPadInstallDir.Text)) Dlg.SelectedPath = TxtPsPadInstallDir.Text;

			if (Dlg.ShowDialog() == DialogResult.OK)
			{
				TxtPsPadInstallDir.Text = Dlg.SelectedPath;
				LoadInfo();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnLoad(object sender, EventArgs e)
		{
			// find PSPad
			string PsPadPath = "";
			try
			{
				using (RegistryKey Key = Registry.CurrentUser.OpenSubKey(@"Software\PSPad"))
				{
					PsPadPath = Key.GetValue("PSPadPath").ToString();
					PsPadPath = Path.GetDirectoryName(PsPadPath);
				};
			}
			catch
			{
				PsPadPath = "";
			}
			TxtPsPadInstallDir.Text = PsPadPath;
			LoadInfo();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnDirLeave(object sender, EventArgs e)
		{
			LoadInfo();
		}

		string PsPadIni;
		//////////////////////////////////////////////////////////////////////////
		private void LoadInfo()
		{
			int LanguageIndex = ListLanguages.SelectedIndex;

			ListLanguages.Items.Clear();			

			if (Directory.Exists(TxtPsPadInstallDir.Text))
			{
				PsPadIni = Path.Combine(TxtPsPadInstallDir.Text, "PSPad.ini");
				if (File.Exists(PsPadIni))
				{
					try
					{
						IniFile Ini = new IniFile(PsPadIni);
						for (int i = 0; i <= 4; i++)
						{
							string Key = "UserHighLighterName";
							if (i > 0) Key += i.ToString();

							string Val = Ini.ReadString("Config", Key, "");
							if (Val == null || Val == string.Empty) Val = "<empty slot>";

							ListLanguages.Items.Add(Val);
						}
					}
					catch
					{
					}
				}
			}
			if (LanguageIndex >= 0 && LanguageIndex < ListLanguages.Items.Count)
				ListLanguages.SelectedIndex = LanguageIndex;

			SetState();
		}

		//////////////////////////////////////////////////////////////////////////
		private void SetState()
		{
			BtnAddLanguage.Enabled = ListLanguages.SelectedItem != null;
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnSelectionChanged(object sender, EventArgs e)
		{
			SetState();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnAddLanguage(object sender, EventArgs e)
		{
			if (!File.Exists(PsPadIni)) return;

			//File.Exists(ParentForm.TxtProjectFile.Text

			while(NativeMethods.FindWindowEx(IntPtr.Zero, IntPtr.Zero, "TfPSPad", null)!=IntPtr.Zero)
			{
				DialogResult Res = MessageBox.Show("Please close PSPad first.", ParentForm.Text, MessageBoxButtons.AbortRetryIgnore, MessageBoxIcon.Error);
				if (Res == DialogResult.Abort) return;
				else if (Res == DialogResult.Ignore) break;
			}

			if (DialogResult.Yes != MessageBox.Show("Do you really want to replace '" +
				ListLanguages.SelectedItem.ToString() + "' with WME scripts?", ParentForm.Text,
				MessageBoxButtons.YesNo, MessageBoxIcon.Question)) return;

			IniFile Ini = new IniFile(PsPadIni);
			string Suffix = "";
			if (ListLanguages.SelectedIndex > 0) Suffix = ListLanguages.SelectedIndex.ToString();
			
			Ini.WriteString("Config", "UserHighLighterName" + Suffix, "WME Script");
			Ini.WriteString("Highlighters", "General" + Suffix, "1");

			string[] Extensions = ParentForm.GetExtensions();
			string Extensions2 = "";
			foreach(string Ext in Extensions)
			{
				if (Extensions2 != "") Extensions2 += ";";
				Extensions2 += "*." + Ext; 
			}

			// language section
			Ini.WriteString("WME Script", "Filter", "WME Script (" + Extensions2 + ")|" + Extensions2);
			Ini.WriteString("WME Script", "HLTabWidth", "4");
			Ini.WriteString("WME Script", "IndentChar", "{");
			Ini.WriteString("WME Script", "UnIndentChar", "}");
			Ini.WriteString("WME Script", "Comment", "000080001FFFFFFF010");
			Ini.WriteString("WME Script", "Identifier", "1FFFFFFF1FFFFFFF000");
			Ini.WriteString("WME Script", "Key", "00FF00001FFFFFFF100");
			Ini.WriteString("WME Script", "Key words 2", "000000801FFFFFFF100");
			Ini.WriteString("WME Script", "Key words 3", "008080001FFFFFFF100");
			Ini.WriteString("WME Script", "Label", "000000FF1FFFFFFF000");
			Ini.WriteString("WME Script", "Number", "000000FF1FFFFFFF000");
			Ini.WriteString("WME Script", "Preprocessor", "008000801FFFFFFF010");
			Ini.WriteString("WME Script", "Reserved Word", "000000801FFFFFFF100");
			Ini.WriteString("WME Script", "Space", "008080001FFFFFFF000");
			Ini.WriteString("WME Script", "String", "008080801FFFFFFF000");
			Ini.WriteString("WME Script", "Symbol", "1FFFFFFF1FFFFFFF000");

			// compiler settings
			Ini.WriteString("WME Script", "Compilator File", WmeUtils.CompilerPath);

			string CompParams = "-script |%File%|";
			if (File.Exists(ParentForm.TxtProjectFile.Text))
				CompParams += " -project |" + ParentForm.TxtProjectFile.Text + "|";
			Ini.WriteString("WME Script", "Compilator Param", CompParams);

			Ini.WriteString("WME Script", "Compilator LOG", "");
			Ini.WriteString("WME Script", "Compilator Run", "");
			Ini.WriteString("WME Script", "Compilator Help", Path.Combine(WmeUtils.ToolsPath, "wme.chm"));
			Ini.WriteString("WME Script", "Compilator SaveAll", "0");
			Ini.WriteString("WME Script", "Compilator ParsLog", "%F,%L:%E");
			Ini.WriteString("WME Script", "Compilator Capture", "1");
			Ini.WriteString("WME Script", "Compilator HideOutput", "1");
			Ini.WriteString("WME Script", "Compilator DefaultDir", "");

			// generate syntax file
			try
			{
				string SyntaxFilePath = Path.Combine(TxtPsPadInstallDir.Text, "Syntax");
				if (!Directory.Exists(SyntaxFilePath)) Directory.CreateDirectory(SyntaxFilePath);
				SyntaxFilePath = Path.Combine(SyntaxFilePath, "WME Script.ini");

				using (StreamWriter sw = new StreamWriter(SyntaxFilePath, false, Encoding.Default))
				{
					sw.WriteLine("; PSPad HighLighter definition file for WME Scripts");
					sw.WriteLine("; Generated automatically by WME Integrator");
					sw.WriteLine("; Do not modify this file");

					sw.WriteLine("[Settings]");
					sw.WriteLine("Name=WME Script");
					sw.WriteLine("HTMLGroup=0");
					sw.WriteLine("Label=1");
					sw.WriteLine("FileType=" + Extensions2.Replace(";", ","));
					sw.WriteLine("CommentString=");
					sw.WriteLine("SlashComment=1");
					sw.WriteLine("CComment=1");
					sw.WriteLine("SlashComment=1");
					sw.WriteLine("Preprocessors=1");
					sw.WriteLine("IndentChar={");
					sw.WriteLine("UnIndentChar=}");
					sw.WriteLine("TabWidth=4");
					sw.WriteLine("CaseSensitive=1");
					sw.WriteLine("DoubleQuote=1");
					sw.WriteLine("KeyWordChars=_");
					sw.WriteLine("CodeExplorer=ftJS");

					WordHolder wh;

					// keywords
					sw.WriteLine("[KeyWords]");
					wh = new WordHolder();
					foreach (string Keyword in ScriptTokenizer.Keywords)
					{
						wh.AddWord(Keyword);
					}
					sw.WriteLine(wh.GetWords());

					// read XML docs
					ScriptInfo Info = new ScriptInfo();
					Info.ReadXml(WmeUtils.XmlDocsPath);


					// methods
					sw.WriteLine("[KeyWords2]");
					wh = new WordHolder();
					foreach (ScriptObject Obj in Info.Objects)
					{
						foreach (ScriptMethod Method in Obj.Methods)
						{
							foreach (string Header in Method.Headers)
							{
								int Brace = Header.IndexOf("(");
								if (Brace >= 0)
								{
									wh.AddWord(Header.Substring(0, Brace).Trim());
								}
							}
						}
					}
					sw.WriteLine(wh.GetWords());


					// attributes
					sw.WriteLine("[KeyWords3]");
					wh = new WordHolder();
					foreach (ScriptObject Obj in Info.Objects)
					{
						foreach (ScriptAttribute Attr in Obj.Attributes)
						{
							if (Attr.Name.StartsWith("[")) continue;
							wh.AddWord(Attr.Name);
						}
					}
					sw.WriteLine(wh.GetWords());
				}
			}
			catch
			{
			}

			LoadInfo();
		}


		//////////////////////////////////////////////////////////////////////////
		private class WordHolder
		{
			List<string> Words = new List<string>();

			//////////////////////////////////////////////////////////////////////////
			public void AddWord(string Word)
			{
				if (Word == null || Word == string.Empty) return;
				if (!Words.Contains(Word)) Words.Add(Word);
			}

			//////////////////////////////////////////////////////////////////////////
			public string GetWords()
			{
				string Ret = "";
				Words.Sort();
				foreach(string Word in Words)
				{
					Ret = Ret + Word + "=" + "\n";
				}
				return Ret;
			}
		};

	}
}
