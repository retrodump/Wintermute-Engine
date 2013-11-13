// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WDynStringArray.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WPlugin : public WBase
	{
	public:
		WPlugin() {};
		WPlugin(WGame^ Game);
		WPlugin(CBPlugin* Native);
		virtual ~WPlugin(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WDynStringArray^ Classes
		{
			WDynStringArray^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynStringArray(&Native->m_Classes);
			}
		}

		property String^ DllPath
		{
			String^ get()
			{
				if(Native->m_DllPath) return gcnew String(Native->m_DllPath);
				else return nullptr;
			}
		}

		property IntPtr DllHandle
		{
			IntPtr get()
			{
				return IntPtr(Native->m_DllHandle);
			}
		}

		property String^ Description
		{
			String^ get()
			{
				return gcnew String(Native->m_PluginInfo.Description);
			}
		}

		property String^ Version
		{
			String^ get()
			{
				return gcnew String(Native->m_PluginInfo.PluginVersion);
			}
		}

		property String^ WmeVersion
		{
			String^ get()
			{
				return gcnew String(Native->m_PluginInfo.WmeVersion);
			}
		}


	internal:
		property CBPlugin* Native
		{
			CBPlugin* get() { return (CBPlugin*)m_Native; };
		}
	};
}}}
