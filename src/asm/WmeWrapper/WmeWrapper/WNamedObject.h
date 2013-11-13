// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once


namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WNamedObject : public WBase
	{
	public:
		WNamedObject() {};
		WNamedObject(WGame^ Game);
		WNamedObject(CBNamedObject* Native);
		virtual ~WNamedObject(void);

		property String^ Name
		{
			String^ get();
			void set(String^ value);
		}
	internal:
		property CBNamedObject* Native
		{
			CBNamedObject* get() { return (CBNamedObject*)m_Native; };
		}
	};
}}}
