// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using DeadCode.WME.Core;

namespace DeadCode.WME.WindowEdit
{
	public partial class WindowDoc
	{
		private Size GridSize
		{
			get
			{
				if (ParentForm as FrmMain != null) return ((FrmMain)ParentForm).GridSize;
				else return new Size(0, 0);
			}
		}

		private Color SelectionColor
		{
			get
			{
				if (ParentForm as FrmMain != null) return ((FrmMain)ParentForm).SelectionColor;
				else return Color.Red;
			}
		}
		private Color BoxColor
		{
			get
			{
				if (ParentForm as FrmMain != null) return ((FrmMain)ParentForm).BoxColor;
				else return Color.FromArgb(100, 0, 0, 255);
			}
		}


		private HotSpot CurrentSpot = null;
		private bool IsResizing = false;
		private bool IsMoving = false;
		private bool IsAdding = false;
		private bool IsAboutToAdd = false;
		private bool UndoPending = false;
		private Point StartPoint;
		private Type ControlToAdd = null;
		//////////////////////////////////////////////////////////////////////////
		private void OnMouseDown(object sender, MouseEventArgs e)
		{
			if (Canvas == null) return;

			if(e.Button==MouseButtons.Right)
			{
				CancelResizeMove();
				return;
			}

			if (e.Button == MouseButtons.Left)
			{
				if (IsAboutToAdd)
				{
					DeselectAll();
					IsAboutToAdd = false;
					IsAdding = true;
					StartPoint = new Point(e.X, e.Y);
					
					if (GridSize.Width > 0)
						StartPoint.X -= ((StartPoint.X + Canvas.HorizontalScroll.Value) % GridSize.Width);
					if (GridSize.Height > 0)
						StartPoint.Y -= ((StartPoint.Y + Canvas.VerticalScroll.Value) % GridSize.Height);
					
					AddingRect = Rectangle.Empty;
					return;
				}

				CurrentSpot = null;
				for (int i = HotSpots.Count - 1; i >= 0; i--)
				{
					HotSpot Spot = HotSpots[i];
					if (Spot.Rect.Contains(new Point(e.X, e.Y)))
					{
						CurrentSpot = Spot;
						break;
					}
				}
				if (CurrentSpot != null && CurrentSpot.Owner != null)
				{
					bool IsControl = (Control.ModifierKeys & Keys.Control) == Keys.Control;
					bool IsShift = (Control.ModifierKeys & Keys.Shift) == Keys.Shift;

					if (CurrentSpot.IsResize() && e.Button == MouseButtons.Left)
					{
						UndoPending = true;
						IsResizing = true;
					}
					else
					{
						if (IsControl || IsShift)
						{
							CurrentSpot.Owner.EditorSelected = !CurrentSpot.Owner.EditorSelected;
							if (CurrentSpot.Owner.EditorSelected) SetOrigPos(CurrentSpot.Owner);
						}
						else
						{
							if (!CurrentSpot.Owner.EditorSelected)
							{
								if (!(CurrentSpot.Owner is RectangleResizer)) DeselectAll();
								CurrentSpot.Owner.EditorSelected = true;
								SetOrigPos(CurrentSpot.Owner);
							}
						}
						if (CurrentSpot.Owner.EditorSelected && e.Button == MouseButtons.Left)
						{
							if (RectResizer == null || RectResizer.Window != CurrentSpot.Owner as WUIWindow)
							{
								UndoPending = true;
								IsMoving = true;
							}
						}
					}
					StartPoint = new Point(e.X, e.Y);
				}
				else
				{
					DeselectAll();
					RectResizer = null;
				}

				RefreshTree(true);

				// invoke selection changed
				OnSelectionChanged(LayoutTree, new EventArgs());
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnMouseUp(object sender, MouseEventArgs e)
		{
			if (IsAdding)
			{
				CreateControl(ControlToAdd, AddingRect);
			}
			IsAdding = false;
			IsResizing = false;
			IsMoving = false;
			CurrentSpot = null;
			UndoPending = false;

			UpdateScrollSize();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnMouseMove(object sender, MouseEventArgs e)
		{
			if (Canvas == null) return;

			if (IsAdding || IsAboutToAdd)
			{
				Canvas.Cursor = Cursors.Cross;
				if (IsAdding)
				{
					Point EndPoint = new Point(e.X, e.Y);
					if (GridSize.Width > 0)
						EndPoint.X -= ((EndPoint.X + Canvas.HorizontalScroll.Value) % GridSize.Width);
					if (GridSize.Height > 0)
						EndPoint.Y -= ((EndPoint.Y + Canvas.VerticalScroll.Value) % GridSize.Height);

					AddingRect = new Rectangle(StartPoint.X, StartPoint.Y, EndPoint.X - StartPoint.X, EndPoint.Y - StartPoint.Y);
					UpdateScrollSize();
				}
			}
			else if (IsMoving)
			{
				int MoveX = e.X - StartPoint.X;
				int MoveY = e.Y - StartPoint.Y;

				if (MoveX != 0 || MoveY != 0)
				{
					if (UndoPending)
					{
						SaveUndoState("Move control");
						UndoPending = false;
					}

					foreach (IEditorResizable It in SelectedItems)
					{
						if (RectResizer != null && !(It is RectangleResizer)) continue;

						// check if some of the parents is selected
						if (It is WUIObject)
						{
							bool IsParentSelected = false;
							WUIObject Parent = ((WUIObject)It).Parent;
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
						}

						It.X = It.OrigRect.X + MoveX;
						It.Y = It.OrigRect.Y + MoveY;

						if (GridSize.Width > 0)
							It.X -= (It.X % GridSize.Width);
						if (GridSize.Height > 0)
							It.Y -= (It.Y % GridSize.Height);
					}
					RefreshProps = true;
					UpdateScrollSize();
				}
			}
			else if (IsResizing)
			{
				int DeltaX = e.X - StartPoint.X;
				int DeltaY = e.Y - StartPoint.Y;

				if (DeltaX != 0 || DeltaY != 0)
				{
					if (UndoPending)
					{
						SaveUndoState("Resize control");
						UndoPending = false;
					}

					foreach (IEditorResizable It in SelectedItems)
					{
						if (RectResizer != null && !(It is RectangleResizer)) continue;


						bool ChangeX = false;
						bool ChangeY = false;
						bool ChangeWidth = false;
						bool ChangeHeight = false;

						switch (CurrentSpot.Type)
						{
							case HotSpot.HotSpotType.ResizeE:
								It.Width = It.OrigRect.Width + DeltaX;
								ChangeWidth = true;
								break;
							case HotSpot.HotSpotType.ResizeW:
								It.X = It.OrigRect.X + DeltaX;
								ChangeX = true;
								It.Width = It.OrigRect.Width - DeltaX;
								ChangeWidth = true;
								break;
							case HotSpot.HotSpotType.ResizeS:
								It.Height = It.OrigRect.Height + DeltaY;
								ChangeHeight = true;
								break;
							case HotSpot.HotSpotType.ResizeN:
								It.Y = It.OrigRect.Y + DeltaY;
								ChangeY = true;
								It.Height = It.OrigRect.Height - DeltaY;
								ChangeHeight = true;
								break;
							case HotSpot.HotSpotType.ResizeSE:
								It.Width = It.OrigRect.Width + DeltaX;
								ChangeWidth = true;
								It.Height = It.OrigRect.Height + DeltaY;
								ChangeHeight = true;
								break;
							case HotSpot.HotSpotType.ResizeSW:
								It.X = It.OrigRect.X + DeltaX;
								ChangeX = true;
								It.Width = It.OrigRect.Width - DeltaX;
								ChangeWidth = true;
								It.Height = It.OrigRect.Height + DeltaY;
								ChangeHeight = true;
								break;
							case HotSpot.HotSpotType.ResizeNE:
								It.Width = It.OrigRect.Width + DeltaX;
								ChangeWidth = true;
								It.Y = It.OrigRect.Y + DeltaY;
								ChangeY = true;
								It.Height = It.OrigRect.Height - DeltaY;
								ChangeHeight = true;
								break;
							case HotSpot.HotSpotType.ResizeNW:
								It.X = It.OrigRect.X + DeltaX;
								ChangeX = true;
								It.Width = It.OrigRect.Width - DeltaX;
								ChangeWidth = true;
								It.Y = It.OrigRect.Y + DeltaY;
								ChangeY = true;
								It.Height = It.OrigRect.Height - DeltaY;
								ChangeHeight = true;
								break;
						}
						if (ChangeWidth)
						{
							int OrigWidth = It.Width;
							if (GridSize.Width > 0)
							{
								It.Width -= It.Width % GridSize.Width;
								It.Width = Math.Max(It.Width, GridSize.Width);
							}
							else It.Width = Math.Max(It.Width, 10);

							if (ChangeX) It.X += OrigWidth - It.Width;
						}
						if (ChangeHeight)
						{
							int OrigHeight = It.Height;
							if (GridSize.Height > 0)
							{
								It.Height -= It.Height % GridSize.Height;
								It.Height = Math.Max(It.Height, GridSize.Height);
							}
							else It.Width = Math.Max(It.Height, 10);

							if (ChangeY) It.Y += OrigHeight - It.Height;
						}
					}

					RefreshProps = true;
					UpdateScrollSize();
				}
			}
			else
			{
				for (int i = HotSpots.Count - 1; i >= 0; i--)
				{
					HotSpot Spot = HotSpots[i];
					if (Spot.Rect.Contains(new Point(e.X, e.Y)))
					{
						switch (Spot.Type)
						{
							case HotSpot.HotSpotType.Container:
								Canvas.Cursor = Cursors.Default;
								break;
							case HotSpot.HotSpotType.Control:
								Canvas.Cursor = Cursors.SizeAll;
								break;
							case HotSpot.HotSpotType.ResizeE:
							case HotSpot.HotSpotType.ResizeW:
								Canvas.Cursor = Cursors.SizeWE;
								break;
							case HotSpot.HotSpotType.ResizeN:
							case HotSpot.HotSpotType.ResizeS:
								Canvas.Cursor = Cursors.SizeNS;
								break;
							case HotSpot.HotSpotType.ResizeNE:
							case HotSpot.HotSpotType.ResizeSW:
								Canvas.Cursor = Cursors.SizeNESW;
								break;
							case HotSpot.HotSpotType.ResizeNW:
							case HotSpot.HotSpotType.ResizeSE:
								Canvas.Cursor = Cursors.SizeNWSE;
								break;
						}
						return;
					}
				}
				Canvas.Cursor = Cursors.Default;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnKeyDown(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.Delete)
			{
				DeleteSelectedItems();
				return;
			}
			else if (e.KeyCode == Keys.Escape)
			{
				CancelResizeMove();
			}

			// handle arrow keys
			int MoveX = 0;
			int MoveY = 0;
			int ResizeX = 0;
			int ResizeY = 0;

			int Step = e.Control ? 5 : 1;

			if (e.KeyCode == Keys.Left)
			{
				if (e.Shift) ResizeX = -Step;
				else MoveX = -Step;
			}
			else if (e.KeyCode == Keys.Right)
			{
				if (e.Shift) ResizeX = Step;
				else MoveX = Step;
			}
			else if (e.KeyCode == Keys.Up)
			{
				if (e.Shift) ResizeY = -Step;
				else MoveY = -Step;
			}
			else if (e.KeyCode == Keys.Down)
			{
				if (e.Shift) ResizeY = Step;
				else MoveY = Step;
			}

			// keyboard movement
			if(MoveX != 0 || MoveY != 0)
			{
				if (SelectedItems.Length > 0) SaveUndoState("Keyboard move");
				foreach(IEditorResizable It in SelectedItems)
				{
					if (RectResizer != null && !(It is RectangleResizer)) continue;

					// check if some of the parents is selected
					if (It is WUIObject)
					{
						bool IsParentSelected = false;
						WUIObject Parent = ((WUIObject)It).Parent;
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
					}

					It.X += MoveX;
					It.Y += MoveY;
				}
				RefreshProps = true;
				UpdateScrollSize();
			}

			// keyboard resize
			if (ResizeX != 0 || ResizeY != 0)
			{
				if (SelectedItems.Length > 0) SaveUndoState("Keyboard resize");
				foreach (IEditorResizable It in SelectedItems)
				{
					if (RectResizer != null && !(It is RectangleResizer)) continue;
					if (It is WUIEntity) continue;

					It.Width += ResizeX;
					It.Height += ResizeY;

					if (It.Width < 1) It.Width = 1;
					if (It.Height < 1) It.Height = 1;
				}
				RefreshProps = true;
				UpdateScrollSize();
			}

		}

		//////////////////////////////////////////////////////////////////////////
		private void CancelResizeMove()
		{
			if(IsMoving || IsResizing)
			{
				foreach (IEditorResizable It in SelectedItems)
				{
					if (RectResizer != null && !(It is RectangleResizer)) continue;

					It.X = It.OrigRect.X;
					It.Y = It.OrigRect.Y;
					It.Width = It.OrigRect.Width;
					It.Height = It.OrigRect.Height;
				}
			}
			IsMoving = false;
			IsResizing = false;
			IsAdding = false;
			IsAboutToAdd = false;
			UndoPending = false;

			RefreshProps = true;
			UpdateScrollSize();
		}
	}
}
