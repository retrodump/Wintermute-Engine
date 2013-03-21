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
	public class UiButtonProxy : UiControlProxy
	{
		//////////////////////////////////////////////////////////////////////////
		public UiButtonProxy(WUIButton NativeObject) : base(NativeObject)
		{
		}

		//////////////////////////////////////////////////////////////////////////
		[Browsable(false)]
		public new WUIButton NativeObject
		{
			get
			{
				return (WUIButton)_NativeObject;
			}
		}

		[Category(CategoryName.Appearance), PropertyOrder(150)]
		[Description("Specifies a caption of the button. It's not directly visible and is typically used by scripts to display a floating 'tooltip'.")]
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

		[Category(CategoryName.Appearance), PropertyOrder(1120)]
		[Description("Specifies whether the button appears to be pressed down")]
		//////////////////////////////////////////////////////////////////////////
		public bool Pressed
		{
			get
			{
				return NativeObject.StayPressed;
			}
			set
			{
				OnPropertyChanging("Pressed");
				NativeObject.StayPressed = value;
			}
		}

		[Category(CategoryName.Appearance), PropertyOrder(1130)]
		[Description("Specifies whether the image should be centered within the button rectangle")]
		//////////////////////////////////////////////////////////////////////////
		public bool CenterImage
		{
			get
			{
				return NativeObject.CenterImage;
			}
			set
			{
				OnPropertyChanging("CenterImage");
				NativeObject.CenterImage = value;
			}
		}

		[Category(CategoryName.Appearance), PropertyOrder(1140)]
		[Description("Specifies whether the button detects mouse-over with pixel precision")]
		//////////////////////////////////////////////////////////////////////////
		public bool PixelPerfect
		{
			get
			{
				return NativeObject.PixelPerfect;
			}
			set
			{
				OnPropertyChanging("PixelPerfect");
				NativeObject.PixelPerfect = value;
			}
		}

		[Category(CategoryName.Appearance), PropertyOrder(1150)]
		[Description("Specifies whether the button can be focused")]
		//////////////////////////////////////////////////////////////////////////
		public bool Focusable
		{
			get
			{
				return NativeObject.CanFocus;
			}
			set
			{
				OnPropertyChanging("Focusable");
				NativeObject.CanFocus = value;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// pressed
		//////////////////////////////////////////////////////////////////////////
		[Category(CategoryName.AppearancePressed), PropertyOrder(10)]
		[Description("The font used for pressed button")]
		[EditorAttribute(typeof(FontTypeEditor), typeof(UITypeEditor))]
		//////////////////////////////////////////////////////////////////////////
		public WFont FontPressed
		{
			get
			{
				return NativeObject.FontPressed;
			}
			set
			{
				OnPropertyChanging("FontPressed");
				NativeObject.FontPressed = value;
			}
		}

		[Category(CategoryName.AppearancePressed), PropertyOrder(20)]
		[Description("The background image used for pressed button")]
		[EditorAttribute(typeof(SpriteTypeEditor), typeof(UITypeEditor))]
		//////////////////////////////////////////////////////////////////////////
		public WSprite ImagePressed
		{
			get
			{
				return NativeObject.ImagePressed;
			}
			set
			{
				OnPropertyChanging("ImagePressed");
				NativeObject.ImagePressed = value;
			}
		}

		[Category(CategoryName.AppearancePressed), PropertyOrder(30)]
		[Description("The background tiled image used for pressed button")]
		[EditorAttribute(typeof(TiledImgTypeEditor), typeof(UITypeEditor))]
		//////////////////////////////////////////////////////////////////////////
		public WUITiledImage TiledImagePressed
		{
			get
			{
				return NativeObject.BackPressed;
			}
			set
			{
				OnPropertyChanging("TiledImagePressed");
				NativeObject.BackPressed = value;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// hover
		//////////////////////////////////////////////////////////////////////////
		[Category(CategoryName.AppearanceHover), PropertyOrder(10)]
		[Description("The font used for hovered button")]
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

		[Category(CategoryName.AppearanceHover), PropertyOrder(20)]
		[Description("The background image used for hovered button")]
		[EditorAttribute(typeof(SpriteTypeEditor), typeof(UITypeEditor))]
		//////////////////////////////////////////////////////////////////////////
		public WSprite ImageHover
		{
			get
			{
				return NativeObject.ImageHover;
			}
			set
			{
				OnPropertyChanging("ImageHover");
				NativeObject.ImageHover = value;
			}
		}

		[Category(CategoryName.AppearanceHover), PropertyOrder(30)]
		[Description("The background tiled image used for hovered button")]
		[EditorAttribute(typeof(TiledImgTypeEditor), typeof(UITypeEditor))]
		//////////////////////////////////////////////////////////////////////////
		public WUITiledImage TiledImageHover
		{
			get
			{
				return NativeObject.BackHover;
			}
			set
			{
				OnPropertyChanging("TiledImageHover");
				NativeObject.BackHover = value;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// disabled
		//////////////////////////////////////////////////////////////////////////
		[Category(CategoryName.AppearanceDisabled), PropertyOrder(10)]
		[Description("The font used for disabled button")]
		[EditorAttribute(typeof(FontTypeEditor), typeof(UITypeEditor))]
		//////////////////////////////////////////////////////////////////////////
		public WFont FontDisabled
		{
			get
			{
				return NativeObject.FontDisabled;
			}
			set
			{
				OnPropertyChanging("FontDisabled");
				NativeObject.FontDisabled = value;
			}
		}

		[Category(CategoryName.AppearanceDisabled), PropertyOrder(20)]
		[Description("The background image used for disabled button")]
		[EditorAttribute(typeof(SpriteTypeEditor), typeof(UITypeEditor))]
		//////////////////////////////////////////////////////////////////////////
		public WSprite ImageDisabled
		{
			get
			{
				return NativeObject.ImageDisabled;
			}
			set
			{
				OnPropertyChanging("ImageDisabled");
				NativeObject.ImageDisabled = value;
			}
		}

		[Category(CategoryName.AppearanceDisabled), PropertyOrder(30)]
		[Description("The background tiled image used for disabled button")]
		[EditorAttribute(typeof(TiledImgTypeEditor), typeof(UITypeEditor))]
		//////////////////////////////////////////////////////////////////////////
		public WUITiledImage TiledImageDisabled
		{
			get
			{
				return NativeObject.BackDisabled;
			}
			set
			{
				OnPropertyChanging("TiledImageDisabled");
				NativeObject.BackDisabled = value;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// focused
		//////////////////////////////////////////////////////////////////////////
		[Category(CategoryName.AppearanceFocused), PropertyOrder(10)]
		[Description("The font used for focused button")]
		[EditorAttribute(typeof(FontTypeEditor), typeof(UITypeEditor))]
		//////////////////////////////////////////////////////////////////////////
		public WFont FontFocused
		{
			get
			{
				return NativeObject.FontFocused;
			}
			set
			{
				OnPropertyChanging("FontFocused");
				NativeObject.FontFocused = value;
			}
		}

		[Category(CategoryName.AppearanceFocused), PropertyOrder(20)]
		[Description("The background image used for focused button")]
		[EditorAttribute(typeof(SpriteTypeEditor), typeof(UITypeEditor))]
		//////////////////////////////////////////////////////////////////////////
		public WSprite ImageFocused
		{
			get
			{
				return NativeObject.ImageFocused;
			}
			set
			{
				OnPropertyChanging("ImageFocused");
				NativeObject.ImageFocused = value;
			}
		}

		[Category(CategoryName.AppearanceFocused), PropertyOrder(30)]
		[Description("The background tiled image used for focused button")]
		[EditorAttribute(typeof(TiledImgTypeEditor), typeof(UITypeEditor))]
		//////////////////////////////////////////////////////////////////////////
		public WUITiledImage TiledImageFocused
		{
			get
			{
				return NativeObject.BackFocused;
			}
			set
			{
				OnPropertyChanging("TiledImageFocused");
				NativeObject.BackFocused = value;
			}
		}
	}
}
