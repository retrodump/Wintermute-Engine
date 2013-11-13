// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using DeadCode.WME.Core;
using DeadCode.WME.Global;
using System.Drawing;
using System.ComponentModel;
using Design;
using System.Drawing.Design;
using DeadCode.WME.Global.UITypeEditors;

namespace DeadCode.WME.WindowEdit
{
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public class UiEditorProxy : UiControlProxy
	{
		//////////////////////////////////////////////////////////////////////////
		public UiEditorProxy(WUIEdit NativeObject) : base(NativeObject)
		{
		}

		//////////////////////////////////////////////////////////////////////////
		[Browsable(false)]
		public new WUIEdit NativeObject
		{
			get
			{
				return (WUIEdit)_NativeObject;
			}
		}

		[Category(CategoryName.Appearance), PropertyOrder(150)]
		[Description("Specifies a caption of the editor. It's not directly visible and is typically used by scripts to display a floating 'tooltip'.")]
		//////////////////////////////////////////////////////////////////////////
		public string Caption
		{
			get
			{
				return NativeObject.Caption;
			}
			set
			{
				OnPropertyChanging("Caption");
				NativeObject.Caption = value;
			}
		}

		[Category(CategoryName.Appearance), PropertyOrder(201)]
		[Description("The font to be used for selected text")]
		[EditorAttribute(typeof(FontTypeEditor), typeof(UITypeEditor))]
		//////////////////////////////////////////////////////////////////////////
		public WFont FontSelected
		{
			get
			{
				return NativeObject.FontSelected;
			}
			set
			{
				OnPropertyChanging("FontSelected");
				NativeObject.FontSelected = value;
			}
		}


		[Category(CategoryName.Appearance), PropertyOrder(300)]
		[Description("Specifies the width/height of the frame around text")]
		//////////////////////////////////////////////////////////////////////////
		public int FrameWidth
		{
			get
			{
				return NativeObject.FrameWidth;
			}
			set
			{
				OnPropertyChanging("FrameWidth");
				NativeObject.FrameWidth = value;
			}
		}

		[Category(CategoryName.Appearance), PropertyOrder(310)]
		[Description("Specifies the maximum length of text")]
		//////////////////////////////////////////////////////////////////////////
		public int MaxLength
		{
			get
			{
				return NativeObject.MaxLength;
			}
			set
			{
				OnPropertyChanging("MaxLength");
				NativeObject.MaxLength = value;
			}
		}

		[Category(CategoryName.Appearance), PropertyOrder(320)]
		[Description("Specifies the blinking rate of text cursor in milliseconds")]
		//////////////////////////////////////////////////////////////////////////
		public int CursorBlinkRate
		{
			get
			{
				return NativeObject.CursorBlinkRate;
			}
			set
			{
				OnPropertyChanging("CursorBlinkRate");
				NativeObject.CursorBlinkRate = value;
			}
		}

	}
}
