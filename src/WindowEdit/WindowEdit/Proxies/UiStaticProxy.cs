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
	public class UiStaticProxy : UiControlProxy
	{
		//////////////////////////////////////////////////////////////////////////
		public UiStaticProxy(WUIStatic NativeObject) : base(NativeObject)
		{
		}

		//////////////////////////////////////////////////////////////////////////
		[Browsable(false)]
		public new WUIStatic NativeObject
		{
			get
			{
				return (WUIStatic)_NativeObject;
			}
		}


		[Category(CategoryName.Appearance), PropertyOrder(300)]
		[Description("Specifies the horizontal alignment of text")]
		//////////////////////////////////////////////////////////////////////////
		public WETextAlign TextAlign
		{
			get
			{
				return NativeObject.TextAlignment;
			}
			set
			{
				OnPropertyChanging("TextAlign");
				NativeObject.TextAlignment = value;
			}
		}

		[Category(CategoryName.Appearance), PropertyOrder(310)]
		[Description("Specifies the vertical alignment of text")]
		//////////////////////////////////////////////////////////////////////////
		public WEVerticalAlign VerticalAlign
		{
			get
			{
				return NativeObject.VerticalAlignment;
			}
			set
			{
				OnPropertyChanging("VerticalAlign");
				NativeObject.VerticalAlignment = value;
			}
		}

	}
}
