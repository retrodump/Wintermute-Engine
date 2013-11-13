// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WDynArray.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WUIEntity : public WUIObject
	{
	public:
		WUIEntity(){};
		WUIEntity(WGame^ Game);
		WUIEntity(CUIEntity* Native);
		virtual ~WUIEntity(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WAdEntity^ Entity
		{
			WAdEntity^ get()
			{
				if(Native->m_Entity) return gcnew WAdEntity(Native->m_Entity);
				else return nullptr;
			}
			void set(WAdEntity^ Value)
			{
				if(Native->m_Entity) Native->Game->UnregisterObject(Native->m_Entity);
				if(Value) Native->m_Entity = Value->Native;
				else Native->m_Entity = NULL;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		virtual bool Display(int OffsetX, int OffsetY) override
		{
			return SUCCEEDED(Native->Display(0, 0));
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
		property CUIEntity* Native
		{
			CUIEntity* get() { return (CUIEntity*)m_Native; };
		}

	};

}}};

