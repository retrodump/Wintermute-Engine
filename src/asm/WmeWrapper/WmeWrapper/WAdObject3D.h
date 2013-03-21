// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once


namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WAdObject3D : public WAdObject
	{
	public:
		WAdObject3D(){};
		WAdObject3D(WGame^ Game);
		WAdObject3D(CAdObject3D* Native);
		virtual ~WAdObject3D(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property bool DropToFloor
		{
			bool get()
			{
				return Native->m_DropToFloor;
			}
			void set(bool Value)
			{
				Native->m_DropToFloor = Value;
			}
		}

		property float Velocity
		{
			float get()
			{
				return Native->m_Velocity;
			}
			void set(float Value)
			{
				Native->m_Velocity = Value;
			}
		}

		property float AngularVelocity
		{
			float get()
			{
				return Native->m_AngVelocity;
			}
			void set(float Value)
			{
				Native->m_AngVelocity = Value;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		bool SkipTo(int X, int Y, bool Tolerant)
		{
			return SUCCEEDED(Native->SkipTo(X, Y, Tolerant));
		}

		bool SkipTo(int X, int Y)
		{
			return SkipTo(X, Y, false);
		}

		virtual bool Update() override
		{
			return SUCCEEDED(Native->Update());
		}

		virtual bool Display() override
		{
			return SUCCEEDED(Native->Display());
		}

	internal:
		property CAdObject3D* Native
		{
			CAdObject3D* get() { return (CAdObject3D*)m_Native; };
		}

	};

}}};

