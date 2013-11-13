// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WAdTalkNode.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WAdTalkDef : public WObject
	{
	public:
		WAdTalkDef(){};
		WAdTalkDef(WGame^ Game);
		WAdTalkDef(CAdTalkDef* Native);
		virtual ~WAdTalkDef(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WDynArray<CAdTalkNode*, WAdTalkNode>^ Nodes
		{
			WDynArray<CAdTalkNode*, WAdTalkNode>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CAdTalkNode*, WAdTalkNode>(&Native->m_Nodes);
			}
		}

		property String^ DefaultSpriteSetFilename
		{
			String^ get()
			{
				if(Native->m_DefaultSpriteSetFilename) return gcnew String(Native->m_DefaultSpriteSetFilename);
				else return nullptr;
			}
			void set(String^ Value)
			{
				char* S = (char*)WUtils::GetString(Value);
				CBUtils::SetString(&Native->m_DefaultSpriteSetFilename, S);
				WUtils::FreeString(S);
			}
		}

		property String^ DefaultSpriteFilename
		{
			String^ get()
			{
				if(Native->m_DefaultSpriteFilename) return gcnew String(Native->m_DefaultSpriteFilename);
				else return nullptr;
			}
			void set(String^ Value)
			{
				char* S = (char*)WUtils::GetString(Value);
				CBUtils::SetString(&Native->m_DefaultSpriteFilename, S);
				WUtils::FreeString(S);
			}
		}

		property WAdSpriteSet^ DefaultSpriteSet
		{
			WAdSpriteSet^ get()
			{
				if(Native->m_DefaultSpriteSet) return gcnew WAdSpriteSet(Native->m_DefaultSpriteSet);
				else return nullptr;
			}
			void set(WAdSpriteSet^ Value)
			{
				SAFE_DELETE(Native->m_DefaultSpriteSet);
				if(Value!=nullptr) Native->m_DefaultSpriteSet = Value->Native;
			}
		}

		property WSprite^ DefaultSprite
		{
			WSprite^ get()
			{
				if(Native->m_DefaultSprite) return gcnew WSprite(Native->m_DefaultSprite);
				else return nullptr;
			}
			void set(WSprite^ Value)
			{
				SAFE_DELETE(Native->m_DefaultSprite);
				if(Value!=nullptr) Native->m_DefaultSprite = Value->Native;
			}
		}


		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		bool LoadDefaultSprite()
		{
			return SUCCEEDED(Native->LoadDefaultSprite());
		}

		WSprite^ GetDefaultSprite(WEDirection Direction)
		{
			CBSprite* Spr = Native->GetDefaultSprite((TDirection)Direction);
			if(Spr) return gcnew WSprite(Spr);
			else return nullptr;
		}

		WSprite^ GetDefaultSprite()
		{
			return GetDefaultSprite(WEDirection::Down);
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
		property CAdTalkDef* Native
		{
			CAdTalkDef* get() { return (CAdTalkDef*)m_Native; };
		}

	};

}}};

