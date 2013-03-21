// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using DeadCode.WME.Core;
using System.Drawing;
using System.ComponentModel;
using System.Drawing.Design;
using Design;
using System.Windows.Forms.Design;
using DeadCode.WME.Global.UITypeEditors;
using DeadCode.WME.Global;

namespace DeadCode.WME.WindowEdit
{
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public class UiProxy : NativePropProxy
	{
		//////////////////////////////////////////////////////////////////////////
		public event EventHandler RefreshNeeded;
		protected void OnRefreshNeeded()
		{
			if (RefreshNeeded != null) RefreshNeeded(this, new EventArgs());
		}

		//////////////////////////////////////////////////////////////////////////
		public UiProxy(WUIObject NativeObject):base(NativeObject)
		{
			_NativeObject = NativeObject;
		}
		
		//////////////////////////////////////////////////////////////////////////
		[Browsable(false)]
		public new WUIObject NativeObject
		{
			get
			{
				return (WUIObject)_NativeObject;
			}
		}

		[Category(CategoryName.Layout), PropertyOrder(10)]
		[Description("The X coordinate of the upper left corner of the control")]
		//////////////////////////////////////////////////////////////////////////
		public int X
		{
			get
			{
				return NativeObject.X;
			}
			set
			{
				OnPropertyChanging("X");
				NativeObject.X = value;
				OnPropertyChanged("X");
			}
		}

		[Category(CategoryName.Layout), PropertyOrder(20)]
		[Description("The Y coordinate of the upper left corner of the control")]
		//////////////////////////////////////////////////////////////////////////
		public int Y
		{
			get
			{
				return NativeObject.Y;
			}
			set
			{
				OnPropertyChanging("Y");
				NativeObject.Y = value;
				OnPropertyChanged("Y");
			}
		}

		[Category(CategoryName.Layout), PropertyOrder(30)]
		[Description("Width of the control")]
		//////////////////////////////////////////////////////////////////////////
		public int Width
		{
			get
			{
				return NativeObject.Width;
			}
			set
			{
				OnPropertyChanging("Width");
				NativeObject.Width = value;
				OnPropertyChanged("Width");
			}
		}

		[Category(CategoryName.Layout), PropertyOrder(40)]
		[Description("Height of the control")]
		//////////////////////////////////////////////////////////////////////////
		public int Height
		{
			get
			{
				return NativeObject.Height;
			}
			set
			{
				OnPropertyChanging("Height");
				NativeObject.Height = value;
				OnPropertyChanged("Height");
			}
		}

		[Category(CategoryName.Appearance), PropertyOrder(1000)]
		[Description("Specifies whether the control is disabled")]
		//////////////////////////////////////////////////////////////////////////
		public bool Disabled
		{
			get
			{
				return NativeObject.Disabled;
			}
			set
			{
				OnPropertyChanging("Disabled");
				NativeObject.Disabled = value;
			}
		}

		[Category(CategoryName.Appearance), PropertyOrder(1110)]
		[Description("Specifies whether the control is visible")]
		//////////////////////////////////////////////////////////////////////////
		public bool Visible
		{
			get
			{
				return NativeObject.Visible;
			}
			set
			{
				OnPropertyChanging("Visible");
				NativeObject.Visible = value;
			}
		}

		[Category(CategoryName.General), PropertyOrder(100)]
		[Description("The internal name of the control")]
		//////////////////////////////////////////////////////////////////////////
		public string Name
		{
			get
			{
				return NativeObject.Name;
			}
			set
			{
				OnPropertyChanging("Name");
				NativeObject.Name = value;
				OnRefreshNeeded();				
			}
		}
	}


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public class UiControlProxy : UiProxy
	{
		public UiControlProxy(WUIObject NativeObject) : base(NativeObject)
		{

		}

		[Category(CategoryName.Behavior), PropertyOrder(110)]
		[Description("Specifies whether the control sends events to its parent window")]
		//////////////////////////////////////////////////////////////////////////
		public bool ParentNotify
		{
			get
			{
				return NativeObject.ParentNotify;
			}
			set
			{
				OnPropertyChanging("ParentNotify");
				NativeObject.ParentNotify = value;
			}
		}


		[Category(CategoryName.Appearance), PropertyOrder(100)]
		[Description("The text of the control")]
		//////////////////////////////////////////////////////////////////////////
		public string Text
		{
			get
			{
				if (NativeObject.Text != null)
					return NativeObject.Text.Replace("\n", "|");
				else
					return NativeObject.Text;
			}
			set
			{
				OnPropertyChanging("Text");
				NativeObject.Text = value;
				OnRefreshNeeded();
			}
		}

		[Category(CategoryName.Appearance), PropertyOrder(200)]
		[Description("The font")]
		[EditorAttribute(typeof(FontTypeEditor), typeof(UITypeEditor))]
		//////////////////////////////////////////////////////////////////////////
		public WFont Font
		{
			get
			{
				return NativeObject.Font;
			}
			set
			{
				OnPropertyChanging("Font");
				NativeObject.Font = value;
			}
		}

		[Category(CategoryName.Appearance), PropertyOrder(210)]
		[Description("The mouse pointer to be displayed over this control")]
		[EditorAttribute(typeof(SpriteTypeEditor), typeof(UITypeEditor))]
		//////////////////////////////////////////////////////////////////////////
		public WSprite Cursor
		{
			get
			{
				return NativeObject.Cursor;
			}
			set
			{
				OnPropertyChanging("Cursor");
				NativeObject.Cursor = value;
			}
		}

		[Category(CategoryName.Appearance), PropertyOrder(220)]
		[Description("The background image")]
		[EditorAttribute(typeof(SpriteTypeEditor), typeof(UITypeEditor))]
		//////////////////////////////////////////////////////////////////////////
		public WSprite Image
		{
			get
			{
				return NativeObject.Image;
			}
			set
			{
				OnPropertyChanging("Image");
				NativeObject.Image = value;
			}
		}

		[Category(CategoryName.Appearance), PropertyOrder(230)]
		[Description("The background tiled image")]
		[EditorAttribute(typeof(TiledImgTypeEditor), typeof(UITypeEditor))]
		//////////////////////////////////////////////////////////////////////////
		public WUITiledImage TiledImage
		{
			get
			{
				return NativeObject.Back;
			}
			set
			{
				OnPropertyChanging("TiledImage");
				NativeObject.Back = value;
			}
		}

	}


	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public class InvBoxProxy : NativePropProxy
	{
		//////////////////////////////////////////////////////////////////////////
		public InvBoxProxy(WAdInventoryBox NativeObject)
			: base(NativeObject)
		{
			_NativeObject = NativeObject;
		}

		//////////////////////////////////////////////////////////////////////////
		[Browsable(false)]
		public new WAdInventoryBox NativeObject
		{
			get
			{
				return (WAdInventoryBox)_NativeObject;
			}
		}

		[Category(CategoryName.Behavior), PropertyOrder(200)]
		[Description("Specifies whether the inventory hides the currently selected item")]
		//////////////////////////////////////////////////////////////////////////
		public bool HideSelected
		{
			get
			{
				return NativeObject.HideSelected;
			}
			set
			{
				OnPropertyChanging("HideSelected");
				NativeObject.HideSelected = value;
			}
		}

		[Category(CategoryName.Behavior), PropertyOrder(210)]
		[Description("Specifies whether the inventory is displayed as an exclusive window (clicking outside the inventory window closes it)")]
		//////////////////////////////////////////////////////////////////////////
		public bool Exclusive
		{
			get
			{
				return NativeObject.Exclusive;
			}
			set
			{
				OnPropertyChanging("Exclusive");
				NativeObject.Exclusive = value;
			}
		}

		[Category(CategoryName.Behavior), PropertyOrder(220)]
		[Description("Specifies by how many items the inventory contents scroll")]
		//////////////////////////////////////////////////////////////////////////
		public int ScrollBy
		{
			get
			{
				return NativeObject.ScrollBy;
			}
			set
			{
				OnPropertyChanging("ScrollBy");
				NativeObject.ScrollBy = value;
			}
		}


		[Category(CategoryName.Layout), PropertyOrder(10)]
		[Description("The area where inventory items are displayed")]
		//////////////////////////////////////////////////////////////////////////
		public Rectangle Area
		{
			get
			{
				return NativeObject.Area;
			}
			set
			{
				OnPropertyChanging("Area");
				NativeObject.Area = value;
				OnPropertyChanged("Area");
			}
		}

		[Category(CategoryName.Layout), PropertyOrder(20)]
		[Description("Item spacing in pixels")]
		//////////////////////////////////////////////////////////////////////////
		public int Spacing
		{
			get
			{
				return NativeObject.Spacing;
			}
			set
			{
				OnPropertyChanging("Spacing");
				NativeObject.Spacing = value;
			}
		}

		[Category(CategoryName.Layout), PropertyOrder(30)]
		[Description("The width of the item cell")]
		//////////////////////////////////////////////////////////////////////////
		public int ItemWidth
		{
			get
			{
				return NativeObject.ItemWidth;
			}
			set
			{
				OnPropertyChanging("ItemWidth");
				NativeObject.ItemWidth = value;
			}
		}

		[Category(CategoryName.Layout), PropertyOrder(40)]
		[Description("The height of the item cell")]
		//////////////////////////////////////////////////////////////////////////
		public int ItemHeight
		{
			get
			{
				return NativeObject.ItemHeight;
			}
			set
			{
				OnPropertyChanging("ItemHeight");
				NativeObject.ItemHeight = value;
			}
		}

		[Category(CategoryName.Appearance), PropertyOrder(50)]
		[Description("Specifies whether the inventory is initially visible")]
		//////////////////////////////////////////////////////////////////////////
		public bool Visible
		{
			get
			{
				return NativeObject.Visible;
			}
			set
			{
				OnPropertyChanging("Visible");
				NativeObject.Visible = value;
			}
		}

	}


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public class RespBoxProxy : NativePropProxy
	{
		//////////////////////////////////////////////////////////////////////////
		public RespBoxProxy(WAdResponseBox NativeObject) : base(NativeObject)
		{
			_NativeObject = NativeObject;
		}

		//////////////////////////////////////////////////////////////////////////
		[Browsable(false)]
		public new WAdResponseBox NativeObject
		{
			get
			{
				return (WAdResponseBox)_NativeObject;
			}
		}

		[Category(CategoryName.Appearance), PropertyOrder(200)]
		[Description("The font to be used for response lines")]
		[EditorAttribute(typeof(FontTypeEditor), typeof(UITypeEditor))]
		//////////////////////////////////////////////////////////////////////////
		public WFont Font
		{
			get
			{
				return NativeObject.Font;
			}
			set
			{
				OnPropertyChanging("Font");
				NativeObject.Font = value;
			}
		}

		[Category(CategoryName.Appearance), PropertyOrder(201)]
		[Description("The font to be used for highlighted response lines")]
		[EditorAttribute(typeof(FontTypeEditor), typeof(UITypeEditor))]
		//////////////////////////////////////////////////////////////////////////
		public WFont FontHover
		{
			get
			{
				return NativeObject.FontHover;
			}
			set
			{
				OnPropertyChanging("FontHover");
				NativeObject.FontHover = value;
			}
		}

		[Category(CategoryName.Appearance), PropertyOrder(210)]
		[Description("The mouse pointer to be displayed over response")]
		[EditorAttribute(typeof(SpriteTypeEditor), typeof(UITypeEditor))]
		//////////////////////////////////////////////////////////////////////////
		public WSprite Cursor
		{
			get
			{
				return NativeObject.Cursor;
			}
			set
			{
				OnPropertyChanging("Cursor");
				NativeObject.Cursor = value;
			}
		}


		[Category(CategoryName.Appearance)]
		[Description("Specifies whether the responses are stacked horizontally")]
		//////////////////////////////////////////////////////////////////////////
		public bool Horizontal
		{
			get
			{
				return NativeObject.Horizontal;
			}
			set
			{
				OnPropertyChanging("Horizontal");
				NativeObject.Horizontal = value;
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
		[Description("Specifies the vertical alignment of the responses")]
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


		[Category(CategoryName.Layout)]
		[Description("The area where dialogue responses are displayed")]
		//////////////////////////////////////////////////////////////////////////
		public Rectangle Area
		{
			get
			{
				return NativeObject.Area;
			}
			set
			{
				OnPropertyChanging("Area");
				NativeObject.Area = value;
				OnPropertyChanged("Area");
			}
		}

		[Category(CategoryName.Layout)]
		[Description("Response spacing in pixels")]
		//////////////////////////////////////////////////////////////////////////
		public int Spacing
		{
			get
			{
				return NativeObject.Spacing;
			}
			set
			{
				OnPropertyChanging("Spacing");
				NativeObject.Spacing = value;
			}
		}

	}
}
