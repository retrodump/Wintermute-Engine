// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once


namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WPoint : public WBase
	{
	public:
		WPoint() {};
		WPoint(int X, int Y);
		WPoint(CBPoint* Native);
		virtual ~WPoint(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property int X
		{
			int get()
			{
				return Native->x;
			}
			void set(int Value)
			{
				Native->x = Value;
			}
		}

		property int Y
		{
			int get()
			{
				return Native->y;
			}
			void set(int Value)
			{
				Native->y = Value;
			}
		}

	internal:
		property CBPoint* Native
		{
			CBPoint* get() { return (CBPoint*)m_Native; };
		}
	};
}}}
