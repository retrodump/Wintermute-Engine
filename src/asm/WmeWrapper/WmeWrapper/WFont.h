// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WDynArray.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WFont : public WObject
	{
	public:
		WFont(){};
		WFont(WGame^ Game);
		WFont(CBFont* Native);
		virtual ~WFont(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property int LetterHeight
		{
			int get()
			{
				return Native->GetLetterHeight();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		int GetTextWidth(String^ Text, int MaxLenght)
		{
			BYTE* S = (BYTE*)WUtils::GetString(Text);
			int Ret = Native->GetTextWidth(S, MaxLenght);
			WUtils::FreeString((char*)S);
			return Ret;
		}

		int GetTextWidth(String^ Text)
		{
			return GetTextWidth(Text, -1);
		}

		int GetTextHeight(String^ Text, int Width)
		{
			BYTE* S = (BYTE*)WUtils::GetString(Text);
			int Ret = Native->GetTextHeight(S, Width);
			WUtils::FreeString((char*)S);
			return Ret;
		}

		bool DrawText(String^ Text, int X, int Y, int Width, WETextAlign Align, int MaxHeight, int MaxLenght)
		{
			BYTE* S = (BYTE*)WUtils::GetString(Text);
			Native->DrawText(S, X, Y, Width, (TTextAlign)Align, MaxHeight, MaxLenght);
			WUtils::FreeString((char*)S);
			return true;
		}

		bool DrawText(String^ Text, int X, int Y, int Width, WETextAlign Align, int MaxHeight)
		{
			return DrawText(Text, X, Y, Width, Align, MaxHeight, -1);
		}

		bool DrawText(System::String^ Text, int X, int Y, int Width, WETextAlign Align)
		{
			return DrawText(Text, X, Y, Width, Align, -1);
		}

		bool DrawText(String^ Text, int X, int Y, int Width)
		{
			return DrawText(Text, X, Y, Width, WETextAlign::Left);
		}


		static WFont^ CreateFromFile(WGame^ Game, String^ Filename);
		

	internal:
		property CBFont* Native
		{
			CBFont* get() { return (CBFont*)m_Native; };
		}

	};

}}};

