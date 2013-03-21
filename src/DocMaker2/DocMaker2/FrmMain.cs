using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.IO;
using Microsoft.Win32;
using DeadCode.WME.DocMaker;

namespace DocMaker2
{
	public partial class FrmMain : Form
	{
		public FrmMain()
		{
			InitializeComponent();
			WinCaption = this.Text;
		}

		private string WinCaption = "";
		//////////////////////////////////////////////////////////////////////////
		private void OnFormLoad(object sender, EventArgs e)
		{
			CmbInputCodepage.Items.Clear();
			CmbHtmlOutputCodepage.Items.Clear();

			EncodingInfo[] Encodings = Encoding.GetEncodings();
			foreach (EncodingInfo EI in Encodings)
			{
				CmbInputCodepage.Items.Add(EI.DisplayName);
				if (EI.GetEncoding() == Encoding.Default) CmbInputCodepage.SelectedItem = EI.DisplayName;

				CmbHtmlOutputCodepage.Items.Add(EI.DisplayName);
				if (EI.GetEncoding() == Encoding.Default) CmbHtmlOutputCodepage.SelectedItem = EI.DisplayName;
			}
			IsDirty = false;

			RegistryKey Key = Registry.CurrentUser.OpenSubKey(@"Software\DEAD:CODE\DocMaker2");
			if (Key != null)
			{
				string LastFile = Key.GetValue("LastFile").ToString();
				Key.Close();

				if (File.Exists(LastFile)) LoadSettings(LastFile);
			}

		}

