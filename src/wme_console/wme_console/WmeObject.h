// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Collections::Generic;
using namespace DeadCode::WME::Debugger;


namespace wme_console
{
	public ref class WmeObject : public IWmeObject
	{
	public:
		//////////////////////////////////////////////////////////////////////////
		WmeObject(IWmeDebugObject* Native);

		//////////////////////////////////////////////////////////////////////////
		virtual property String^ NativeClass
		{
			String^ get();
		}

		//////////////////////////////////////////////////////////////////////////
		virtual IWmeValue^ GetProperty(String^ Name);

	private:
		IWmeDebugObject* m_Native;
	};
}

