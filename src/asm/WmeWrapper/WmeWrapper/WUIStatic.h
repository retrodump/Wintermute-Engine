// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WDynArray.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WUIStatic : public WUIObject
	{
	public:
		WUIStatic(){};
		WUIStatic(WGame^ Game);
		WUIStatic(CUIText* Native);
		virtual ~WUIStatic(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WETextAlign TextAlignment
		{
			WETextAlign get()
			{
				return (WETextAlign)Native->m_TextAlign;
			}
			void set(WETextAlign Value)
			{
				Native->m_TextAlign = (TTextAlign)Value;
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

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		bool SizeToFit()
		{
			return SUCCEEDED(Native->SizeToFit());
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
		property CUIText* Native
		{
			CUIText* get() { return (CUIText*)m_Native; };
		}

	};

}}};

