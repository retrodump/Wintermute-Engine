// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WDynArray.h"
#include "enums.h"
#include "Interfaces.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;
	using namespace System::Runtime::InteropServices;

	ref class WUIWindow;

	public ref class WUIObject : public WObject, public IEditorResizable
	{
	public:
		WUIObject(){};
		WUIObject(WGame^ Game);
		WUIObject(CUIObject* Native);
		virtual ~WUIObject(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property bool CanFocus
		{
			bool get()
			{
				return Native->m_CanFocus;
			}
			void set(bool Value)
			{
				Native->m_CanFocus = Value;
			}
		}

		property bool Focused
		{
			bool get()
			{
				return Native->IsFocused();
			}
		}

		property bool ParentNotify
		{
			bool get()
			{
				return Native->m_ParentNotify;
			}
			void set(bool Value)
			{
				Native->m_ParentNotify = Value;
			}
		}

		property WUIWindow^ Parent
		{
			WUIWindow^ get()
			{
				if(Native->m_Parent) return (WUIWindow^)WUtils::CastUIObject(Native->m_Parent);
				else return nullptr;
			}
		}

		property bool SharedFonts
		{
			bool get()
			{
				return Native->m_SharedFonts;
			}
			void set(bool Value)
			{
				Native->m_SharedFonts = Value;
			}
		}

		property bool SharedImages
		{
			bool get()
			{
				return Native->m_SharedImages;
			}
			void set(bool Value)
			{
				Native->m_SharedImages = Value;
			}
		}

		property String^ Text
		{
			String^ get()
			{
				if(Native->m_Text) return gcnew String(Native->m_Text);
				else return nullptr;
			}
			void set(String^ Value)
			{
				char* S = (char*)WUtils::GetString(Value);
				Native->SetText(S);
				WUtils::FreeString(S);
			}
		}

		property bool Visible
		{
			bool get()
			{
				return Native->m_Visible;
			}
			void set(bool Value)
			{
				Native->m_Visible = Value;
			}
		}

		property bool Disabled
		{
			bool get()
			{
				return Native->m_Disable;
			}
			void set(bool Value)
			{
				Native->m_Disable = Value;
			}
		}

		property WFont^ Font
		{
			WFont^ get()
			{
				if(Native->m_Font) return gcnew WFont(Native->m_Font);
				else return nullptr;
			}
			void set(WFont^ Value)
			{
				if(!Native->m_SharedFonts) Native->Game->m_FontStorage->RemoveFont(Native->m_Font);
				Native->m_Font = NULL;

				if(Value!=nullptr) Native->m_Font = Value->Native;
			}
		}

		property WSprite^ Image
		{
			WSprite^ get()
			{
				if(Native->m_Image) return gcnew WSprite(Native->m_Image);
				else return nullptr;
			}
			void set(WSprite^ Value)
			{
				if(!Native->m_SharedImages) SAFE_DELETE(Native->m_Image);
				else Native->m_Image = NULL;

				if(Value!=nullptr) Native->m_Image = Value->Native;
			}
		}

		property WUITiledImage^ Back
		{
			WUITiledImage^ get()
			{
				if(Native->m_Back) return gcnew WUITiledImage(Native->m_Back);
				else return nullptr;
			}
			void set(WUITiledImage^ Value)
			{
				SAFE_DELETE(Native->m_Back);
				if(Value!=nullptr) Native->m_Back = Value->Native;
			}
		}

		property int Width
		{
			virtual int get()
			{
				return Native->m_Width;
			}
			virtual void set(int Value)
			{
				Native->m_Width = Value;
			}
		}

		property int Height
		{
			virtual int get() override
			{
				return Native->m_Height;
			}
			virtual void set(int Value) override
			{
				Native->m_Height = Value;
			}
		}

		property WEUIObjectType Type
		{
			WEUIObjectType get()
			{
				return (WEUIObjectType)Native->m_Type;
			}
			void set(WEUIObjectType Value)
			{
				Native->m_Type = (TUIObjectType)Value;
			}
		}

		property WUIObject^ FocusedControl
		{
			WUIObject^ get()
			{
				if(Native->m_FocusedWidget) return WUtils::CastUIObject(Native->m_FocusedWidget);
				else return nullptr;
			}
			void set(WUIObject^ Value)
			{
				if(Value!=nullptr) Native->m_FocusedWidget = Value->Native;
				else Native->m_FocusedWidget = NULL;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		bool GetTotalOffset([Out]int% OffsetX, [Out]int% OffsetY)
		{
			int X, Y;
			bool Ret = SUCCEEDED(Native->GetTotalOffset(&X, &Y));
			OffsetX = X;
			OffsetY = Y;
			return Ret;
		}

		bool Focus()
		{
			return SUCCEEDED(Native->Focus());
		}

		virtual bool Display(int OffsetX, int OffsetY)
		{
			return SUCCEEDED(Native->Display(OffsetX, OffsetY));
		}

		virtual bool Display()
		{
			return Display(0, 0);
		}

		virtual bool CorrectSize()
		{
			Native->CorrectSize();
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		// save / load
		//////////////////////////////////////////////////////////////////////////
		virtual bool SaveAsText(WDynBuffer^ Buffer, int Indent)
		{
			return SUCCEEDED(Native->SaveAsText(Buffer->Native, Indent));
		}

		virtual bool SaveAsText(WDynBuffer^ Buffer)
		{
			return SaveAsText(Buffer, 0);
		}

		virtual bool LoadFromBuffer(String^ Buffer, bool Complete)
		{
			return false;
		}

		virtual bool LoadFromBuffer(String^ Buffer)
		{
			return false;
		}

	internal:
		property CUIObject* Native
		{
			CUIObject* get() { return (CUIObject*)m_Native; };
		}

	};

}}};

