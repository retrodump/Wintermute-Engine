// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WDynArray.h"
#include "WRegion.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WAdRegion : public WRegion
	{
	public:
		WAdRegion(){};
		WAdRegion(WGame^ Game);
		WAdRegion(CAdRegion* Native);
		virtual ~WAdRegion(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property System::Drawing::Color Color
		{
			System::Drawing::Color get()
			{
				return System::Drawing::Color::FromArgb(Native->m_Alpha);
			}
			void set(System::Drawing::Color Value)
			{
				Native->m_Alpha = Value.ToArgb();
			}
		}

		property float Scale
		{
			float get()
			{
				return Native->m_Zoom;
			}
			void set(float Value)
			{
				Native->m_Zoom = Value;
			}
		}

		property bool Blocked
		{
			bool get()
			{
				return Native->m_Blocked;
			}
			void set(bool Value)
			{
				Native->m_Blocked = Value;
			}
		}

		property bool Decoration
		{
			bool get()
			{
				return Native->m_Decoration;
			}
			void set(bool Value)
			{
				Native->m_Decoration = Value;
			}
		}

	internal:
		property CAdRegion* Native
		{
			CAdRegion* get() { return (CAdRegion*)m_Native; };
		}

	};

}}};

