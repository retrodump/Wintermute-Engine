// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once


namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WAdWaypointGroup : public WObject
	{
	public:
		WAdWaypointGroup(){};
		WAdWaypointGroup(WGame^ Game);
		WAdWaypointGroup(CAdWaypointGroup* Native);
		virtual ~WAdWaypointGroup(void);

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


	internal:
		property CAdWaypointGroup* Native
		{
			CAdWaypointGroup* get() { return (CAdWaypointGroup*)m_Native; };
		}

	};

}}};

