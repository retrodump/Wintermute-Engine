// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WDynArray.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WUIEdit : public WUIObject
	{
	public:
		WUIEdit(){};
		WUIEdit(WGame^ Game);
		WUIEdit(CUIEdit* Native);
		virtual ~WUIEdit(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property int MaxLength
		{
			int get()
			{
				return Native->m_MaxLength;
			}
			void set(int Value)
			{
				Native->m_MaxLength = Value;
			}
		}

		property bool CursorVisible
		{
			bool get()
			{
				return Native->m_CursorVisible;
			}
			void set(bool Value)
			{
				Native->m_CursorVisible = Value;
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

		property int FrameWidth
		{
			int get()
			{
				return Native->m_FrameWidth;
			}
			void set(int Value)
			{
				Native->m_FrameWidth = Value;
			}
		}

		property int CursorBlinkRate
		{
			int get()
			{
				return Native->m_CursorBlinkRate;
			}
			void set(int Value)
			{
				Native->m_CursorBlinkRate = Value;
			}
		}

		property String^ CursorChar
		{
			String^ get()
			{
				if(Native->m_CursorChar) return gcnew String(Native->m_CursorChar);
				else return nullptr;
			}
			void set(String^ Value)
			{
				Value = Value->Substring(0, 1);
				char* S = (char*)WUtils::GetString(Value);
				Native->SetCursorChar(S);
				WUtils::FreeString(S);
			}
		}

		property int SelectionStart
		{
			int get()
			{
				return Native->m_SelStart;
			}
			void set(int Value)
			{
				Native->m_SelStart = Value;
			}
		}

		property int SelectionEnd
		{
			int get()
			{
				return Native->m_SelEnd;
			}
			void set(int Value)
			{
				Native->m_SelEnd = Value;
			}
		}

		property WFont^ FontSelected
		{
			WFont^ get()
			{
				if(Native->m_FontSelected) return gcnew WFont(Native->m_FontSelected);
				else return nullptr;
			}
			void set(WFont^ Value)
			{
				if(!Native->m_SharedFonts) Native->Game->m_FontStorage->RemoveFont(Native->m_FontSelected);
				Native->m_FontSelected = NULL;

				if(Value!=nullptr) Native->m_FontSelected = Value->Native;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		int InsertChars(int Pos, String^ Text, int NumChars)
		{
			char* S = (char*)WUtils::GetString(Text);
			int Ret = Native->InsertChars(Pos, (BYTE*)S, NumChars);
			WUtils::FreeString(S);
			return Ret;
		}

		int InsertChars(int Pos, String^ Text)
		{
			return InsertChars(Pos, Text, Text->Length);
		}

		int DeleteChars(int Start, int End)
		{
			return Native->DeleteChars(Start, End);
		}

		virtual bool Display(int OffsetX, int OffsetY) override
		{
			return SUCCEEDED(Native->Display(OffsetX, OffsetY));
		}

		virtual bool Display() override
		{
			return Display(0, 0);
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
		property CUIEdit* Native
		{
			CUIEdit* get() { return (CUIEdit*)m_Native; };
		}

	};

}}};

