// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WDynArray.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WAdResponseBox : public WObject, public IEditorResizable
	{
	public:
		WAdResponseBox(){};
		WAdResponseBox(WGame^ Game);
		WAdResponseBox(CAdResponseBox* Native);
		virtual ~WAdResponseBox(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WDynArray<CAdResponse*, WAdResponse>^ Responses
		{
			WDynArray<CAdResponse*, WAdResponse>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CAdResponse*, WAdResponse>(&Native->m_Responses);
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

		property WEVerticalAlign VerticalAlignment
		{
			WEVerticalAlign get()
			{
				return (WEVerticalAlign)Native->m_VerticalAlign;
			}
			void set(WEVerticalAlign Value)
			{
				Native->m_VerticalAlign = (TVerticalAlign)Value;
			}
		}

		property int Spacing
		{
			int get()
			{
				return Native->m_Spacing;
			}
			void set(int Value)
			{
				Native->m_Spacing = Value;
			}
		}

		property int ScrollOffset
		{
			int get()
			{
				return Native->m_ScrollOffset;
			}
			void set(int Value)
			{
				Native->m_ScrollOffset = Value;
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
				if(Native->m_Font) Native->Game->m_FontStorage->RemoveFont(Native->m_Font);

				if(Value!=nullptr) Native->m_Font = Value->Native;
				else Native->m_Font = NULL;
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
				if(Native->m_FontHover) Native->Game->m_FontStorage->RemoveFont(Native->m_FontHover);

				if(Value!=nullptr) Native->m_FontHover = Value->Native;
				else Native->m_FontHover = NULL;
			}
		}

		property System::Drawing::Rectangle Area
		{
			System::Drawing::Rectangle get()
			{
				return System::Drawing::Rectangle::FromLTRB(Native->m_ResponseArea.left, Native->m_ResponseArea.top, Native->m_ResponseArea.right, Native->m_ResponseArea.bottom);
			}
			void set(System::Drawing::Rectangle Value)
			{
				Native->m_ResponseArea.left = Value.Left;
				Native->m_ResponseArea.right = Value.Right;
				Native->m_ResponseArea.top = Value.Top;
				Native->m_ResponseArea.bottom = Value.Bottom;
			}
		}

		property WUIWindow^ Window
		{
			WUIWindow^ get()
			{
				if(Native->m_Window) return gcnew WUIWindow(Native->m_Window);
				else return nullptr;
			}
			void set(WUIWindow^ Value)
			{
				SAFE_DELETE(Native->m_Window);
				if(Value!=nullptr) Native->m_Window = Value->Native;
			}
		}

		property bool Horizontal
		{
			bool get()
			{
				return Native->m_Horizontal;
			}
			void set(bool Value)
			{
				Native->m_Horizontal = Value;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		bool Display()
		{
			return SUCCEEDED(Native->Display());
		}

		bool CreateButtons()
		{
			return SUCCEEDED(Native->CreateButtons());
		}


		//////////////////////////////////////////////////////////////////////////
		// Saving / loading
		//////////////////////////////////////////////////////////////////////////
		bool LoadFromFile(String^ Filename)
		{
			bool Ret = false;
			char* S = (char*)WUtils::GetString(Filename);
			Ret = SUCCEEDED(Native->LoadFile(S));
			WUtils::FreeString(S);

			return Ret;
		}

		bool LoadFromBuffer(String^ Buffer, bool Complete)
		{
			bool Ret = false;
			char* S = (char*)WUtils::GetString(Buffer);
			Ret = SUCCEEDED(Native->LoadBuffer((BYTE*)S, Complete));
			WUtils::FreeString(S);

			return Ret;
		}

		bool LoadFromBuffer(String^ Buffer)
		{
			return LoadFromBuffer(Buffer, true);
		}

		bool SaveAsText(WDynBuffer^ Buffer, int Indent)
		{
			return SUCCEEDED(Native->SaveAsText(Buffer->Native, Indent));
		}

		bool SaveAsText(WDynBuffer^ Buffer)
		{
			return SaveAsText(Buffer, 0);
		}


		//////////////////////////////////////////////////////////////////////////
		// IEditorResizable implementation
		//////////////////////////////////////////////////////////////////////////
		property int X
		{
			virtual int get() override
			{
				return this->Area.X;
			}
			virtual void set(int Value) override
			{
				this->Area = System::Drawing::Rectangle(Value, this->Area.Y, this->Area.Width, this->Area.Height);
			}
		}

		property int Y
		{
			virtual int get() override
			{
				return this->Area.Y;
			}
			virtual void set(int Value) override
			{
				this->Area = System::Drawing::Rectangle(this->Area.X, Value, this->Area.Width, this->Area.Height);
			}
		}

		property int Width
		{
			virtual int get()
			{
				return this->Area.Width;
			}
			virtual void set(int Value)
			{
				this->Area = System::Drawing::Rectangle(this->Area.X, this->Area.Y, Value, this->Area.Height);
			}
		}

		property int Height
		{
			virtual int get() override
			{
				return this->Area.Height;
			}
			virtual void set(int Value) override
			{
				this->Area = System::Drawing::Rectangle(this->Area.X, this->Area.Y, this->Area.Width, Value);
			}
		}

	internal:
		property CAdResponseBox* Native
		{
			CAdResponseBox* get() { return (CAdResponseBox*)m_Native; };
		}

	};

}}};

