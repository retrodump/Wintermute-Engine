// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WDynBuffer.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WUITiledImage : public WObject
	{
	public:
		WUITiledImage(){};
		WUITiledImage(WGame^ Game);
		WUITiledImage(CUITiledImage* Native);
		virtual ~WUITiledImage(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WSubFrame^ Image
		{
			WSubFrame^ get()
			{
				if(Native->m_Image) return gcnew WSubFrame(Native->m_Image);
				else return nullptr;
			}
			void set(WSubFrame^ Value)
			{
				SAFE_DELETE(Native->m_Image);
				if(Value!=nullptr) Native->m_Image = Value->Native;
			}
		}

		property System::Drawing::Rectangle UpLeft
		{
			System::Drawing::Rectangle get()
			{
				RECT rc = Native->m_UpLeft;
				return System::Drawing::Rectangle::FromLTRB(rc.left, rc.top, rc.right, rc.bottom);
			}
			void set(System::Drawing::Rectangle Value)
			{
				SetRect(&Native->m_UpLeft, Value.Left, Value.Top, Value.Right, Value.Bottom);
			}
		}

		property System::Drawing::Rectangle UpMiddle
		{
			System::Drawing::Rectangle get()
			{
				RECT rc = Native->m_UpMiddle;
				return System::Drawing::Rectangle::FromLTRB(rc.left, rc.top, rc.right, rc.bottom);
			}
			void set(System::Drawing::Rectangle Value)
			{
				SetRect(&Native->m_UpMiddle, Value.Left, Value.Top, Value.Right, Value.Bottom);
			}
		}

		property System::Drawing::Rectangle UpRight
		{
			System::Drawing::Rectangle get()
			{
				RECT rc = Native->m_UpRight;
				return System::Drawing::Rectangle::FromLTRB(rc.left, rc.top, rc.right, rc.bottom);
			}
			void set(System::Drawing::Rectangle Value)
			{
				SetRect(&Native->m_UpRight, Value.Left, Value.Top, Value.Right, Value.Bottom);
			}
		}

		property System::Drawing::Rectangle MiddleLeft
		{
			System::Drawing::Rectangle get()
			{
				RECT rc = Native->m_MiddleLeft;
				return System::Drawing::Rectangle::FromLTRB(rc.left, rc.top, rc.right, rc.bottom);
			}
			void set(System::Drawing::Rectangle Value)
			{
				SetRect(&Native->m_MiddleLeft, Value.Left, Value.Top, Value.Right, Value.Bottom);
			}
		}

		property System::Drawing::Rectangle MiddleMiddle
		{
			System::Drawing::Rectangle get()
			{
				RECT rc = Native->m_MiddleMiddle;
				return System::Drawing::Rectangle::FromLTRB(rc.left, rc.top, rc.right, rc.bottom);
			}
			void set(System::Drawing::Rectangle Value)
			{
				SetRect(&Native->m_MiddleMiddle, Value.Left, Value.Top, Value.Right, Value.Bottom);
			}
		}

		property System::Drawing::Rectangle MiddleRight
		{
			System::Drawing::Rectangle get()
			{
				RECT rc = Native->m_MiddleRight;
				return System::Drawing::Rectangle::FromLTRB(rc.left, rc.top, rc.right, rc.bottom);
			}
			void set(System::Drawing::Rectangle Value)
			{
				SetRect(&Native->m_MiddleRight, Value.Left, Value.Top, Value.Right, Value.Bottom);
			}
		}

		property System::Drawing::Rectangle DownLeft
		{
			System::Drawing::Rectangle get()
			{
				RECT rc = Native->m_DownLeft;
				return System::Drawing::Rectangle::FromLTRB(rc.left, rc.top, rc.right, rc.bottom);
			}
			void set(System::Drawing::Rectangle Value)
			{
				SetRect(&Native->m_DownLeft, Value.Left, Value.Top, Value.Right, Value.Bottom);
			}
		}

		property System::Drawing::Rectangle DownMiddle
		{
			System::Drawing::Rectangle get()
			{
				RECT rc = Native->m_DownMiddle;
				return System::Drawing::Rectangle::FromLTRB(rc.left, rc.top, rc.right, rc.bottom);
			}
			void set(System::Drawing::Rectangle Value)
			{
				SetRect(&Native->m_DownMiddle, Value.Left, Value.Top, Value.Right, Value.Bottom);
			}
		}

		property System::Drawing::Rectangle DownRight
		{
			System::Drawing::Rectangle get()
			{
				RECT rc = Native->m_DownRight;
				return System::Drawing::Rectangle::FromLTRB(rc.left, rc.top, rc.right, rc.bottom);
			}
			void set(System::Drawing::Rectangle Value)
			{
				SetRect(&Native->m_DownRight, Value.Left, Value.Top, Value.Right, Value.Bottom);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		bool CorrectSize(int% Width, int% Height)
		{
			int W, H;
			W = Width;
			H = Height;
			Native->CorrectSize(&W, &H);
			Width = W;
			Height = H;
			return true;
		}

		bool Display(int X, int Y, int Width, int Height)
		{
			return SUCCEEDED(Native->Display(X, Y, Width, Height));
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

	internal:
		property CUITiledImage* Native
		{
			CUITiledImage* get() { return (CUITiledImage*)m_Native; };
		}

	};

}}};

