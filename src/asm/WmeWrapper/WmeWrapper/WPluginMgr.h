// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once


namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WPluginMgr : public WBase
	{
	public:
		WPluginMgr() {};
		WPluginMgr(WGame^ Game);
		WPluginMgr(CBPluginMgr* Native);
		virtual ~WPluginMgr(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WDynArray<CBPlugin*, WPlugin>^ Plugins
		{
			WDynArray<CBPlugin*, WPlugin>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CBPlugin*, WPlugin>(&Native->m_Plugins);
			}
		}


	internal:
		property CBPluginMgr* Native
		{
			CBPluginMgr* get() { return (CBPluginMgr*)m_Native; };
		}
	};
}}}
