// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using DeadCode.WME.Global.Actions;
using System.Windows.Forms;
using DeadCode.WME.Core;
using System.IO;
using DeadCode.WME.Global;
using DeadCode.WME.Controls;
using DeadCode.WME.DefinitionFileParser;
using System.Drawing;
using System.Media;
using System.ComponentModel;

namespace DeadCode.WME.WindowEdit
{
	public partial class WindowDoc : Document, IActionProvider, IDisposable
	{
		const string ClipFormat = "Wme.WindowEdit.ItemsCollection";

		//////////////////////////////////////////////////////////////////////////
		public WindowDoc(FormBase ParentForm)
		{
			_ParentForm = ParentForm;
		}

		//////////////////////////////////////////////////////////////////////////
		public void Dispose()
		{
			DisposeNatives();
			GC.SuppressFinalize(this);
		}

		//////////////////////////////////////////////////////////////////////////
		private void DisposeNatives()
		{
			if(_Canvas != null)
			{
				_Canvas.PaintContent -= new WmeCanvas.PaintContentDelegate(OnPaintContent);
				_Canvas.MouseMove -= new MouseEventHandler(OnMouseMove);
				_Canvas.MouseDown -= new MouseEventHandler(OnMouseDown);
				_Canvas.MouseUp -= new MouseEventHandler(OnMouseUp);
				_Canvas.KeyDown -= new KeyEventHandler(OnKeyDown);

				_Canvas.Release();
				_Canvas.Invalidate();
				_Canvas = null;
			}

			if(LayoutTree != null)
			{
				LayoutTree.SelectionsChanged -= new EventHandler(OnSelectionChanged);
				LayoutTree.ItemDrag -= new ItemDragEventHandler(TreeItemDrag);
				LayoutTree.DragEnter -= new DragEventHandler(TreeDragEnter);
				LayoutTree.DragOver -= new DragEventHandler(TreeDragOver);
				LayoutTree.DragLeave -= new EventHandler(TreeDragLeave);
				LayoutTree.DragDrop -= new DragEventHandler(TreeDragDrop);
				LayoutTree.KeyUp -= new KeyEventHandler(TreeKeyUp);

				LayoutTree.Nodes.Clear();
				LayoutTree.SelectedNodes.Clear();
			}
			Application.Idle -= new EventHandler(OnAppIdle);

			if (_PropGrid != null)
			{
				_PropGrid.SelectedGridItemChanged -= new SelectedGridItemChangedEventHandler(OnPropertySelected);
				_PropGrid.SelectedObject = null;
			}

			RectResizer = null;

			if (Window != null)
			{
				Window.Dispose();
				Window = null;
			}

			if (InvBox != null)
			{
				InvBox.Dispose();
				InvBox = null;
			}

			if (RespBox != null)
			{
				RespBox.Dispose();
				RespBox = null;
			}

			if (Game != null)
			{
				Game.Dispose();
				Game = null;
			}
		}


		private FormBase _ParentForm;
		//////////////////////////////////////////////////////////////////////////
		public FormBase ParentForm
		{
			get
			{
				return _ParentForm;
			}
		}

		private WmeCanvas _Canvas;
		//////////////////////////////////////////////////////////////////////////
		public WmeCanvas Canvas
		{
			get
			{
				return _Canvas;
			}
		}

