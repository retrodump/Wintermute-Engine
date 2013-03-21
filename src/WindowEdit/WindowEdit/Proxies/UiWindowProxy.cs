// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using DeadCode.WME.Core;
using System.Drawing;
using System.ComponentModel;
using Design;
using System.Drawing.Design;
using DeadCode.WME.Global;
using DeadCode.WME.Global.UITypeEditors;

namespace DeadCode.WME.WindowEdit
{
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public class UiWindowProxy : UiControlProxy
	{
		//////////////////////////////////////////////////////////////////////////
		public UiWindowProxy(WUIWindow NativeObject) : base(NativeObject)
		{
		}

		//////////////////////////////////////////////////////////////////////////
		[Browsable(false)]
		public new WUIWindow NativeObject
		{
			get
			{
				return (WUIWindow)_NativeObject;
			}
		}


		[Category(CategoryName.Appearance), PropertyOrder(201)]
		[Description("The font used for inactive window")]
		[EditorAttribute(typeof(FontTypeEditor), typeof(UITypeEditor))]
		//////////////////////////////////////////////////////////////////////////
		public WFont FontInactive
		{
			get
			{
				return NativeObject.FontInactive;
			}
			set
			{
				OnPropertyChanging("FontInactive");
				NativeObject.FontInactive = value;
			}
		}


		[Category(CategoryName.Appearance), PropertyOrder(221)]
		[Description("The background image used for inactive window")]
		[EditorAttribute(typeof(SpriteTypeEditor), typeof(UITypeEditor))]
		//////////////////////////////////////////////////////////////////////////
		public WSprite ImageInactive
		{
			get
			{
				return NativeObject.ImageInactive;
			}
			set
			{
				OnPropertyChanging("ImageInactive");
				NativeObject.ImageInactive = value;
			}
		}

		[Category(CategoryName.Appearance), PropertyOrder(231)]
		[Description("The background tiled image used for inactive window")]
		[EditorAttribute(typeof(TiledImgTypeEditor), typeof(UITypeEditor))]
		//////////////////////////////////////////////////////////////////////////
		public WUITiledImage TiledImageInactive
		{
			get
			{
				return NativeObject.BackInactive;
			}
			set
			{
				OnPropertyChanging("TiledImageInactive");
				NativeObject.BackInactive = value;
			}
		}

		[Category(CategoryName.Appearance), PropertyOrder(300)]
		[Description("Specifies the alignment of the window caption")]
		//////////////////////////////////////////////////////////////////////////
		public WETextAlign TextAlign
		{
			get
			{
				return NativeObject.TitleAlignment;
			}
			set
			{
				OnPropertyChanging("TextAlign");
				NativeObject.TitleAlignment = value;
			}
		}


		[Category(CategoryName.Appearance), PropertyOrder(310)]
		[Description("Indicates whether any content outside the window is clipped off")]
		//////////////////////////////////////////////////////////////////////////
		public bool ClipContents
		{
			get
			{
				return NativeObject.ClipContents;
			}
			set
			{
				OnPropertyChanging("ClipContents");
				NativeObject.ClipContents = value;
			}
		}

		[Category(CategoryName.Appearance), PropertyOrder(320)]
		[Description("The color tint of the window")]
		//////////////////////////////////////////////////////////////////////////
		public Color AlphaColor
		{
			get
			{
				return NativeObject.Color;
			}
			set
			{
				OnPropertyChanging("AlphaColor");
				NativeObject.Color = value;
			}
		}

		[Category(CategoryName.Appearance), PropertyOrder(330)]
		[Description("The color tint to be applied to the background if window is displayed")]
		[Editor(typeof(ColorEditorEx), typeof(UITypeEditor))]
		//////////////////////////////////////////////////////////////////////////
		public Color FadeColor
		{
			get
			{
				return NativeObject.FadeColor;
			}
			set
			{
				OnPropertyChanging("FadeColor");
				NativeObject.FadeColor = value;
			}
		}



		[Category(CategoryName.Layout), PropertyOrder(100)]
		[Description("The area where the window caption is displayed")]
		//////////////////////////////////////////////////////////////////////////
		public Rectangle TitleRectangle
		{
			get
			{
				return NativeObject.TitleRectangle;
			}
			set
			{
				OnPropertyChanging("TitleRectangle");
				NativeObject.TitleRectangle = value;
			}
		}

		[Category(CategoryName.Layout), PropertyOrder(110)]
		[Description("The area which can be used to drag the window")]
		//////////////////////////////////////////////////////////////////////////
		public Rectangle DragRectangle
		{
			get
			{
				return NativeObject.DragRectangle;
			}
			set
			{
				OnPropertyChanging("DragRectangle");
				NativeObject.DragRectangle = value;
			}
		}

		[Category(CategoryName.Behavior), PropertyOrder(200)]
		[Description("Specifies that the window appears to be 'transparent' to mouse clicks")]
		//////////////////////////////////////////////////////////////////////////
		public bool Transparent
		{
			get
			{
				return NativeObject.Transparent;
			}
			set
			{
				OnPropertyChanging("Transparent");
				NativeObject.Transparent = value;
			}
		}

		[Category(CategoryName.Behavior), PropertyOrder(210)]
		[Description("Specifies that the window hides when the user clicks anywhere outside the window")]
		//////////////////////////////////////////////////////////////////////////
		public bool Menu
		{
			get
			{
				return NativeObject.IsMenu;
			}
			set
			{
				OnPropertyChanging("Menu");
				NativeObject.IsMenu = value;
			}
		}

		[Category(CategoryName.Behavior), PropertyOrder(220)]
		[Description("Specifies that the window is painted as in-game (before the inventory and other GUI)")]
		//////////////////////////////////////////////////////////////////////////
		public bool InGame
		{
			get
			{
				return NativeObject.InGame;
			}
			set
			{
				OnPropertyChanging("InGame");
				NativeObject.InGame = value;
			}
		}

		[Category(CategoryName.Behavior), PropertyOrder(230)]
		[Description("Specifies whether the window pauses music when running in system exclusive mode")]
		//////////////////////////////////////////////////////////////////////////
		public bool PauseMusic
		{
			get
			{
				return NativeObject.PauseMusic;
			}
			set
			{
				OnPropertyChanging("PauseMusic");
				NativeObject.PauseMusic = value;
			}
		}

	}

}
