// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using DeadCode.WME.Core;
using System.Drawing;

namespace DeadCode.WME.WindowEdit
{
	public class HotSpot
	{
		public enum HotSpotType
		{
			Container,
			Control,
			ResizeE,
			ResizeW,
			ResizeN,
			ResizeS,
			ResizeNW,
			ResizeNE,
			ResizeSW,
			ResizeSE
		}

		public Rectangle Rect;
		public IEditorResizable Owner;
		public HotSpotType Type;

		//////////////////////////////////////////////////////////////////////////
		public HotSpot(Rectangle Rect, IEditorResizable Owner, HotSpotType Type)
		{
			this.Rect = Rect;
			this.Owner = Owner;
			this.Type = Type;
		}

		//////////////////////////////////////////////////////////////////////////
		public bool IsResize()
		{
			return !IsMove();
		}

		//////////////////////////////////////////////////////////////////////////
		public bool IsMove()
		{
			return Type == HotSpotType.Container || Type == HotSpotType.Control;
		}
	}
}
