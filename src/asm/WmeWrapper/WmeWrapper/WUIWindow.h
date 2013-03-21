// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WDynArray.h"
#include "enums.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WUIWindow : public WUIObject
	{
	public:
		WUIWindow(){};
		WUIWindow(WGame^ Game);
		WUIWindow(CUIWindow* Native);
		virtual ~WUIWindow(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WDynArray<CUIObject*, WUIObject>^ Controls
		{
			WDynArray<CUIObject*, WUIObject>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CUIObject*, WUIObject>(&Native->m_Widgets, TypeConvertor);
			}
		}

		static WUIObject^ TypeConvertor(CUIObject* NativeVal)
		{
			return WUtils::CastUIObject(NativeVal);
		}

		property bool PauseMusic
		{
			bool get()
			{
				return Native->m_PauseMusic;
			}
			void set(bool Value)
			{
				Native->m_PauseMusic = Value;
			}
		}

		property bool Freezable
		{
			bool get()
			{
				return Native->m_Freezable;
			}
			void set(bool Value)
			{
				Native->MakeFreezable(Value);
			}
		}

		property bool ClipContents
		{
			bool get()
			{
				return Native->m_ClipContents;
			}
			void set(bool Value)
			{
				Native->m_ClipContents = Value;
			}
		}

		property bool InGame
		{
			bool get()
			{
				return Native->m_InGame;
			}
			void set(bool Value)
			{
				Native->m_InGame = Value;
			}
		}

		property bool IsMenu
		{
			bool get()
			{
				return Native->m_IsMenu;
			}
			void set(bool Value)
			{
				Native->m_IsMenu = Value;
			}
		}

		property bool FadeBackground
		{
			bool get()
			{
				return Native->m_FadeBackground;
			}
			void set(bool Value)
			{
				Native->m_FadeBackground = Value;
			}
		}

		property System::Drawing::Color FadeColor
		{
			System::Drawing::Color get()
			{
				return System::Drawing::Color::FromArgb(Native->m_FadeColor);
			}
			void set(System::Drawing::Color Value)
			{
				Native->m_FadeColor = Value.ToArgb();
				Native->m_FadeBackground = (Native->m_FadeColor != 0);
			}
		}

		property WEWindowMode Mode
		{
			WEWindowMode get()
			{
				return (WEWindowMode)Native->m_Mode;
			}
			void set(WEWindowMode Value)
			{
				Native->m_Mode = (TWindowMode)Value;
			}
		}

		property bool Dragging
		{
			bool get()
			{
				return Native->m_Dragging;
			}
			void set(bool Value)
			{
				Native->m_Dragging = Value;
			}
		}

		property bool Transparent
		{
			bool get()
			{
				return Native->m_Transparent;
			}
			void set(bool Value)
			{
				Native->m_Transparent = Value;
			}
		}

		property System::Drawing::Rectangle TitleRectangle
		{
			System::Drawing::Rectangle get()
			{
				return System::Drawing::Rectangle::FromLTRB(Native->m_TitleRect.left, Native->m_TitleRect.top, Native->m_TitleRect.right, Native->m_TitleRect.bottom);
			}
			void set(System::Drawing::Rectangle Value)
			{
				Native->m_TitleRect.left = Value.Left;
				Native->m_TitleRect.right = Value.Right;
				Native->m_TitleRect.top = Value.Top;
				Native->m_TitleRect.bottom = Value.Bottom;
			}
		}

		property System::Drawing::Rectangle DragRectangle
		{
			System::Drawing::Rectangle get()
			{
				return System::Drawing::Rectangle::FromLTRB(Native->m_DragRect.left, Native->m_DragRect.top, Native->m_DragRect.right, Native->m_DragRect.bottom);
			}
			void set(System::Drawing::Rectangle Value)
			{
				Native->m_DragRect.left = Value.Left;
				Native->m_DragRect.right = Value.Right;
				Native->m_DragRect.top = Value.Top;
				Native->m_DragRect.bottom = Value.Bottom;
			}
		}

		property WETextAlign TitleAlignment
		{
			WETextAlign get()
			{
				return (WETextAlign)Native->m_TitleAlign;
			}
			void set(WETextAlign Value)
			{
				Native->m_TitleAlign = (TTextAlign)Value;
			}
		}

		property WUITiledImage^ BackInactive
		{
			WUITiledImage^ get()
			{
				if(Native->m_BackInactive) return gcnew WUITiledImage(Native->m_BackInactive);
				else return nullptr;
			}
			void set(WUITiledImage^ Value)
			{
				SAFE_DELETE(Native->m_BackInactive);
				if(Value!=nullptr) Native->m_BackInactive = Value->Native;
			}
		}

		property WFont^ FontInactive
		{
			WFont^ get()
			{
				if(Native->m_FontInactive) return gcnew WFont(Native->m_FontInactive);
				else return nullptr;
			}
			void set(WFont^ Value)
			{
				if(!Native->m_SharedFonts) Native->Game->m_FontStorage->RemoveFont(Native->m_FontInactive);
				Native->m_FontInactive = NULL;

				if(Value!=nullptr) Native->m_FontInactive = Value->Native;
			}
		}

		property WSprite^ ImageInactive
		{
			WSprite^ get()
			{
				if(Native->m_ImageInactive) return gcnew WSprite(Native->m_ImageInactive);
				else return nullptr;
			}
			void set(WSprite^ Value)
			{
				if(!Native->m_SharedImages) SAFE_DELETE(Native->m_ImageInactive);
				else Native->m_ImageInactive = NULL;

				if(Value!=nullptr) Native->m_ImageInactive = Value->Native;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		bool Close()
		{
			return SUCCEEDED(Native->Close());
		}

		bool GoExclusive()
		{
			return SUCCEEDED(Native->GoExclusive());
		}

		bool GoSystemExclusive()
		{
			return SUCCEEDED(Native->GoSystemExclusive());
		}

		bool MoveFocus(bool Forward)
		{
			return SUCCEEDED(Native->MoveFocus(Forward));
		}

		bool MoveFocus()
		{
			return MoveFocus(true);
		}

		bool ShowControl(String^ Name, bool Visible)
		{
			char* S = (char*)WUtils::GetString(Name);
			bool Ret = SUCCEEDED(Native->ShowWidget(S, Visible));
			WUtils::FreeString(S);
			return Ret;
		}

		bool ShowControl(String^ Name)
		{
			return ShowControl(Name, true);
		}

		bool EnableControl(String^ Name, bool Enable)
		{
			char* S = (char*)WUtils::GetString(Name);
			bool Ret = SUCCEEDED(Native->EnableWidget(S, Enable));
			WUtils::FreeString(S);
			return Ret;
		}

		bool EnableControl(String^ Name)
		{
			return EnableControl(Name, true);
		}

		virtual bool Display(int OffsetX, int OffsetY) override
		{
			return SUCCEEDED(Native->Display(OffsetX, OffsetY));
		}

		virtual bool Display() override
		{
			return Display(0, 0);
		}

		bool AddControl(WUIObject^ Control)
		{
			if(Control==nullptr || Control->Native==NULL) return false;

			Native->m_Widgets.Add(Control->Native);
			Control->Native->m_Parent = this->Native;
			return true;
		}

		bool RemoveControl(WUIObject^ Control)
		{
			for(int i=0; i<Native->m_Widgets.GetSize(); i++)
			{
				if(Control->Native == Native->m_Widgets[i])
				{
					SAFE_DELETE(Native->m_Widgets[i]);
					Native->m_Widgets.RemoveAt(i);
					Control->InvalidateNative();
					return true;
				}
			}
			return false;
		}


		//////////////////////////////////////////////////////////////////////////
		// Saving / loading
		//////////////////////////////////////////////////////////////////////////
		virtual bool LoadFromFile(String^ Filename)
		{
			bool Ret = false;
			char* S = (char*)WUtils::GetString(Filename);
			Ret = SUCCEEDED(Native->LoadFile(S));
			WUtils::FreeString(S);

			return Ret;
		}

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
		property CUIWindow* Native
		{
			CUIWindow* get() { return (CUIWindow*)m_Native; };
		}

	};

}}};

