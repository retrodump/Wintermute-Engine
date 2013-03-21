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
using DeadCode.WME.Global.Actions;
using System.IO;
using DeadCode.WME.Global.Utils;

namespace DeadCode.WME.WindowEdit
{
	public partial class FrmMain : ActionForm, IActionProvider
	{
		private const string AppName = "WindowEdit";

		// settings
		public Size GridSize = new Size(5, 5);
		public Color SelectionColor = Color.Red;
		public Color BoxColor = Color.FromArgb(100, 0, 0, 255);


		//////////////////////////////////////////////////////////////////////////
		public FrmMain()
		{			
			InitializeComponent();
			AppMgr = new ApplicationMgr("WindowEdit");

			// load available actions
			ActionManager.LoadActions(typeof(FrmMain));
			ActionManager.LoadActions(typeof(WindowDoc));

			// setup image list
			ImageList Img = new ImageList();
			Img.TransparentColor = Color.Magenta;
			Img.ColorDepth = ColorDepth.Depth32Bit;
			Img.Images.Add("window", new Bitmap(typeof(FrmMain), "Graphics.Icons.window.png"));
			Img.Images.Add("button", new Bitmap(typeof(FrmMain), "Graphics.Icons.button.png"));
			Img.Images.Add("editor", new Bitmap(typeof(FrmMain), "Graphics.Icons.editor.png"));
			Img.Images.Add("static", new Bitmap(typeof(FrmMain), "Graphics.Icons.static.png"));
			Img.Images.Add("entity", new Bitmap(typeof(FrmMain), "Graphics.Icons.entity.png"));
			Img.Images.Add("box",    new Bitmap(typeof(FrmMain), "Graphics.Icons.box.png"));
			TreeLayout.ImageList = Img;
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnLoad(object sender, EventArgs e)
		{
			this.MinimumSize = this.Size;

			AppMgr.Settings.LoadFromXmlFile();
			LoadLayout(AppMgr.Settings);
			LoadSettings(AppMgr.Settings);

			// build menus and toolbars
			ActionStripItem MainMenuDef = DefineMainMenu();
			ActContext.StripBuilder.AddToolStrip(MainMenuDef, MainMenu.Items, true);

			ActionStripItem MainToolbarDef = DefineMainToolbar();
			ActContext.StripBuilder.AddToolStrip(MainToolbarDef, MainToolbar.Items, false);

			ActContext.ActivateObject(ActiveObjectSlot.Application, this);

			ActContext.StripBuilder.SetManagedToolStripsState();

			// handle command-line
			if (Environment.GetCommandLineArgs().Length > 1)
				OpenFile(Environment.GetCommandLineArgs()[1]);
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnFormClosed(object sender, FormClosedEventArgs e)
		{
			SaveLayout(AppMgr.Settings);
			SaveSettings(AppMgr.Settings);
			AppMgr.Settings.SaveToXmlFile();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnFormClosing(object sender, FormClosingEventArgs e)
		{
			if(CurrentDoc!=null)
			{
				e.Cancel = !CurrentDoc.CloseDocument();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public void SaveSettings(SettingsNode RootNode)
		{
			SettingsNode Section = RootNode.GetNode("General", true, true);
			if (Section == null) return;

			Section.Clear();

			Section.SetValue("SelectionColor", SelectionColor);
			Section.SetValue("BoxColor", BoxColor);
			Section.SetValue("BackgroundColor", WindowCanvas.BackColor);
			Section.SetValue("GridWidth", GridSize.Width);
			Section.SetValue("GridHeight", GridSize.Height);

			SettingsNode RecentList = new SettingsNode("RecentFiles");
			foreach (string RecentFile in _RecentFiles)
			{
				RecentList.Children.Add(new SettingsNode("RecentFile", RecentFile));
			}
			Section.Children.Add(RecentList);


		}

		//////////////////////////////////////////////////////////////////////////
		public void LoadSettings(SettingsNode RootNode)
		{
			SettingsNode Section = RootNode.GetNode("General", true);
			if (Section == null) return;

			SelectionColor = Section.GetColor("SelectionColor", SelectionColor);
			BoxColor = Section.GetColor("BoxColor", BoxColor);
			WindowCanvas.BackColor = Section.GetColor("BackgroundColor", WindowCanvas.BackColor);
			GridSize = new Size(Section.GetInt("GridWidth", GridSize.Width), Section.GetInt("GridHeight", GridSize.Height));

			_RecentFiles.Clear();
			SettingsNode RecentList = Section.GetNode("RecentFiles");
			if (RecentList != null)
			{
				foreach (SettingsNode Node in RecentList.Children)
				{
					string RecentFile = Node.GetString();
					if(File.Exists(RecentFile)) _RecentFiles.Add(RecentFile);
				}
			}
		}


		//////////////////////////////////////////////////////////////////////////
		public ActionStripItem DefineMainMenu()
		{
			ActionStripItem Root = new ActionStripItem();

			ActionStripItem FileMenu = Root.AddSub("FileMenu", "File");
			FileMenu.AddItem("File.New");
			FileMenu.AddItem("File.Open");
			FileMenu.AddItem("File.Save");
			FileMenu.AddItem("File.SaveAs");
			FileMenu.AddItem();
			FileMenu.AddItem("File.RecentFiles");
			FileMenu.AddItem();
			FileMenu.AddItem("File.AppClose");

			ActionStripItem EditMenu = Root.AddSub("EditMenu", "Edit");
			EditMenu.AddItem("Edit.Undo");
			EditMenu.AddItem("Edit.Redo");
			EditMenu.AddItem();
			EditMenu.AddItem("Edit.Cut");
			EditMenu.AddItem("Edit.Copy");
			EditMenu.AddItem("Edit.Paste");
			EditMenu.AddItem("Document.DeleteItem");

			ActionStripItem ControlMenu = Root.AddSub("ControlMenu", "Controls");
			ControlMenu.AddItem("Document.AddNone");
			ControlMenu.AddItem();
			ControlMenu.AddItem("Document.AddButton");
			ControlMenu.AddItem("Document.AddStatic");
			ControlMenu.AddItem("Document.AddEditor");
			ControlMenu.AddItem("Document.AddEntity");
			ControlMenu.AddItem();
			ControlMenu.AddItem("Document.AddWindow");

			ActionStripItem ToolsMenu = Root.AddSub("ToolsMenu", "Tools");
			ToolsMenu.AddItem("Tools.Options");

			ActionStripItem HelpMenu = Root.AddSub("HelpMenu", "Help");
			HelpMenu.AddItem("Help.Contents");
			HelpMenu.AddItem();
			HelpMenu.AddItem("Help.About");
			
			return Root;
		}

		//////////////////////////////////////////////////////////////////////////
		public ActionStripItem DefineMainToolbar()
		{
			ActionStripItem Root = new ActionStripItem();

			Root.AddItem("File.New");
			Root.AddItem("File.Open");
			Root.AddItem("File.Save");
			Root.AddItem();
			Root.AddItem("Edit.Undo");
			Root.AddItem("Edit.Redo");
			Root.AddItem();
			Root.AddItem("Edit.Cut");
			Root.AddItem("Edit.Copy");
			Root.AddItem("Edit.Paste");
			Root.AddItem("Document.DeleteItem");
			Root.AddItem();
			Root.AddItem("Document.AddNone");
			Root.AddItem();
			Root.AddItem("Document.AddButton");
			Root.AddItem("Document.AddStatic");
			Root.AddItem("Document.AddEditor");
			Root.AddItem("Document.AddEntity");
			Root.AddItem();
			Root.AddItem("Document.AddWindow");
			Root.AddItem();
			Root.AddItem();
			Root.AddItem("Document.Refresh");

			return Root;
		}

		private WindowDoc _CurrentDoc = null;
		//////////////////////////////////////////////////////////////////////////
		public WindowDoc CurrentDoc
		{
			get
			{
				return _CurrentDoc;
			}
			set
			{
				_CurrentDoc = value;
				ActContext.ActivateObject(ActiveObjectSlot.Document, _CurrentDoc);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnDocumentCaptionChanged(object sender, EventArgs e)
		{
			this.Text = ((Document)sender).GetDocumentCaption(AppName);
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnDocumentFileNameChanged(object sender, EventArgs e)
		{
			AddRecentFile(((Document)sender).FileName);
		}

		List<string> _RecentFiles = new List<string>();
		//////////////////////////////////////////////////////////////////////////
		public void RunRecentFile(string FileName)
		{
			OpenFile(FileName);
		}

		//////////////////////////////////////////////////////////////////////////
		private void AddRecentFile(string FileName)
		{
			if (FileName == null || FileName == string.Empty) return;

			foreach(string RecentFile in _RecentFiles)
			{
				if (string.Compare(FileName, RecentFile, true) == 0)
				{
					_RecentFiles.Remove(RecentFile);
					break;
				}				
			}
			_RecentFiles.Insert(0, FileName);
			while(_RecentFiles.Count > 8)
			{
				_RecentFiles.RemoveAt(_RecentFiles.Count - 1);
			}

			ActContext.StripBuilder.RefreshManagedToolStrips();
		}


		#region Actions
		//////////////////////////////////////////////////////////////////////////
		[ActionProp("File.AppClose",
			Caption = "Exit",
			Type = ActionType.Button,
			ShortcutKeys = Keys.Alt|Keys.F4)
		]
		public void File_AppClose(ActionParam Param)
		{
			switch (Param.Type)
			{
				case ActionParam.QueryType.Invoke:
					this.Close();
					break;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		[ActionProp("File.New",
			Caption = "New",
			IconName = "Graphics.Icons.new.png",
			Type = ActionType.Button,
			ShortcutKeys = Keys.Control | Keys.N)
		]
		public void File_New(ActionParam Param)
		{
			switch (Param.Type)
			{
				case ActionParam.QueryType.Invoke:
					if(CurrentDoc != null)
					{
						if (CurrentDoc.CloseDocument())
						{
							CurrentDoc.CaptionChanged -= new EventHandler(OnDocumentCaptionChanged);
							CurrentDoc.FileNameChanged -= new EventHandler(OnDocumentFileNameChanged);
							CurrentDoc.Dispose();
						}
						else
							return;
					}

					PropItem.SelectedObject = null;

					WindowDoc NewDoc = new WindowDoc(this);
					NewDoc.CaptionChanged += new EventHandler(OnDocumentCaptionChanged);
					NewDoc.FileNameChanged += new EventHandler(OnDocumentFileNameChanged);
					if (NewDoc.NewDocument(WindowCanvas))
					{
						CurrentDoc = NewDoc;
						CurrentDoc.PropGrid = PropItem;
						CurrentDoc.LayoutTree = TreeLayout;
					}
					else CurrentDoc = null;

					break;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		[ActionProp("File.Open",
			Caption = "Open",
			IconName = "Graphics.Icons.open.png",
			Type = ActionType.Button,
			ShortcutKeys = Keys.Control | Keys.O)
		]
		public void File_Open(ActionParam Param)
		{
			switch (Param.Type)
			{
				case ActionParam.QueryType.Invoke:
					OpenFile();
					break;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OpenFile()
		{
			OpenFile(null);
		}

		//////////////////////////////////////////////////////////////////////////
		private void OpenFile(string Filename)
		{
			if (CurrentDoc != null)
			{
				if (CurrentDoc.CloseDocument())
				{
					CurrentDoc.CaptionChanged -= new EventHandler(OnDocumentCaptionChanged);
					CurrentDoc.FileNameChanged -= new EventHandler(OnDocumentFileNameChanged);
					CurrentDoc.Dispose();
				}
				else
					return;
			}
			HourGlass.Enabled = true;
			WindowDoc NewDoc = new WindowDoc(this);
			NewDoc.CaptionChanged += new EventHandler(OnDocumentCaptionChanged);
			NewDoc.FileNameChanged += new EventHandler(OnDocumentFileNameChanged);

			Document.DocumentOpenResult Res = NewDoc.OpenDocument(WindowCanvas, Filename);
			if (Res == Document.DocumentOpenResult.Ok)
			{
				CurrentDoc = NewDoc;
				CurrentDoc.PropGrid = PropItem;
				CurrentDoc.LayoutTree = TreeLayout;
				AddRecentFile(CurrentDoc.FileName);
			}
			else
			{
				CurrentDoc = null;
				if(Res == Document.DocumentOpenResult.Error)
					MessageBox.Show("Error opening file. See WindowEdit.log for details.", Form.ActiveForm.Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
				this.Text = AppName;
			}
			HourGlass.Enabled = false;
		}



		//////////////////////////////////////////////////////////////////////////
		[ActionProp("File.RecentFiles",
			Caption = "Recent files",
			Type = ActionType.Custom)
		]
		public void File_RecentFiles(ActionParam Param)
		{
			switch (Param.Type)
			{
				case ActionParam.QueryType.CustomBuild:
					if(Param.Items != null)
					{
						if (_RecentFiles.Count > 0)
						{
							int Count = 0;
							foreach (string RecentFile in _RecentFiles)
							{
								if (!File.Exists(RecentFile)) continue;

								Count++;
								string FileToLoad = RecentFile;
								ToolStripMenuItem NewItem = new ToolStripMenuItem("&" + Count.ToString() + " " + WmeUtils.ShortenPathname(RecentFile, 50));
								NewItem.Click += delegate(object sender, EventArgs e)
								{
									RunRecentFile(FileToLoad);
								};
								Param.Items.Add(NewItem);
							}
						}
						Param.Processed = true;
					}
					break;
			}
		}


		//////////////////////////////////////////////////////////////////////////
		[ActionProp("Tools.Options",
			Caption = "Options...",
			Type = ActionType.Button)
		]
		public void Tools_Options(ActionParam Param)
		{
			switch (Param.Type)
			{
				case ActionParam.QueryType.Invoke:
					SettingsForm dlg = new SettingsForm();
					dlg.GridSize = GridSize;
					dlg.SelectionColor = SelectionColor;
					dlg.BoxColor = BoxColor;
					dlg.BackgroundColor = WindowCanvas.BackColor;

					if(dlg.ShowDialog() == DialogResult.OK)
					{
						GridSize = dlg.GridSize;
						SelectionColor = dlg.SelectionColor;
						BoxColor = dlg.BoxColor;
						WindowCanvas.BackColor = dlg.BackgroundColor;
					}
					break;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		[ActionProp("Help.Contents",
			Caption = "Documentation",
			IconName = "Graphics.Icons.help.png",
			ShortcutKeys = Keys.F1,
			Type = ActionType.Button)
		]
		public void Help_Contents(ActionParam Param)
		{
			switch (Param.Type)
			{
				case ActionParam.QueryType.Invoke:
					string ChmPath = WmeUtils.ToolsPath + "wme.chm";
					Help.ShowHelp(this, ChmPath);
					break;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		[ActionProp("Help.About",
			Caption = "About...",
			Type = ActionType.Button)
		]
		public void Help_About(ActionParam Param)
		{
			switch (Param.Type)
			{
				case ActionParam.QueryType.Invoke:
					FrmAbout dlg = new FrmAbout();
					dlg.ShowDialog();
					break;
			}
		}

		#endregion


	}
}
