// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WAdTalkHolder.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WAdItem : public WAdTalkHolder
	{
	public:
		WAdItem(){};
		WAdItem(WGame^ Game);
		WAdItem(CAdItem* Native);
		virtual ~WAdItem(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property bool DisplayAmount
		{
			bool get()
			{
				return Native->m_DisplayAmount;
			}
			void set(bool Value)
			{
				Native->m_DisplayAmount = Value;
			}
		}

		property int Amount
		{
			int get()
			{
				return Native->m_Amount;
			}
			void set(int Value)
			{
				Native->m_Amount = Value;
			}
		}

		property String^ AmountString
		{
			String^ get()
			{
				if(Native->m_AmountString) return gcnew String(Native->m_AmountString);
				else return nullptr;
			}
			void set(String^ Value)
			{
				SAFE_DELETE_ARRAY(Native->m_AmountString);
				if(Value!=nullptr)
				{
					char* S = (char*)WUtils::GetString(Value);
					CBUtils::SetString(&Native->m_AmountString, S);
					WUtils::FreeString(S);
				}
			}
		}

		property int AmountOffsetX
		{
			int get()
			{
				return Native->m_AmountOffsetX;
			}
			void set(int Value)
			{
				Native->m_AmountOffsetX = Value;
			}
		}

		property int AmountOffsetY
		{
			int get()
			{
				return Native->m_AmountOffsetY;
			}
			void set(int Value)
			{
				Native->m_AmountOffsetY = Value;
			}
		}

		property WETextAlign AmountAlignment
		{
			WETextAlign get()
			{
				return (WETextAlign)Native->m_AmountAlign;
			}
			void set(WETextAlign Value)
			{
				Native->m_AmountAlign = (TTextAlign)Value;
			}
		}

		property bool Taken
		{
			bool get()
			{
				return Native->m_InInventory;
			}
		}

		property bool CombinedCursor
		{
			bool get()
			{
				return Native->m_CursorCombined;
			}
			void set(bool Value)
			{
				Native->m_CursorCombined = Value;
			}
		}

		property WSprite^ SpriteHover
		{
			WSprite^ get()
			{
				if(Native->m_SpriteHover) return gcnew WSprite(Native->m_SpriteHover);
				else return nullptr;
			}
			void set(WSprite^ Value)
			{
				SAFE_DELETE(Native->m_SpriteHover);
				if(Value!=nullptr) Native->m_SpriteHover = Value->Native;
			}
		}

		property WSprite^ CursorNormal
		{
			WSprite^ get()
			{
				if(Native->m_CursorNormal) return gcnew WSprite(Native->m_CursorNormal);
				else return nullptr;
			}
			void set(WSprite^ Value)
			{
				SAFE_DELETE(Native->m_CursorNormal);
				if(Value!=nullptr) Native->m_CursorNormal = Value->Native;
			}
		}

		property WSprite^ CursorHover
		{
			WSprite^ get()
			{
				if(Native->m_CursorHover) return gcnew WSprite(Native->m_CursorHover);
				else return nullptr;
			}
			void set(WSprite^ Value)
			{
				SAFE_DELETE(Native->m_CursorHover);
				if(Value!=nullptr) Native->m_CursorHover = Value->Native;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		virtual bool Update() override
		{
			return SUCCEEDED(Native->Update());
		}

		bool Display(int X, int Y)
		{
			return SUCCEEDED(Native->Display(X, Y));
		}

		//////////////////////////////////////////////////////////////////////////
		// Saving / loading
		//////////////////////////////////////////////////////////////////////////
		virtual bool SaveAsText(WDynBuffer^ Buffer, int Indent) override
		{
			return SUCCEEDED(Native->SaveAsText(Buffer->Native, Indent));
		}

		virtual bool SaveAsText(WDynBuffer^ Buffer) override
		{
			return SaveAsText(Buffer, 0);
		}


	internal:
		property CAdItem* Native
		{
			CAdItem* get() { return (CAdItem*)m_Native; };
		}

	};

}}};

