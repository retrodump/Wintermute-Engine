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
	public class UiEntityProxy : UiProxy
	{
		//////////////////////////////////////////////////////////////////////////
		public UiEntityProxy(WUIEntity NativeObject) : base(NativeObject)
		{
		}

		//////////////////////////////////////////////////////////////////////////
		[Browsable(false)]
		public new WUIEntity NativeObject
		{
			get
			{
				return (WUIEntity)_NativeObject;
			}
		}

		[Category(CategoryName.Layout)]
		[ReadOnly(true)]
		//////////////////////////////////////////////////////////////////////////
		public new int Width
		{
			get
			{
				return NativeObject.Width;
			}
		}

		[Category(CategoryName.Layout)]
		[ReadOnly(true)]
		//////////////////////////////////////////////////////////////////////////
		public new int Height
		{
			get
			{
				return NativeObject.Width;
			}
		}

		[Category(CategoryName.Appearance), PropertyOrder(10)]
		[Description("The entity assigned to this entity container")]
		[EditorAttribute(typeof(EntityTypeEditor), typeof(UITypeEditor))]
		//////////////////////////////////////////////////////////////////////////
		public WAdEntity Entity
		{
			get
			{
				return NativeObject.Entity;
			}
			set
			{
				OnPropertyChanging("Entity");
				NativeObject.Entity = value;
			}
		}
	}

}