		//////////////////////////////////////////////////////////////////////////
		private static Encoding GetEncodingByName(string EncName)
		{
			EncodingInfo[] Encodings = Encoding.GetEncodings();
			foreach (EncodingInfo EI in Encodings)
			{
				if (EI.DisplayName == EncName) return EI.GetEncoding();
			}
			return Encoding.Default;
			
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnBrowseInputPath(object sender, EventArgs e)
		{
			FolderBrowserDialog dlg = new FolderBrowserDialog();
			dlg.Description = "Select input folder";
			dlg.SelectedPath = TxtInputPath.Text;

			if(dlg.ShowDialog()==DialogResult.OK)
			{
				TxtInputPath.Text = dlg.SelectedPath;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnBrowseHtmlOutput(object sender, EventArgs e)
		{
			FolderBrowserDialog dlg = new FolderBrowserDialog();
			dlg.Description = "Select output folder";
			dlg.SelectedPath = TxtHtmlOutputPath.Text;

			if (dlg.ShowDialog() == DialogResult.OK)
			{
				TxtHtmlOutputPath.Text = dlg.SelectedPath;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnBrowseXmlFile(object sender, EventArgs e)
		{
			SaveFileDialog dlg = new SaveFileDialog();
			dlg.FileName = TxtOutputXmlFile.Text;
			dlg.Filter = "XML files (*.xml)|*.xml|All files (*.*)|*.*";
			dlg.FilterIndex = 1;
			dlg.RestoreDirectory = true;

			if(dlg.ShowDialog()==DialogResult.OK)
			{
				TxtOutputXmlFile.Text = dlg.FileName;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private bool SaveSettings(string Filename)
		{
			try
			{
				XmlDocument Doc = new XmlDocument();

				// header
				XmlDeclaration Decl = Doc.CreateXmlDeclaration("1.0", "utf-8", null);

				// root
				XmlElement RootNode = Doc.CreateElement("DocMakerSettings");
				Doc.AppendChild(RootNode);

				if (SaveControls(RootNode, this))
				{
					XmlTextWriter Writer = new XmlTextWriter(Filename, null);
					Writer.Formatting = Formatting.Indented;
					Doc.Save(Writer);
					Writer.Close();

					CurrentFilename = Filename;
					IsDirty = false;

					return true;
				}
				else return false;
			}
			catch(Exception e)
			{
				MessageBox.Show("Error saving settings to file '" + Filename + "'.\n\n" + e.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private bool SaveControls(XmlNode RootNode, Control ParentControl)
		{
			if (ParentControl == null) return false;

			foreach (Control C in ParentControl.Controls)
			{
				if (C.Controls.Count>0)
				{
					SaveControls(RootNode, C);
				}
				else if ((C is TextBox && !((TextBox)C).ReadOnly) || C is ComboBox)
				{
					XmlElement Node = RootNode.OwnerDocument.CreateElement(C.Name);
					RootNode.AppendChild(Node);
					Node.AppendChild(RootNode.OwnerDocument.CreateTextNode(C.Text));
				}
			}
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		private bool LoadSettings(string Filename)
		{
			try
			{
				XmlDocument Doc = new XmlDocument();
				Doc.Load(Filename);

				XmlElement Root = Doc.DocumentElement;
				if (Root == null || Root.Name != "DocMakerSettings") return false;

				bool Ret = LoadControls(Root, this);
				if(Ret)
				{
					CurrentFilename = Filename;
					IsDirty = false;
				}
				return Ret;
			}
			catch
			{
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private bool LoadControls(XmlElement RootNode, Control ParentControl)
		{
			if (ParentControl == null) return false;

			foreach (Control C in ParentControl.Controls)
			{
				if (C.Controls.Count > 0)
				{
					LoadControls(RootNode, C);
				}
				else if (C is TextBox || C is ComboBox)
				{
					XmlNodeList NodeList = RootNode.GetElementsByTagName(C.Name);
					if(NodeList.Count>0)
					{
						string Value = "";

						XmlNode TextNode = NodeList[0].FirstChild;
						if (TextNode != null) Value = TextNode.Value;

						if (C is TextBox) C.Text = Value;
						else if(C is ComboBox)
						{
							ComboBox Combo = C as ComboBox;
							Encoding Enc = GetEncodingByName(Value);

							int Index = Combo.Items.IndexOf(Enc.EncodingName);
							if (Index >= 0) Combo.SelectedIndex = Index;
						}
					}
				}
			}
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnLoadSettings(object sender, EventArgs e)
		{
			if (IsDirty)
			{
				DialogResult dr = MessageBox.Show("Do you want to save current settings?", "DocMaker", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Warning);
				if (dr == DialogResult.Cancel)
				{
					return;
				}
				else if (dr == DialogResult.Yes)
				{
					OnSaveSettings(sender, new EventArgs());
					if (IsDirty) return;
				}
			}

			
			OpenFileDialog dlg = new OpenFileDialog();
			dlg.FileName = _CurrentFilename;
			dlg.Filter = "XML files (*.xml)|*.xml|All files (*.*)|*.*";
			dlg.FilterIndex = 1;
			dlg.RestoreDirectory = true;

			if (dlg.ShowDialog() == DialogResult.OK)
			{
				if(!LoadSettings(dlg.FileName))
				{
					MessageBox.Show("Error loading settings from file '" + dlg.FileName + "'.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnSaveSettings(object sender, EventArgs e)
		{
			if (_CurrentFilename == "") OnSaveSettingsAs(sender, e);
			else SaveSettings(_CurrentFilename);
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnSaveSettingsAs(object sender, EventArgs e)
		{
			SaveFileDialog dlg = new SaveFileDialog();
			dlg.FileName = _CurrentFilename;
			dlg.Filter = "XML files (*.xml)|*.xml|All files (*.*)|*.*";
			dlg.FilterIndex = 1;
			dlg.RestoreDirectory = true;

			if(dlg.ShowDialog()==DialogResult.OK)
			{
				SaveSettings(dlg.FileName);
			}
		}


		//////////////////////////////////////////////////////////////////////////
		private void OnMakeDirty(object sender, EventArgs e)
		{
			IsDirty = true;
		}

		//////////////////////////////////////////////////////////////////////////
		private bool _IsDirty = false;
		private bool IsDirty
		{
			get
			{
				return _IsDirty;
			}
			set
			{
				_IsDirty = value;
				this.Text = WinCaption + " - " + CurrentFilename + (_IsDirty ? "*" : "");
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private string _CurrentFilename = "";
		private string CurrentFilename
		{
			get
			{
				if (_CurrentFilename == "") return "Untitled";
				else return Path.GetFileNameWithoutExtension(_CurrentFilename);
			}
			set
			{
				_CurrentFilename = value;
				this.Text = WinCaption + " - " + CurrentFilename + (_IsDirty ? "*" : "");
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnFormClosing(object sender, FormClosingEventArgs e)
		{
			if(IsDirty)
			{
				DialogResult dr = MessageBox.Show("Do you want to save current settings?", "DocMaker", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Warning);
				if (dr == DialogResult.Cancel)
				{
					e.Cancel = true;
					return;
				}
				else if (dr == DialogResult.No) return;
				else
				{
					OnSaveSettings(sender, new EventArgs());
					if (IsDirty) e.Cancel = true;
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnFormClosed(object sender, FormClosedEventArgs e)
		{
			if(_CurrentFilename!="" && !IsDirty)
			{
				RegistryKey Key = Registry.CurrentUser.CreateSubKey(@"Software\DEAD:CODE\DocMaker2");
				if(Key!=null)
				{
					Key.SetValue("LastFile", _CurrentFilename);
					Key.Close();
				}

			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnLinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
		{
			System.Diagnostics.Process.Start(LnkWebsite.Text);
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnGenerateXml(object sender, EventArgs e)
		{
			TxtLog.Text = "";

			try
			{
				string Dir = Path.GetDirectoryName(TxtOutputXmlFile.Text);
				Directory.CreateDirectory(Dir);

				ScriptInfo Info = new ScriptInfo();
				Info.SourceEncoding = GetEncodingByName(CmbInputCodepage.Text);

				string[] Extensions = TxtExtensions.Text.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
				foreach (string Ext in Extensions)
				{
					Info.Load(TxtInputPath.Text, "*." + Ext, true);
				}
				XmlDocument Doc = Info.WriteXml();
				XmlTextWriter Writer = new XmlTextWriter(TxtOutputXmlFile.Text, null);
				Writer.Formatting = Formatting.Indented;
				Doc.Save(Writer);
				Writer.Close();
				TxtLog.Text = "Done.";
			}
			catch(Exception ex)
			{
				TxtLog.Text = "Error generating XML file:\r\n\r\n" + ex.Message;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnGenerateHtml(object sender, EventArgs e)
		{
			TxtLog.Text = "";

			try
			{
				Directory.CreateDirectory(TxtHtmlOutputPath.Text);

				ScriptInfo Info = new ScriptInfo();
				Info.SourceEncoding = GetEncodingByName(CmbInputCodepage.Text);

				// parse files
				string[] Extensions = TxtExtensions.Text.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
				foreach (string Ext in Extensions)
				{
					Info.Load(TxtInputPath.Text, "*." + Ext, true);
				}

				// generate html
				foreach(ScriptObject Obj in Info.Objects)
				{
					string Filename = TxtHtmlOutputPath.Text;
					if (!Filename.EndsWith(Path.DirectorySeparatorChar.ToString())) Filename += Path.DirectorySeparatorChar;
					Filename += Obj.BaseFilename + ".html";

					WriteHtml(Filename, Obj);
				}
				
				
				TxtLog.Text = "Done.";
			}
			catch (Exception ex)
			{
				TxtLog.Text = "Error generating HTML files:\r\n\r\n" + ex.Message;
			}
		}
	}
}