// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WDynArray.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WRegion : public WObject
	{
	public:
		WRegion(){};
		WRegion(WGame^ Game);
		WRegion(CBRegion* Native);
		virtual ~WRegion(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WDynArray<CBPoint*, WPoint>^ Points
		{
			WDynArray<CBPoint*, WPoint>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CBPoint*, WPoint>(&Native->m_Points);
			}
		}

		property System::Drawing::Rectangle BoundingRectangle
		{
			System::Drawing::Rectangle get()
			{
				RECT rc;
				Native->GetBoundingRect(&rc);
				return System::Drawing::Rectangle::FromLTRB(rc.left, rc.top, rc.right, rc.bottom);
			}
		}

		property bool Active
		{
			bool get()
			{
				return Native->m_Active;
			}
			void set(bool Value)
			{
				Native->m_Active = Value;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		bool Contains(System::Drawing::Point Point)
		{
			return Native->PointInRegion(Point.X, Point.Y);
		}

		bool Contains(int X, int Y)
		{
			return Native->PointInRegion(X, Y);
		}

	internal:
		property CBRegion* Native
		{
			CBRegion* get() { return (CBRegion*)m_Native; };
		}

	};

}}};

