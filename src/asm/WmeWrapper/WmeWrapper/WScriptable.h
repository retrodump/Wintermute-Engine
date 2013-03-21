// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WNamedObject.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;

	ref class WGame;
	ref class WScValue;

	public ref class WScriptable : public WNamedObject
	{
	public:
		WScriptable(){};
		WScriptable(WGame^ Game);
		WScriptable(CBScriptable* Native);
		virtual ~WScriptable(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WScValue^ Properties
		{
			WScValue^ get();
		}

		property int RefCount
		{
			int get()
			{
				return Native->m_RefCount;
			}
			void set(int value)
			{
				Native->m_RefCount = value;
			}
		}

	internal:
		property CBScriptable* Native
		{
			CBScriptable* get() { return (CBScriptable*)m_Native; };
		}

	};

}}};

