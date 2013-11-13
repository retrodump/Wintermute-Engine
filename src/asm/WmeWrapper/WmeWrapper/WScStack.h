// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WBase.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WScStack : public WBase
	{
	public:
		WScStack() {};
		WScStack(WGame^ Game);
		WScStack(CScStack* Native);
		virtual ~WScStack(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WDynArray<CScValue*, WScValue>^ Values
		{
			WDynArray<CScValue*, WScValue>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CScValue*, WScValue>(&Native->m_Values);
			}
		}

		property int SP
		{
			int get()
			{
				return Native->m_SP;
			}
		}


	internal:
		property CScStack* Native
		{
			CScStack* get() { return (CScStack*)m_Native; };
		}
	};
}}}
