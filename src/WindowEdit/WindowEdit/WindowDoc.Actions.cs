// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using DeadCode.WME.Global.Actions;
using System.Windows.Forms;
using DeadCode.WME.Core;
using DeadCode.WME.Global.Utils;
using DeadCode.WME.Global;

namespace DeadCode.WME.WindowEdit
{
	public partial class WindowDoc
	{
		//////////////////////////////////////////////////////////////////////////
		[ActionProp("File.Save",
			Caption = "Save",
			IconName = "Graphics.Icons.save.png",
			Type = ActionType.Button,
			ShortcutKeys = Keys.Control | Keys.S)
		]
		public void File_Save(ActionParam Param)
		{
			switch (Param.Type)
			{
				case ActionParam.QueryType.Invoke:
					SaveDocument(false);
					break;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		[ActionProp("File.SaveAs",
			Caption = "Save as...",
			Type = ActionType.Button)
		]
		public void File_SaveAs(ActionParam Param)
		{
			switch (Param.Type)
			{
				case ActionParam.QueryType.Invoke:
					SaveDocument(true);
					break;
			}
		}
		/*
		//////////////////////////////////////////////////////////////////////////
		[ActionProp("Document.Refresh",
			Caption = "Refresh",
			Type = ActionType.Button,
			DisplayType = DisplayType.Text)
		]
		public void Document_Reftesh(ActionParam Param)
		{
			switch (Param.Type)
			{
				case ActionParam.QueryType.Invoke:
					FillLayout();
					TiledImgEditorForm dlg = new TiledImgEditorForm();
					dlg.Game = this.Game;
					dlg.ShowDialog();
					break;
			}
		}
		*/

		//////////////////////////////////////////////////////////////////////////
		[ActionProp("Document.AddNone",
			Caption = "Pointer",
			IconName = "Graphics.Icons.pointer.png",
			Type = ActionType.Toggle,
			DisplayType = DisplayType.Image)
		]
		public void Document_AddNone(ActionParam Param)
		{
			switch (Param.Type)
			{
				case ActionParam.QueryType.Invoke:
					IsAboutToAdd = false;
					IsAdding = false;
					ControlToAdd = null;
					break;
				case ActionParam.QueryType.GetState:
					Param.State.Checked = (!IsAboutToAdd && !IsAdding);
					break;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		[ActionProp("Document.AddWindow",
			Caption = "Add nested window",
			IconName = "Graphics.Icons.window.png",
			Type = ActionType.Toggle,
			DisplayType = DisplayType.Image)
		]
		public void Document_AddWindow(ActionParam Param)
		{
			switch (Param.Type)
			{
				case ActionParam.QueryType.Invoke:
					AddControl(typeof(WUIWindow));
					break;
				case ActionParam.QueryType.GetState:
					Param.State.Checked = (IsAboutToAdd || IsAdding) && ControlToAdd == typeof(WUIWindow);
					break;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		[ActionProp("Document.AddButton",
			Caption = "Add button",
			IconName = "Graphics.Icons.button.png",
		   Type = ActionType.Toggle,
			DisplayType = DisplayType.Image)
		]
		public void Document_AddButton(ActionParam Param)
		{
			switch (Param.Type)
			{
				case ActionParam.QueryType.Invoke:
					AddControl(typeof(WUIButton));
					break;

				case ActionParam.QueryType.GetState:
					Param.State.Checked = (IsAboutToAdd || IsAdding) && ControlToAdd == typeof(WUIButton);
					break;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		[ActionProp("Document.AddStatic",
			Caption = "Add static control",
			IconName = "Graphics.Icons.static.png",
			Type = ActionType.Toggle,
			DisplayType = DisplayType.Image)
		]
		public void Document_AddStatic(ActionParam Param)
		{
			switch (Param.Type)
			{
				case ActionParam.QueryType.Invoke:
					AddControl(typeof(WUIStatic));
					break;

				case ActionParam.QueryType.GetState:
					Param.State.Checked = (IsAboutToAdd || IsAdding) && ControlToAdd == typeof(WUIStatic);
					break;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		[ActionProp("Document.AddEditor",
			Caption = "Add editor",
			IconName = "Graphics.Icons.editor.png",
			Type = ActionType.Toggle,
			DisplayType = DisplayType.Image)
		]
		public void Document_AddEditor(ActionParam Param)
		{
			switch (Param.Type)
			{
				case ActionParam.QueryType.Invoke:
					AddControl(typeof(WUIEdit));
					break;

				case ActionParam.QueryType.GetState:
					Param.State.Checked = (IsAboutToAdd || IsAdding) && ControlToAdd == typeof(WUIEdit);
					break;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		[ActionProp("Document.AddEntity",
			Caption = "Add entity container",
			IconName = "Graphics.Icons.entity.png",
			Type = ActionType.Toggle,
			DisplayType = DisplayType.Image)
		]
		public void Document_AddEntity(ActionParam Param)
		{
			switch (Param.Type)
			{
				case ActionParam.QueryType.Invoke:
					AddControl(typeof(WUIEntity));
					break;

				case ActionParam.QueryType.GetState:
					Param.State.Checked = (IsAboutToAdd || IsAdding) && ControlToAdd == typeof(WUIEntity);
					break;
			}
		}


		//////////////////////////////////////////////////////////////////////////
		[ActionProp("Document.DeleteItem",
			Caption = "Delete",
			IconName = "Graphics.Icons.delete.png",
			Type = ActionType.Button,
			DisplayType = DisplayType.Image)
		]
		public void Document_DeleteItem(ActionParam Param)
		{
			switch (Param.Type)
			{
				case ActionParam.QueryType.Invoke:
					DeleteSelectedItems();
					break;

				case ActionParam.QueryType.GetState:
					Param.State.Enabled = CanDeleteSelectedItems();
					break;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		[ActionProp("Edit.Cut",
			Caption = "Cut",
			IconName = "Graphics.Icons.cut.png",
			Type = ActionType.Button,
			DisplayType = DisplayType.Image,
			ShortcutKeys = Keys.Control | Keys.X)
		]
		public void Document_Cut(ActionParam Param)
		{
			switch (Param.Type)
			{
				case ActionParam.QueryType.Invoke:
					CopySelectedItems();
					DeleteSelectedItems();
					break;

				case ActionParam.QueryType.GetState:
					Param.State.Enabled = SelectedControls.Length > 0 && CanDeleteSelectedItems();
					break;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		[ActionProp("Edit.Copy",
			Caption = "Copy",
			IconName = "Graphics.Icons.copy.png",
			Type = ActionType.Button,
			DisplayType = DisplayType.Image,
			ShortcutKeys = Keys.Control | Keys.C)
		]
		public void Document_Copy(ActionParam Param)
		{
			switch (Param.Type)
			{
				case ActionParam.QueryType.Invoke:
					CopySelectedItems();
					break;

				case ActionParam.QueryType.GetState:
					Param.State.Enabled = SelectedControls.Length > 0;
					break;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		[ActionProp("Edit.Paste",
			Caption = "Paste",
			IconName = "Graphics.Icons.paste.png",
			Type = ActionType.Button,
			DisplayType = DisplayType.Image,
			ShortcutKeys = Keys.Control | Keys.V)
		]
		public void Document_Paste(ActionParam Param)
		{
			switch (Param.Type)
			{
				case ActionParam.QueryType.Invoke:
					PasteItems();
					break;

				case ActionParam.QueryType.GetState:
					Param.State.Enabled = Clipboard.ContainsData(ClipFormat);
					break;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		[ActionProp("Edit.Undo",
			Caption = "Undo",
			IconName = "Graphics.Icons.undo.png",
			Type = ActionType.Button,
			DisplayType = DisplayType.Image,
			ShortcutKeys = Keys.Control | Keys.Z)
		]
		public void Document_Undo(ActionParam Param)
		{
			switch (Param.Type)
			{
				case ActionParam.QueryType.Invoke:
					string Str = GetUndo();
					if(Str != null)
					{
						using(new HourGlass())
							LoadUndoState(Str);
					}
					break;

				case ActionParam.QueryType.GetState:
					Param.State.Enabled = UndoAvailable;
					Param.State.Caption = "Undo";
					if (Param.State.Enabled) Param.State.Caption += ": " + UndoName;

					break;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		[ActionProp("Edit.Redo",
			Caption = "Redo",
			IconName = "Graphics.Icons.redo.png",
			Type = ActionType.Button,
			DisplayType = DisplayType.Image,
			ShortcutKeys = Keys.Control | Keys.Y)
		]
		public void Document_Redo(ActionParam Param)
		{
			switch (Param.Type)
			{
				case ActionParam.QueryType.Invoke:
					string Str = GetRedo();
					if (Str != null)
					{
						using (new HourGlass())
							LoadUndoState(Str);
					}
					break;

				case ActionParam.QueryType.GetState:
					Param.State.Enabled = RedoAvailable;

					Param.State.Caption = "Redo";
					if (Param.State.Enabled) Param.State.Caption += ": " + RedoName;

					break;
			}
		}

	}
}
