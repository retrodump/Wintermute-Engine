// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once


namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	//////////////////////////////////////////////////////////////////////////
	public ref class WWapointsArray3D
	{
	internal:
		WWapointsArray3D(CAdWaypointGroup3D* Native)
		{
			m_Native = Native;
		}
	public:
		property virtual int Count
		{
			int get()
			{
				return m_Native->m_Points.GetSize();
			}
		}

		property WVector3^ default[int]
		{
			WVector3^ get(int Index)
			{
				if(Index < 0 || Index >= m_Native->m_Points.GetSize())
					throw gcnew System::ArgumentOutOfRangeException("Index", "Index is out of range");

				return gcnew WVector3(m_Native->m_Points[Index]->x, m_Native->m_Points[Index]->y, m_Native->m_Points[Index]->z);
			}
		}
	private:
		CAdWaypointGroup3D* m_Native;
	};


	//////////////////////////////////////////////////////////////////////////
	public ref class WAdWaypointGroup3D : public WBase
	{
	public:
		WAdWaypointGroup3D(){};
		WAdWaypointGroup3D(WGame^ Game);
		WAdWaypointGroup3D(CAdWaypointGroup3D* Native);
		virtual ~WAdWaypointGroup3D(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
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

		property WWapointsArray3D^ Points
		{
			WWapointsArray3D^ get()
			{
				return gcnew WWapointsArray3D(Native);
			}
		}


	internal:
		property CAdWaypointGroup3D* Native
		{
			CAdWaypointGroup3D* get() { return (CAdWaypointGroup3D*)m_Native; };
		}

	};

}}};
