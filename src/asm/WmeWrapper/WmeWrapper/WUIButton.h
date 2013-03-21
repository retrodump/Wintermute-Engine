// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WDynArray.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WUIButton : public WUIObject
	{
	public:
		WUIButton(){};
		WUIButton(WGame^ Game);
		WUIButton(CUIButton* Native);
		virtual ~WUIButton(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property bool PixelPerfect
		{
			bool get()
			{
				return Native->m_PixelPerfect;
			}
			void set(bool Value)
			{
				Native->m_PixelPerfect = Value;
			}
		}

		property bool StayPressed
		{
			bool get()
			{
				return Native->m_StayPressed;
			}
			void set(bool Value)
			{
				Native->m_StayPressed = Value;
			}
		}

		property bool CenterImage
		{
			bool get()
			{
				return Native->m_CenterImage;
			}
			void set(bool Value)
			{
				Native->m_CenterImage = Value;
			}
		}

		property WETextAlign TextAlignment
		{
			WETextAlign get()
			{
				return (WETextAlign)Native->m_Align;
			}
			void set(WETextAlign Value)
			{
				Native->m_Align = (TTextAlign)Value;
			}
		}

		property WSprite^ ImageHover
		{
			WSprite^ get()
			{
				if(Native->m_ImageHover) return gcnew WSprite(Native->m_ImageHover);
				else return nullptr;
			}
			void set(WSprite^ Value)
			{
				if(!Native->m_SharedImages) SAFE_DELETE(Native->m_ImageHover);
				else Native->m_ImageHover = NULL;

				if(Value!=nullptr) Native->m_ImageHover = Value->Native;
			}
		}

		property WSprite^ ImagePressed
		{
			WSprite^ get()
			{
				if(Native->m_ImagePress) return gcnew WSprite(Native->m_ImagePress);
				else return nullptr;
			}
			void set(WSprite^ Value)
			{
				if(!Native->m_SharedImages) SAFE_DELETE(Native->m_ImagePress);
				else Native->m_ImagePress = NULL;

				if(Value!=nullptr) Native->m_ImagePress = Value->Native;
			}
		}

		property WSprite^ ImageDisabled
		{
			WSprite^ get()
			{
				if(Native->m_ImageDisable) return gcnew WSprite(Native->m_ImageDisable);
				else return nullptr;
			}
			void set(WSprite^ Value)
			{
				if(!Native->m_SharedImages) SAFE_DELETE(Native->m_ImageDisable);
				else Native->m_ImageDisable = NULL;

				if(Value!=nullptr) Native->m_ImageDisable = Value->Native;
			}
		}

		property WSprite^ ImageFocused
		{
			WSprite^ get()
			{
				if(Native->m_ImageFocus) return gcnew WSprite(Native->m_ImageFocus);
				else return nullptr;
			}
			void set(WSprite^ Value)
			{
				if(!Native->m_SharedImages) SAFE_DELETE(Native->m_ImageFocus);
				else Native->m_ImageFocus = NULL;

				if(Value!=nullptr) Native->m_ImageFocus = Value->Native;
			}
		}

		property WFont^ FontHover
		{
			WFont^ get()
			{
				if(Native->m_FontHover) return gcnew WFont(Native->m_FontHover);
				else return nullptr;
			}
			void set(WFont^ Value)
			{
				if(!Native->m_SharedFonts) Native->Game->m_FontStorage->RemoveFont(Native->m_FontHover);
				Native->m_FontHover = NULL;

				if(Value!=nullptr) Native->m_FontHover = Value->Native;
			}
		}

		property WFont^ FontPressed
		{
			WFont^ get()
			{
				if(Native->m_FontPress) return gcnew WFont(Native->m_FontPress);
				else return nullptr;
			}
			void set(WFont^ Value)
			{
				if(!Native->m_SharedFonts) Native->Game->m_FontStorage->RemoveFont(Native->m_FontPress);
				Native->m_FontPress = NULL;

				if(Value!=nullptr) Native->m_FontPress = Value->Native;
			}
		}

		property WFont^ FontDisabled
		{
			WFont^ get()
			{
				if(Native->m_FontDisable) return gcnew WFont(Native->m_FontDisable);
				else return nullptr;
			}
			void set(WFont^ Value)
			{
				if(!Native->m_SharedFonts) Native->Game->m_FontStorage->RemoveFont(Native->m_FontDisable);
				Native->m_FontDisable = NULL;

				if(Value!=nullptr) Native->m_FontDisable = Value->Native;
			}
		}

		property WFont^ FontFocused
		{
			WFont^ get()
			{
				if(Native->m_FontFocus) return gcnew WFont(Native->m_FontFocus);
				else return nullptr;
			}
			void set(WFont^ Value)
			{
				if(!Native->m_SharedFonts) Native->Game->m_FontStorage->RemoveFont(Native->m_FontFocus);
				Native->m_FontFocus = NULL;

				if(Value!=nullptr) Native->m_FontFocus = Value->Native;
			}
		}

		property WUITiledImage^ BackHover
		{
			WUITiledImage^ get()
			{
				if(Native->m_BackHover) return gcnew WUITiledImage(Native->m_BackHover);
				else return nullptr;
			}
			void set(WUITiledImage^ Value)
			{
				SAFE_DELETE(Native->m_BackHover);
				if(Value!=nullptr) Native->m_BackHover = Value->Native;
			}
		}

		property WUITiledImage^ BackPressed
		{
			WUITiledImage^ get()
			{
				if(Native->m_BackPress) return gcnew WUITiledImage(Native->m_BackPress);
				else return nullptr;
			}
			void set(WUITiledImage^ Value)
			{
				SAFE_DELETE(Native->m_BackPress);
				if(Value!=nullptr) Native->m_BackPress = Value->Native;
			}
		}

		property WUITiledImage^ BackDisabled
		{
			WUITiledImage^ get()
			{
				if(Native->m_BackDisable) return gcnew WUITiledImage(Native->m_BackDisable);
				else return nullptr;
			}
			void set(WUITiledImage^ Value)
			{
				SAFE_DELETE(Native->m_BackDisable);
				if(Value!=nullptr) Native->m_BackDisable = Value->Native;
			}
		}

		property WUITiledImage^ BackFocused
		{
			WUITiledImage^ get()
			{
				if(Native->m_BackFocus) return gcnew WUITiledImage(Native->m_BackFocus);
				else return nullptr;
			}
			void set(WUITiledImage^ Value)
			{
				SAFE_DELETE(Native->m_BackFocus);
				if(Value!=nullptr) Native->m_BackFocus = Value->Native;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		bool Press()
		{
			Native->Press();
			return true;
		}

		virtual bool Display(int OffsetX, int OffsetY) override
		{
			return SUCCEEDED(Native->Display(OffsetX, OffsetY));
		}

		virtual bool Display() override
		{
			return Display(0, 0);
		}

		virtual bool CorrectSize() override
		{
			Native->CorrectSize();
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		// Saving / loading
		//////////////////////////////////////////////////////////////////////////
		virtual bool LoadFromBuffer(String^ Buffer, bool Complete) override
		{
			bool Ret = false;
			char* S = (char*)WUtils::GetString(Buffer);
			Ret = SUCCEEDED(Native->LoadBuffer((BYTE*)S, Complete));
			WUtils::FreeString(S);

			return Ret;
		}

		virtual bool LoadFromBuffer(String^ Buffer) override
		{
			return LoadFromBuffer(Buffer, true);
		}

	internal:
		property CUIButton* Native
		{
			CUIButton* get() { return (CUIButton*)m_Native; };
		}

	};

}}};