		private PropertyGrid _PropGrid = null;
		//////////////////////////////////////////////////////////////////////////
		public PropertyGrid PropGrid
		{
			get
			{
				return _PropGrid;
			}
			set
			{
				_PropGrid = value;

				if (_PropGrid != null)
				{
					_PropGrid.SelectedGridItemChanged += new SelectedGridItemChangedEventHandler(OnPropertySelected);
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnPropertySelected(object sender, SelectedGridItemChangedEventArgs e)
		{
			bool Handled = false;
			if (e.NewSelection != null && PropGrid.SelectedObjects.Length == 1 && PropGrid.SelectedObject is UiWindowProxy)
			{
				if (e.NewSelection.Value is Rectangle)
				{
					RectangleResizer.RectType NewType = RectangleResizer.RectType.Unknown;

					if(e.NewSelection.Label=="TitleRectangle")
						NewType = RectangleResizer.RectType.Title;
					else if (e.NewSelection.Label == "DragRectangle")
						NewType = RectangleResizer.RectType.Drag;

					if (NewType != RectangleResizer.RectType.Unknown)
					{
						UiWindowProxy Proxy = PropGrid.SelectedObject as UiWindowProxy;
						WUIWindow NewWindow = Proxy.NativeObject;

						if (RectResizer == null || RectResizer.Window != NewWindow || RectResizer.Type != NewType)
							RectResizer = new RectangleResizer(NewWindow, NewType);
						Handled = true;
					}
				}
			}
			if (!Handled) RectResizer = null;
		}

		private TreeViewMS _LayoutTree = null;
		//////////////////////////////////////////////////////////////////////////
		public TreeViewMS LayoutTree
		{
			get
			{
				return _LayoutTree;
			}
			set
			{
				_LayoutTree = value;
				_LayoutTree.SelectionsChanged += new EventHandler(OnSelectionChanged);
				
				// drag and drop stuff
				_LayoutTree.ItemDrag += new ItemDragEventHandler(TreeItemDrag);
				_LayoutTree.DragEnter += new DragEventHandler(TreeDragEnter);
				_LayoutTree.DragOver += new DragEventHandler(TreeDragOver);
				_LayoutTree.DragLeave += new EventHandler(TreeDragLeave);
				_LayoutTree.DragDrop += new DragEventHandler(TreeDragDrop);
				_LayoutTree.KeyUp += new KeyEventHandler(TreeKeyUp);

				FillLayout();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnSelectionChanged(object sender, EventArgs e)
		{
			if (UpdatingTree) return;

			DeselectAll();

			List<object> SelObj = new List<object>();
			foreach(TreeNode Node in LayoutTree.SelectedNodes)
			{
				NativePropProxy Proxy = Node.Tag as NativePropProxy;
				if (Proxy != null && Proxy.NativeObject != null)
					Proxy.NativeObject.EditorSelected = true;

				SelObj.Add(Node.Tag);
			}
			// update property grid
			if(PropGrid != null) PropGrid.SelectedObjects = SelObj.ToArray();
		}

		//////////////////////////////////////////////////////////////////////////
		private void FillLayout()
		{
			if (LayoutTree == null) return;

			List<WObject> ExpandedNodes = new List<WObject>();
			List<WObject> SelectedNodes = new List<WObject>();
			GetNodeStates(LayoutTree.Nodes, ExpandedNodes, SelectedNodes);

			UpdatingTree = true;
			LayoutTree.BeginUpdate();

			LayoutTree.SelectedNodes.Clear();
			LayoutTree.Nodes.Clear();
			
			WUIWindow Win = this.MainWindow;
			if (Win == null) return;

			FillLayoutNodes(LayoutTree.Nodes, Win);

			if(InvBox != null)
			{
				TreeNode Node = new TreeNode();
				Node.Text = "Inventory box";
				Node.ImageKey = GetIconKey(InvBox);
				Node.Tag = new InvBoxProxy(InvBox);
				LayoutTree.Nodes.Add(Node);
			}
			if (RespBox != null)
			{
				TreeNode Node = new TreeNode();
				Node.Text = "Response box";
				Node.ImageKey = GetIconKey(RespBox);
				Node.Tag = new RespBoxProxy(RespBox);
				LayoutTree.Nodes.Add(Node);
			}
			
			SetNodeStates(LayoutTree.Nodes, ExpandedNodes, SelectedNodes, true);
			LayoutTree.EndUpdate();			
			UpdatingTree = false;

			// invoke selection changed
			OnSelectionChanged(LayoutTree, new EventArgs());
		}

		//////////////////////////////////////////////////////////////////////////
		private void GetNodeStates(TreeNodeCollection TreeNodes, List<WObject> ExpandedNodes, List<WObject> SelectedNodes)
		{
			foreach(TreeNode Node in TreeNodes)
			{
				if (Node.Tag != null && Node.Tag is NativePropProxy)
				{
					if(Node.IsExpanded)
						ExpandedNodes.Add(((NativePropProxy)Node.Tag).NativeObject);
					if (LayoutTree.SelectedNodes.Contains(Node))
						SelectedNodes.Add(((NativePropProxy)Node.Tag).NativeObject);
				}
				if (Node.Nodes.Count > 0) GetNodeStates(Node.Nodes, ExpandedNodes, SelectedNodes);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void SetNodeStates(TreeNodeCollection TreeNodes, List<WObject> ExpandedNodes, List<WObject> SelectedNodes, bool HandleSelection)
		{
			foreach (TreeNode Node in TreeNodes)
			{
				if (Node.Tag != null && Node.Tag is NativePropProxy)
				{
					bool NeedsSelection = false;

					NativePropProxy Proxy = Node.Tag as NativePropProxy;
					if (ExpandedNodes != null && ExpandedNodes.Contains(Proxy.NativeObject)) Node.Expand();
					if (SelectedNodes != null && SelectedNodes.Contains(Proxy.NativeObject))
					{
						NeedsSelection = true;
					}

					if(Proxy is UiProxy)
					{
						WUIObject Obj = ((UiProxy)Proxy).NativeObject;
						Node.Text = Obj.Name;
						if (Obj.Text != null && Obj.Text != string.Empty)
							Node.Text += " [" + Obj.Text.Replace('\n', '|') + "]";
					}

					if (Proxy.NativeObject.EditorSelected) NeedsSelection = true;

					if (HandleSelection)
					{
						if (NeedsSelection)
						{
							if (Node.Parent != null) Node.Parent.Expand();
							LayoutTree.SelectedNodes.Add(Node);
							Node.EnsureVisible();
						}
						else
						{
							if (LayoutTree.SelectedNodes.Contains(Node))
								LayoutTree.SelectedNodes.Remove(Node);
						}
					}

				}
				if (Node.Nodes.Count > 0) SetNodeStates(Node.Nodes, ExpandedNodes, SelectedNodes, HandleSelection);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void FillLayoutNodes(TreeNodeCollection Nodes, WUIWindow Win)
		{
			TreeNode Node = new TreeNode();
			
			Node.ImageKey = GetIconKey(Win);
			UiWindowProxy WinProxy = new UiWindowProxy(Win);
			WinProxy.RefreshNeeded += new EventHandler(OnTreeRefresh);
			WinProxy.PropertyChanging += new PropertyChangedEventHandler(OnPropertyChanging);
			WinProxy.PropertyChanged += new PropertyChangedEventHandler(OnLayoutChanged);
			Node.Tag = WinProxy;
			Nodes.Add(Node);

			foreach(WUIObject Obj in Win.Controls)
			{
				if (Obj is WUIWindow)
				{
					FillLayoutNodes(Node.Nodes, Obj as WUIWindow);
				}
				else
				{
					TreeNode Node2 = new TreeNode();
					Node2.ImageKey = GetIconKey(Obj);
					
					UiProxy ControlProxy;					
					if(Obj is WUIEntity)
						ControlProxy = new UiEntityProxy(Obj as WUIEntity);
					else if(Obj is WUIStatic)
						ControlProxy = new UiStaticProxy(Obj as WUIStatic);
					else if(Obj is WUIButton)
						ControlProxy = new UiButtonProxy(Obj as WUIButton);
					else if (Obj is WUIEdit)
						ControlProxy = new UiEditorProxy(Obj as WUIEdit);
					else
						ControlProxy = new UiControlProxy(Obj);

					ControlProxy.RefreshNeeded += new EventHandler(OnTreeRefresh);
					ControlProxy.PropertyChanging += new PropertyChangedEventHandler(OnPropertyChanging);
					ControlProxy.PropertyChanged += new PropertyChangedEventHandler(OnLayoutChanged);
					Node2.Tag = ControlProxy;
					
					Node.Nodes.Add(Node2);
					Node.Expand();
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnTreeRefresh(object sender, EventArgs e)
		{
			RefreshTree(false);
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnLayoutChanged(object sender, PropertyChangedEventArgs e)
		{
			UpdateScrollSize();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnPropertyChanging(object sender, PropertyChangedEventArgs e)
		{
			string UndoName = "Property changed";
			if(e.PropertyName != null && e.PropertyName != "")
				UndoName += " '" + e.PropertyName + "'";
			
			SaveUndoState(UndoName);
		}

		bool UpdatingTree = false;
		//////////////////////////////////////////////////////////////////////////
		private void RefreshTree(bool HandleSelection)
		{
			if (LayoutTree != null)
			{
				UpdatingTree = true;
				LayoutTree.BeginUpdate();
				SetNodeStates(LayoutTree.Nodes, null, null, HandleSelection);
				LayoutTree.EndUpdate();
				UpdatingTree = false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		string GetIconKey(WObject Obj)
		{
			if (LayoutTree == null || LayoutTree.ImageList == null) return "";

			ImageList Img = LayoutTree.ImageList;
			if (Obj is WUIWindow) return "window";
			else if (Obj is WUIButton) return "button";
			else if (Obj is WUIEdit) return "editor";
			else if (Obj is WUIStatic) return "static";
			else if (Obj is WUIEntity) return "entity";
			else return "box";
		}

		//////////////////////////////////////////////////////////////////////////
		public bool CloseDocument()
		{
			bool CanClose = false;
			if (IsDirty)
			{
				DialogResult Res = MessageBox.Show("Do you want to save your changes?", Form.ActiveForm.Text, MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);
				switch(Res)
				{
					case DialogResult.Yes:
						CanClose = SaveDocument(false);
						break;
					case DialogResult.No:
						CanClose = true;
						break;
					default:
						CanClose = false;
						break;
				}
			}
			else CanClose = true;

			if(CanClose)
			{
				FileName = "";
				IsDirty = false;
				DisposeNatives();
				return true;
			}
			return CanClose;
		}

		//////////////////////////////////////////////////////////////////////////
		public bool NewDocument(WmeCanvas Canvas)
		{
			DocumentTypeForm dlg = new DocumentTypeForm();
			dlg.AppMgr = ParentForm.AppMgr;
			if (dlg.ShowDialog() != DialogResult.OK) return false;


			if (InitEngine(null, Canvas))
			{
				WUIWindow Win = new WUIWindow(Game);
				Win.Width = 200;
				Win.Height = 100;
				Game.Windows.Add(Win);
				Game.FocusedWindow = Win;

				switch(dlg.SelectedType)
				{
					case "InvBox":
						InvBox = new WAdInventoryBox(Game);
						InvBox.Window = Win;
						InvBox.Area = new Rectangle(0, 0, Win.Width, Win.Height);
						break;
					case "RespBox":
						RespBox = new WAdResponseBox(Game);
						RespBox.Window = Win;
						RespBox.Area = new Rectangle(0, 0, Win.Width, Win.Height);
						break;
					default:
						Window = Win;
						break;
				}


				UpdateScrollSize();
				FileName = "";
				IsDirty = true;

				return true;
			}
			else return false;
		}

		//////////////////////////////////////////////////////////////////////////
		private bool InitEngine(string DefinitionFile, WmeCanvas Canvas)
		{
			try
			{
				DisposeNatives();

				string ProjectFile = GetProjectFile(DefinitionFile, ParentForm);
				if (ProjectFile != null)
				{
					string LogFile = Path.Combine(Path.GetDirectoryName(ProjectFile), "WindowEdit.log");

					Game = new WAdGame();
					Game.ForceScripts = true;
					Game.DoNotExpandStrings = true;
					Game.EditorMode = true;
					if (!Canvas.Create(Game, ProjectFile, LogFile))
					{
						DisposeNatives();
						return false;
					}
					_Canvas = Canvas;
					_Canvas.PaintContent += new WmeCanvas.PaintContentDelegate(OnPaintContent);
					_Canvas.MouseMove += new MouseEventHandler(OnMouseMove);
					_Canvas.MouseDown += new MouseEventHandler(OnMouseDown);
					_Canvas.MouseUp += new MouseEventHandler(OnMouseUp);
					_Canvas.KeyDown += new KeyEventHandler(OnKeyDown);

					Application.Idle += new EventHandler(OnAppIdle);

					return true;
				}
				return false;
			}
			catch(Exception e)
			{
				MessageBox.Show("Error initializing game engine:\n\n" + e.Message, Form.ActiveForm.Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public DocumentOpenResult OpenDocument(WmeCanvas Canvas)
		{
			return OpenDocument(Canvas, null);
		}

		//////////////////////////////////////////////////////////////////////////
		public DocumentOpenResult OpenDocument(WmeCanvas Canvas, string FileName)
		{
			if (FileName == null)
			{
				OpenFileDialog dlg = new OpenFileDialog();
				dlg.Filter = "Windows and definition files (*.window; *.def)|*.window;*.def|Windows (*.window)|*.window|Defintion files (*.def)|*.def|All files (*.*)|*.*";
				dlg.RestoreDirectory = true;
				dlg.CheckFileExists = true;

				if (dlg.ShowDialog() != DialogResult.OK) return DocumentOpenResult.Cancel;
				else FileName = dlg.FileName;
			}

			DefinitionFile DefFile = new DefinitionFile();
			string FileType = "";

			if(DefFile.ParseFile(FileName) && DefFile.Children.Count > 0)
				FileType = DefFile.Children[0].Name.ToUpper();
			
			if(FileType != "WINDOW" && FileType != "INVENTORY_BOX" && FileType != "RESPONSE_BOX")
			{
				MessageBox.Show("Unsupported file type.", Form.ActiveForm.Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
				return DocumentOpenResult.Cancel;
			}

			bool Ret = false;

			if (InitEngine(FileName, Canvas))
			{				
				Game.AbsolutePathWarning = false;
				switch(FileType)
				{
					case "WINDOW":
						Window = new WUIWindow(Game);
						Ret = Window.LoadFromFile(FileName);
						if(Ret)
						{
							Game.Windows.Add(Window);
							Game.FocusedWindow = Window;
						}
						break;

					case "INVENTORY_BOX":
						InvBox = new WAdInventoryBox(Game);
						Ret = InvBox.LoadFromFile(FileName);
						if(Ret && InvBox.Window != null)
						{
							Game.Windows.Add(InvBox.Window);
							Game.FocusedWindow = InvBox.Window;
						}
						break;

					case "RESPONSE_BOX":
						RespBox = new WAdResponseBox(Game);
						Ret = RespBox.LoadFromFile(FileName);
						if (Ret && RespBox.Window != null)
						{
							Game.Windows.Add(RespBox.Window);
							Game.FocusedWindow = RespBox.Window;
						}
						break;
				
				}
				Game.AbsolutePathWarning = true;
				UpdateScrollSize();

				this.FileName = FileName;
			}
			else Ret = false;

			if (!Ret) DisposeNatives();

			if (Ret) return DocumentOpenResult.Ok;
			else return DocumentOpenResult.Error;
		}

		//////////////////////////////////////////////////////////////////////////
		public bool SaveDocument(bool SaveAs)
		{
			if (FileName == "") SaveAs = true;

			string NewFileName = FileName;

			if (SaveAs)
			{
				string Filter;
				if (InvBox != null || RespBox != null)
					Filter = "Definiton files (*.def)|*.def";
				else
					Filter = "Windows (*.window)|*.window";
				Filter += "|All files (*.*)|*.*";

				SaveFileDialog dlg = new SaveFileDialog();
				dlg.FileName = FileName;
				dlg.Filter = Filter;
				dlg.AddExtension = true;
				dlg.CheckPathExists = true;
				dlg.OverwritePrompt = true;
				dlg.RestoreDirectory = true;

				if (dlg.ShowDialog() != DialogResult.OK) return false;

				NewFileName = dlg.FileName;
			}

			DeleteEditorProps();

			WDynBuffer Buf = new WDynBuffer(Game);
			if (InvBox != null) InvBox.SaveAsText(Buf);
			else if (RespBox != null) RespBox.SaveAsText(Buf);
			else Window.SaveAsText(Buf);

			string RelProjectPath = WmeUtils.GetRelativePath(NewFileName, Path.GetDirectoryName(Game.ProjectFile) + "\\");

			string FileContent = "";
			FileContent += "; generated by WindowEdit\n\n";
			FileContent += "; $EDITOR_PROJECT_ROOT_DIR$ " + RelProjectPath + "\n\n";
			FileContent += Buf.Text;

			Buf.Dispose();

			try
			{
				using (StreamWriter sw = new StreamWriter(NewFileName, false, Encoding.Default))
				{
					sw.WriteLine(FileContent);
				}

				FileName = NewFileName;
				IsDirty = false;
				
				return true;
			}
			catch(Exception e)
			{
				MessageBox.Show("Error saving file.\n\n" + e.Message, Form.ActiveForm.Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
				return false;
			}			
		}

		private bool RefreshProps = false;
		//////////////////////////////////////////////////////////////////////////
		private void OnPaintContent(object sender, EventArgs e)
		{
			if (MainWindow != null)
			{				
				MainWindow.X -= Canvas.HorizontalScroll.Value;
				MainWindow.Y -= Canvas.VerticalScroll.Value;
				
				Game.DisplayWindows(true);
				Game.DisplayWindows(false);
				Game.ActiveObject = Game.Renderer.GetObjectAt(Game.MousePos);
			
				PaintSelections();

				MainWindow.X += Canvas.HorizontalScroll.Value;
				MainWindow.Y += Canvas.VerticalScroll.Value;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnAppIdle(object sender, EventArgs e)
		{
			if (RefreshProps && PropGrid != null)
			{
				PropGrid.Refresh();
				RefreshProps = false;
			}

			if (!IsMoving && !IsResizing)
			{
				foreach (IEditorResizable It in AllItems)
				{
					SetOrigPos(It);
				}
			}
		}

		private List<HotSpot> HotSpots = new List<HotSpot>();
		private Rectangle AddingRect = Rectangle.Empty;
		//////////////////////////////////////////////////////////////////////////
		private void PaintSelections()
		{
			HotSpots.Clear();

			if (IsAdding)
			{
				if (!AddingRect.IsEmpty)
				{
					Game.Renderer.DrawRect(AddingRect, SelectionColor);
				}
			}
			else
			{
				PaintSelections(MainWindow);

				if (InvBox != null) PaintInventoryBox(InvBox);
				if (RespBox != null) PaintResponseBox(RespBox);
				if (RectResizer != null) PaintResizer(RectResizer);
			}
		}
		
		//////////////////////////////////////////////////////////////////////////
		private void PaintSelections(WUIWindow Win)
		{
			Rectangle Rect;

			int OffsetX = 0;
			int OffsetY = 0;

			bool MultiSel = SelectedControls.Length > 1;

			bool WinVisible = Win.Visible;
			
			WUIObject Parent = Win.Parent;
			while(Parent != null)
			{
				if (!Parent.Visible) WinVisible = false;
				OffsetX += Parent.X;
				OffsetY += Parent.Y;
				Parent = Parent.Parent;
			}

			Rect = new Rectangle(OffsetX + Win.X, OffsetY + Win.Y, Win.Width - 1, Win.Height - 1);

			if(WinVisible)
				HotSpots.Add(new HotSpot(Rect, Win, HotSpot.HotSpotType.Container));

			if (Win.EditorSelected && RectResizer == null)
			{
				Game.Renderer.DrawRect(Rect, SelectionColor);
				if (!MultiSel && WinVisible) PaintResizeHandles(Rect, Win);
			}

			OffsetX += Win.X;
			OffsetY += Win.Y;
			foreach(WUIObject C in Win.Controls)
			{
				if (C is WUIWindow) PaintSelections(C as WUIWindow);
				else
				{
					bool IsVisible = C.Visible && WinVisible;

					Rect = new Rectangle(OffsetX + C.X, OffsetY + C.Y, C.Width - 1, C.Height - 1);
					if(IsVisible)
						HotSpots.Add(new HotSpot(Rect, C, HotSpot.HotSpotType.Control));

					if (C.EditorSelected && RectResizer == null)
					{
						Game.Renderer.DrawRect(Rect, SelectionColor);
						if (!MultiSel && !(C is WUIEntity) && IsVisible) PaintResizeHandles(Rect, C);
					}
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void PaintResponseBox(WAdResponseBox RespBox)
		{
			int OffsetX = 0;
			int OffsetY = 0;

			if (MainWindow != null)
			{
				OffsetX += MainWindow.X;
				OffsetY += MainWindow.Y;
			}

			Color Col = RespBox.EditorSelected ? SelectionColor : BoxColor;

			Rectangle Rect = new Rectangle(OffsetX + RespBox.Area.X, OffsetY + RespBox.Area.Y, RespBox.Area.Width - 1, RespBox.Area.Height - 1);
			Game.Renderer.DrawRect(Rect, Col);

			if (RespBox.EditorSelected && RectResizer == null)
			{
				HotSpots.Add(new HotSpot(Rect, RespBox, HotSpot.HotSpotType.Control));
				PaintResizeHandles(Rect, RespBox);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void PaintInventoryBox(WAdInventoryBox InvBox)
		{
			int OffsetX = 0;
			int OffsetY = 0;

			if (MainWindow != null)
			{
				OffsetX += MainWindow.X;
				OffsetY += MainWindow.Y;
			}

			Color Col = InvBox.EditorSelected ? SelectionColor : BoxColor;

			Rectangle Rect;

			// paint item rectangles
			int X = InvBox.Area.X;
			int Y = InvBox.Area.Y;

			while(Y + InvBox.ItemHeight <= InvBox.Area.Y + InvBox.Area.Height)
			{
				while(X + InvBox.ItemWidth <= InvBox.Area.X + InvBox.Area.Width)
				{
					Rect = new Rectangle(OffsetX + X, OffsetY + Y, InvBox.ItemWidth - 1, InvBox.ItemHeight - 1);
					Game.Renderer.DrawRect(Rect, Col);

					X += InvBox.ItemWidth + InvBox.Spacing;
				}
				X = InvBox.Area.X;
				Y += InvBox.ItemHeight + InvBox.Spacing;
			}


			Rect = new Rectangle(OffsetX + InvBox.Area.X, OffsetY + InvBox.Area.Y, InvBox.Area.Width - 1, InvBox.Area.Height - 1);
			Game.Renderer.DrawRect(Rect, Col);

			if (InvBox.EditorSelected && RectResizer == null)
			{
				HotSpots.Add(new HotSpot(Rect, InvBox, HotSpot.HotSpotType.Control));
				PaintResizeHandles(Rect, InvBox);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void PaintResizer(RectangleResizer Resizer)
		{
			int OffsetX = 0;
			int OffsetY = 0;

			WUIObject Parent = Resizer.Window;
			while (Parent != null)
			{
				OffsetX += Parent.X;
				OffsetY += Parent.Y;
				Parent = Parent.Parent;
			}

			Rectangle Rect = new Rectangle(OffsetX + Resizer.X, OffsetY + Resizer.Y, Resizer.Width - 1, Resizer.Height - 1);
			Game.Renderer.DrawRect(Rect, SelectionColor);
			HotSpots.Add(new HotSpot(Rect, Resizer, HotSpot.HotSpotType.Control));
			PaintResizeHandles(Rect, Resizer);
		}

		//////////////////////////////////////////////////////////////////////////
		private WUIObject[] SelectedControls
		{
			get
			{
				List<WUIObject> SelObj = new List<WUIObject>();
				GetSelectedObjects(MainWindow, SelObj);
				return SelObj.ToArray();
			}
		}
		
		RectangleResizer RectResizer = null;
		//////////////////////////////////////////////////////////////////////////
		private IEditorResizable[] SelectedItems
		{
			get
			{
				List<IEditorResizable> SelItems = new List<IEditorResizable>();
				
				if (RespBox != null && RespBox.EditorSelected)
				{
					SelItems.Add(RespBox);
				}
				else
				if (InvBox != null && InvBox.EditorSelected)
				{
					SelItems.Add(InvBox);
				}
				else
				{
					List<WUIObject> SelObj = new List<WUIObject>();
					GetSelectedObjects(MainWindow, SelObj);
					foreach(WUIObject C in SelObj)
					{
						SelItems.Add(C);
					}
				}
				if (RectResizer != null) SelItems.Add(RectResizer);

				return SelItems.ToArray();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void GetSelectedObjects(WUIWindow Win, List<WUIObject> SelObj)
		{
			if (Win.EditorSelected) SelObj.Add(Win);
			foreach(WUIObject C in Win.Controls)
			{
				if (C is WUIWindow) GetSelectedObjects(C as WUIWindow, SelObj);
				else if (C.EditorSelected) SelObj.Add(C);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private WUIObject[] AllControls
		{
			get
			{
				List<WUIObject> AllObj = new List<WUIObject>();
				GetAllObjects(MainWindow,AllObj);
				return AllObj.ToArray();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private IEditorResizable[] AllItems
		{
			get
			{
				List<IEditorResizable> Items = new List<IEditorResizable>();

				if (RespBox != null)
				{
					Items.Add(RespBox);
				}
				
				if (InvBox != null)
				{
					Items.Add(InvBox);
				}
				
				List<WUIObject> AllObj = new List<WUIObject>();
				GetAllObjects(MainWindow, AllObj);
				foreach (WUIObject C in AllObj)
				{
					Items.Add(C);
				}

				if (RectResizer != null) Items.Add(RectResizer);


				return Items.ToArray();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void GetAllObjects(WUIWindow Win, List<WUIObject> AllObj)
		{
			AllObj.Add(Win);
			foreach (WUIObject C in Win.Controls)
			{
				if (C is WUIWindow) GetAllObjects(C as WUIWindow, AllObj);
				else AllObj.Add(C);
			}
		}


		//////////////////////////////////////////////////////////////////////////
		private void PaintResizeHandles(Rectangle OwnerRect, IEditorResizable Owner)
		{
			Rectangle Rect;

			Rect = new Rectangle(OwnerRect.X - 2, OwnerRect.Y - 2, 5, 5);
			Game.Renderer.DrawRect(Rect, SelectionColor);
			HotSpots.Add(new HotSpot(Rect, Owner, HotSpot.HotSpotType.ResizeNW));

			Rect = new Rectangle(OwnerRect.X + OwnerRect.Width / 2 - 2, OwnerRect.Y - 2, 5, 5);
			Game.Renderer.DrawRect(Rect, SelectionColor);
			HotSpots.Add(new HotSpot(Rect, Owner, HotSpot.HotSpotType.ResizeN));

			Rect = new Rectangle(OwnerRect.X + OwnerRect.Width - 3, OwnerRect.Y - 2, 5, 5);
			Game.Renderer.DrawRect(Rect, SelectionColor);
			HotSpots.Add(new HotSpot(Rect, Owner, HotSpot.HotSpotType.ResizeNE));

			Rect = new Rectangle(OwnerRect.X - 2, OwnerRect.Y + OwnerRect.Height / 2 - 2, 5, 5);
			Game.Renderer.DrawRect(Rect, SelectionColor);
			HotSpots.Add(new HotSpot(Rect, Owner, HotSpot.HotSpotType.ResizeW));

			Rect = new Rectangle(OwnerRect.X + OwnerRect.Width - 3, OwnerRect.Y + OwnerRect.Height / 2 - 2, 5, 5);
			Game.Renderer.DrawRect(Rect, SelectionColor);
			HotSpots.Add(new HotSpot(Rect, Owner, HotSpot.HotSpotType.ResizeE));

			Rect = new Rectangle(OwnerRect.X - 2, OwnerRect.Y + OwnerRect.Height - 3, 5, 5);
			Game.Renderer.DrawRect(Rect, SelectionColor);
			HotSpots.Add(new HotSpot(Rect, Owner, HotSpot.HotSpotType.ResizeSW));

			Rect = new Rectangle(OwnerRect.X + OwnerRect.Width / 2 - 2, OwnerRect.Y + OwnerRect.Height - 3, 5, 5);
			Game.Renderer.DrawRect(Rect, SelectionColor);
			HotSpots.Add(new HotSpot(Rect, Owner, HotSpot.HotSpotType.ResizeS));

			Rect = new Rectangle(OwnerRect.X + OwnerRect.Width - 3, OwnerRect.Y + OwnerRect.Height - 3, 5, 5);
			Game.Renderer.DrawRect(Rect, SelectionColor);
			HotSpots.Add(new HotSpot(Rect, Owner, HotSpot.HotSpotType.ResizeSE));
			
		}
		

		//////////////////////////////////////////////////////////////////////////
		private void DeselectAll()
		{
			foreach(IEditorResizable It in AllItems)
			{
				It.EditorSelected = false;
			}
		}


		//////////////////////////////////////////////////////////////////////////
		private void CreateControl(Type ControlToAdd, Rectangle AddingRect)
		{
			WUIObject NewControl = null;
			if (ControlToAdd != null)
			{
				try
				{
					NewControl = (WUIObject)Activator.CreateInstance(ControlToAdd, new object[] { Game });
				}
				catch
				{
					NewControl = null;
				}
			}
			if (NewControl != null)
			{
				SaveUndoState("Create control");

				if(AddingRect.IsEmpty)
				{
					AddingRect.X = StartPoint.X;
					AddingRect.Y = StartPoint.Y;
				}

				AddingRect.Offset(Canvas.HorizontalScroll.Value, Canvas.VerticalScroll.Value);				

				WUIWindow Parent = GetNearestWindow(AddingRect.X, AddingRect.Y);

				if (AddingRect.Width < 0)
				{
					AddingRect.Width = -AddingRect.Width;
					AddingRect.X -= AddingRect.Width;
				}
				if (AddingRect.Height < 0)
				{
					AddingRect.Height = -AddingRect.Height;
					AddingRect.Y -= AddingRect.Height;
				}

				int OffsetX, OffsetY;
				Parent.GetTotalOffset(out OffsetX, out OffsetY);

				NewControl.X = AddingRect.X - OffsetX - Parent.X;
				NewControl.Y = AddingRect.Y - OffsetY - Parent.Y;
				NewControl.Width = AddingRect.Width;
				NewControl.Height = AddingRect.Height;

				if (NewControl.Width == 0) NewControl.Width = 100;
				if (NewControl.Height == 0) NewControl.Height = 50;

				if(NewControl is WUIEntity)
				{
					NewControl.Width = 50;
					NewControl.Height = 50;
				}

				if (NewControl is WUIButton) NewControl.Text = "Button";
				if (NewControl is WUIStatic) NewControl.Text = "Static";
				if (NewControl is WUIEdit) NewControl.Text = "Editor";

				NewControl.ParentNotify = true;

				Parent.AddControl(NewControl);

				DeselectAll();
				RefreshTree(true);

				NewControl.EditorSelected = true;
				FillLayout();
				RefreshTree(true);

				OnSelectionChanged(LayoutTree, new EventArgs());

			}
		}

		//////////////////////////////////////////////////////////////////////////
		private WUIWindow GetNearestWindow(int X, int Y)
		{
			for (int i = AllControls.Length - 1; i >= 0; i--)
			{
				WUIWindow C = AllControls[i] as WUIWindow;
				if (C == null) continue;

				int WinX, WinY;
				C.GetTotalOffset(out WinX, out WinY);
				WinX += C.X;
				WinY += C.Y;

				if(X >= WinX && X <= WinX + C.Width && Y >= WinY && Y <= WinY + C.Height)
					return C;
			}
			return MainWindow;
		}

		//////////////////////////////////////////////////////////////////////////
		private static void SetOrigPos(IEditorResizable Obj)
		{
			Obj.OrigRect = new Rectangle(Obj.X, Obj.Y, Obj.Width, Obj.Height);
		}

		//////////////////////////////////////////////////////////////////////////
		private void UpdateScrollSize()
		{
			if(MainWindow != null)
			{
				Rectangle Rect = GetWindowSize(MainWindow);
				if(IsAdding)
				{
					Rectangle Rect2 = AddingRect;
					Rect2.Offset(Canvas.HorizontalScroll.Value, Canvas.VerticalScroll.Value);
					Rect = Rectangle.Union(Rect, Rect2);
				}
				Canvas.AutoScrollMinSize = new Size(Rect.Width + MainWindow.X, Rect.Height + MainWindow.Y);
			}			
		}

		//////////////////////////////////////////////////////////////////////////
		private Rectangle GetWindowSize(WUIWindow Win)
		{
			return GetWindowSize(Win, 0, 0);
		}

		//////////////////////////////////////////////////////////////////////////
		private Rectangle GetWindowSize(WUIWindow Win, int OffsetX, int OffsetY)
		{
			Rectangle Rect = new Rectangle(OffsetX + Win.X, OffsetY + Win.Y, Win.Width, Win.Height);
			foreach(WUIObject Obj in Win.Controls)
			{
				Rectangle Rect2;
				if(Obj is WUIWindow) Rect2 = GetWindowSize((WUIWindow)Obj, OffsetX + Win.X, OffsetY + Win.Y);
				else Rect2 = new Rectangle(OffsetX + Win.X + Obj.X, OffsetY + Win.Y + Obj.Y, Obj.Width, Obj.Height);
				
				Rect = Rectangle.Union(Rect, Rect2);
			}
			return Rect;
		}

		//////////////////////////////////////////////////////////////////////////
		private WUIWindow MainWindow
		{
			get
			{
				if (Window != null) return Window;
				else if (InvBox != null && InvBox.Window != null) return InvBox.Window;
				else if (RespBox != null && RespBox.Window != null) return RespBox.Window;
				else return null;
			}
		}

		private WUIWindow Window = null;
		private WAdGame Game = null;
		private WAdInventoryBox InvBox = null;
		private WAdResponseBox RespBox = null;

		//////////////////////////////////////////////////////////////////////////
		private void AddControl(Type type)
		{
			IsAboutToAdd = true;
			ControlToAdd = type;
		}

		//////////////////////////////////////////////////////////////////////////
		public override string GetCurrentStateForUndo()
		{
			WDynBuffer Buf = new WDynBuffer(Game);

			if (InvBox != null) InvBox.SaveAsText(Buf);
			else if (RespBox != null) RespBox.SaveAsText(Buf);
			else if (Window != null) Window.SaveAsText(Buf);
			else
			{
				Buf.Dispose();
				return null;
			}

			string Ret = Buf.Text;
			Buf.Dispose();

			return Ret;			
		}

		//////////////////////////////////////////////////////////////////////////
		public void LoadUndoState(string State)
		{
			if(InvBox != null)
			{
				WAdInventoryBox NewInvBox = new WAdInventoryBox(Game);
				if (NewInvBox.LoadFromBuffer(State))
				{
					Game.Windows.Remove(MainWindow);
					InvBox.Dispose();
					InvBox = NewInvBox;
				}
				else NewInvBox.Dispose();
			}
			else if(RespBox != null)
			{
				WAdResponseBox NewRespBox = new WAdResponseBox(Game);
				if (NewRespBox.LoadFromBuffer(State))
				{
					Game.Windows.Remove(MainWindow);
					RespBox.Dispose();
					RespBox = NewRespBox;
				}
				else NewRespBox.Dispose();
			}
			else if(Window != null)
			{
				WUIWindow NewWin = new WUIWindow(Game);
				if (NewWin.LoadFromBuffer(State))
				{
					Game.Windows.Remove(MainWindow);
					Window.Dispose();
					Window = NewWin;
				}
				else NewWin.Dispose();
			}

			Game.Windows.Add(MainWindow);
			Game.FocusedWindow = MainWindow;							

			FillLayout();
			RefreshTree(true);
		}

		#region Clipboard handling
		//////////////////////////////////////////////////////////////////////////
		private void DeleteSelectedItems()
		{
			if (!CanDeleteSelectedItems())
			{
				SystemSounds.Asterisk.Play();
				return;
			}

			SaveUndoState("Delete items");
			foreach(WUIObject C in SelectedControls)
			{
				C.Parent.RemoveControl(C);
			}
			FillLayout();
		}

		//////////////////////////////////////////////////////////////////////////
		private bool CanDeleteSelectedItems()
		{
			bool Ret = false;
			if (SelectedControls.Length > 0)
			{
				Ret = true;
				foreach (WUIObject C in SelectedControls)
				{
					if (C == MainWindow)
					{
						Ret = false;
						break;
					}
				}
			}
			return Ret;
		}

		//////////////////////////////////////////////////////////////////////////
		private void DeleteEditorProps()
		{
			foreach(WUIObject C in AllControls)
			{
				DeleteEditorProps(C);
			}
			if (InvBox != null) DeleteEditorProps(InvBox);
			if (RespBox != null) DeleteEditorProps(RespBox);
		}

		//////////////////////////////////////////////////////////////////////////
		private void DeleteEditorProps(WObject Obj)
		{
			Obj.DeleteEditorProp("OrigX");
			Obj.DeleteEditorProp("OrigY");
			Obj.DeleteEditorProp("OrigWidth");
			Obj.DeleteEditorProp("OrigHeight");
		}

		//////////////////////////////////////////////////////////////////////////
		private void CopySelectedItems()
		{
			List<string> Types = new List<string>();
			List<string> Values = new List<string>();

			WDynBuffer Buf = new WDynBuffer(Game);
			foreach(WUIObject C in SelectedControls)
			{
				bool IsParentSelected = false;
				WUIObject Parent = C.Parent;
				while (Parent != null)
				{
					if (Parent.EditorSelected)
					{
						IsParentSelected = true;
						break;
					}
					Parent = Parent.Parent;
				}
				if (IsParentSelected) continue;

				Types.Add(C.GetType().AssemblyQualifiedName);

				Buf.Clear();
				C.SaveAsText(Buf);
				Values.Add(Buf.Text);
			}
			Buf.Dispose();

			if (Types.Count == 0)
			{
				SystemSounds.Asterisk.Play();
				return;
			}

			string[,] ClipData = new string[Types.Count, 2];
			for(int i=0; i<Types.Count; i++)
			{
				ClipData[i, 0] = Types[i];
				ClipData[i, 1] = Values[i];
			}
			Clipboard.SetData(ClipFormat, ClipData);
		}

		//////////////////////////////////////////////////////////////////////////
		private void PasteItems()
		{
			if (!Clipboard.ContainsData(ClipFormat)) return;

			SaveUndoState("Paste from clipboard");

			bool WasError = false;

			DeselectAll();
			RefreshTree(true);

			string[,] ClipData = (string[,])Clipboard.GetData(ClipFormat);
			for (int i = 0; i < ClipData.Length / 2; i++)
			{
				string TypeName = ClipData[i, 0];
				string Value = ClipData[i, 1];

				try
				{
					WUIObject NewControl = (WUIObject)Activator.CreateInstance(Type.GetType(TypeName), new object[] { Game });
					if (NewControl.LoadFromBuffer(Value, true))
					{
						MainWindow.AddControl(NewControl);
						NewControl.EditorSelected = true;
					}
					else
					{
						NewControl.Dispose();
						WasError = true;
					}
				}
				catch
				{
					WasError = true;
				}				
			}
			if (WasError) SystemSounds.Asterisk.Play();
			FillLayout();
			RefreshTree(true);
		}
		#endregion


		#region Drag and drop

		//////////////////////////////////////////////////////////////////////////
		private void TreeItemDrag(object sender, ItemDragEventArgs e)
		{
			foreach(TreeNode Node in LayoutTree.SelectedNodes)
			{
				if (Node.Tag is UiWindowProxy && ((UiWindowProxy)Node.Tag).NativeObject == MainWindow)
					return;

				if (!(Node.Tag is UiProxy)) return;
			}
			LayoutTree.DoDragDrop(e.Item, DragDropEffects.Move);
		}

		//////////////////////////////////////////////////////////////////////////
		private void TreeDragEnter(object sender, DragEventArgs e)
		{
			e.Effect = DragDropEffects.Move;
		}

		//////////////////////////////////////////////////////////////////////////
		private void TreeDragLeave(object sender, EventArgs e)
		{
			DeselectPrevNode();
		}

		TreeNode previousNode = null;
		//////////////////////////////////////////////////////////////////////////
		private void TreeDragOver(object sender, DragEventArgs e)
		{
			// Scrolling down/up
			Point pt = LayoutTree.PointToClient(new Point(e.X, e.Y));

			if (pt.Y + 10 > LayoutTree.ClientSize.Height)
				NativeMethods.SendMessage(LayoutTree.Handle, 277, 1, IntPtr.Zero);
			else if (pt.Y < LayoutTree.Top + 10)
				NativeMethods.SendMessage(LayoutTree.Handle, 277, 0, IntPtr.Zero);


			if (e.Data.GetDataPresent(typeof(NodesCollection)))
			{				
				TreeNode DestinationNode = LayoutTree.GetNodeAt(pt);
				if (DestinationNode != null &&
					!LayoutTree.SelectedNodes.Contains(DestinationNode) &&
					DestinationNode.Tag is UiProxy)
				{
					e.Effect = DragDropEffects.Move;

					if (previousNode != DestinationNode) DeselectPrevNode();

					previousNode = DestinationNode;
					previousNode.BackColor = LayoutTree.SelectionBackColor;
					previousNode.ForeColor = LayoutTree.BackColor;
					
					DestinationNode.EnsureVisible();

					return;
				}
			}
			e.Effect = DragDropEffects.None;
			DeselectPrevNode();
		}

		//////////////////////////////////////////////////////////////////////////
		private void TreeKeyUp(object sender, KeyEventArgs e)
		{
			if(e.KeyCode == Keys.Escape)
			{
				DeselectPrevNode();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void TreeDragDrop(object sender, DragEventArgs e)
		{
			DeselectPrevNode();			

			if(e.Data.GetDataPresent(typeof(NodesCollection)))
			{
				Point pt = LayoutTree.PointToClient(new Point(e.X, e.Y));
				TreeNode DestinationNode = LayoutTree.GetNodeAt(pt);
				if (DestinationNode == null) return;
				if (DestinationNode.Tag as UiProxy == null) return;

				SaveUndoState("Controls reorder");

				WUIObject Target = ((UiProxy)DestinationNode.Tag).NativeObject;

				WUIWindow NewParent;
				int NewIndex;

				if(Target is WUIWindow)
				{
					NewParent = Target as WUIWindow;
					NewIndex = 0;
				}
				else
				{
					NewParent = Target.Parent as WUIWindow;
					NewIndex = NewParent.Controls.IndexOf(Target);
				}

				foreach (TreeNode Node in LayoutTree.SelectedNodes)
				{
					UiProxy Proxy = Node.Tag as UiProxy;
					if(Proxy == null) continue;

					WUIObject Obj = Proxy.NativeObject;

					if (Obj.Parent != NewParent)
					{
						Obj.Parent.Controls.Remove(Obj);
						NewParent.AddControl(Obj);
					}

					NewParent.Controls.Remove(Obj);
					NewParent.Controls.Insert(NewIndex, Obj);

                    // **************FIX****************
                    // This is pretty much bad, because you can't increment index as you go node by node
					// NewIndex++;
                    // **************FIX ENDS****************
				}
				FillLayout();
				RefreshTree(true);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void DeselectPrevNode()
		{
			if (previousNode != null)
			{
				previousNode.BackColor = LayoutTree.BackColor;
				previousNode.ForeColor = LayoutTree.ForeColor;
				previousNode = null;
			}
		}

		#endregion
	}
}
