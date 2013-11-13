// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WAdObject.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;

	public ref class WAdTalkHolder : public WAdObject
	{
	public:
		WAdTalkHolder(){};
		WAdTalkHolder(WGame^ Game);
		WAdTalkHolder(CAdTalkHolder* Native);
		virtual ~WAdTalkHolder(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WDynArray<CBSprite*, WSprite>^ TalkSprites
		{
			WDynArray<CBSprite*, WSprite>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CBSprite*, WSprite>(&Native->m_TalkSprites);
			}
		}

		property WDynArray<CBSprite*, WSprite>^ TalkSpritesEx
		{
			WDynArray<CBSprite*, WSprite>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CBSprite*, WSprite>(&Native->m_TalkSpritesEx);
			}
		}

		property WSprite^ Sprite
		{
			WSprite^ get()
			{
				if(Native->m_Sprite) return gcnew WSprite(Native->m_Sprite);
				else return nullptr;
			}
			void set(WSprite^ Value)
			{
				SAFE_DELETE(Native->m_Sprite);
				if(Value) Native->m_Sprite = Value->Native;
			}
		}


	internal:
		property CAdTalkHolder* Native
		{
			CAdTalkHolder* get() { return (CAdTalkHolder*)m_Native; };
		}

	};

}}};

