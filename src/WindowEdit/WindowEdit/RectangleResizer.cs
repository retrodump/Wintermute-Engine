// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using DeadCode.WME.Core;

namespace DeadCode.WME.WindowEdit
{
	public class RectangleResizer : IEditorResizable
	{
		public enum RectType
		{
			Drag, Title, Unknown
		};

		public WUIWindow Window;
		public RectType Type;

		//////////////////////////////////////////////////////////////////////////
		public RectangleResizer(WUIWindow Window, RectType Type)
		{
			this.Window = Window;
			this.Type = Type;

			_OrigRect = GetRect();
		}

		//////////////////////////////////////////////////////////////////////////
		private Rectangle GetRect()
		{
			switch(Type)
			{
				case RectType.Drag:
					return Window.DragRectangle;
				case RectType.Title:
					return Window.TitleRectangle;
			}
			return Rectangle.Empty;
		}

		//////////////////////////////////////////////////////////////////////////
		private void SetRect(Rectangle Rect)
		{
			switch (Type)
			{
				case RectType.Drag:
					Window.DragRectangle = Rect;
					break;
				case RectType.Title:
					Window.TitleRectangle = Rect;
					break;
			}
		}

		private Rectangle _OrigRect;
		//////////////////////////////////////////////////////////////////////////
		public Rectangle OrigRect
		{
			get
			{
				return _OrigRect;
			}
			set
			{
				_OrigRect = value;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public int X
		{
			get
			{
				return GetRect().X;
			}
			set
			{
				Rectangle R = GetRect();
				SetRect(new Rectangle(value, R.Y, R.Width, R.Height));
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public int Y
		{
			get
			{
				return GetRect().Y;
			}
			set
			{
				Rectangle R = GetRect();
				SetRect(new Rectangle(R.X, value, R.Width, R.Height));
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public int Width
		{
			get
			{
				return GetRect().Width;
			}
			set
			{
				Rectangle R = GetRect();
				SetRect(new Rectangle(R.X, R.Y, value, R.Height));
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public int Height
		{
			get
			{
				return GetRect().Height;
			}
			set
			{
				Rectangle R = GetRect();
				SetRect(new Rectangle(R.X, R.Y, R.Width, value));
			}
		}

		private bool _EditorSelected;
		//////////////////////////////////////////////////////////////////////////
		public bool EditorSelected
		{
			get
			{
				return _EditorSelected;
			}
			set
			{
				_EditorSelected = value;
			}
		}
		
	}
}
