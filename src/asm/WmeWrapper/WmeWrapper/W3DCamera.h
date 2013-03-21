// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once


namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;

	ref class WGame;

	public ref class W3DCamera : public WNamedObject
	{
	public:
		W3DCamera(){};
		W3DCamera(WGame^ Game);
		W3DCamera(C3DCamera* Native);
		virtual ~W3DCamera(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WVector3^ Postition
		{
			WVector3^ get()
			{
				return gcnew WVector3(Native->m_Position.x, Native->m_Position.y, Native->m_Position.z);
			}
			void set(WVector3^ Value)
			{
				Native->m_Position.x = Value->X;
				Native->m_Position.y = Value->Y;
				Native->m_Position.z = Value->Z;
			}
		}

		property WVector3^ Target
		{
			WVector3^ get()
			{
				return gcnew WVector3(Native->m_Target.x, Native->m_Target.y, Native->m_Target.z);
			}
			void set(WVector3^ Value)
			{
				Native->m_Target.x = Value->X;
				Native->m_Target.y = Value->Y;
				Native->m_Target.z = Value->Z;
			}
		}

		property float Bank
		{
			float get()
			{
				return Native->m_Bank;
			}
			void set(float Value)
			{
				Native->m_Bank = Value;
			}
		}

		property float FOV
		{
			float get()
			{
				return Native->m_FOV;
			}
			void set(float Value)
			{
				Native->m_FOV = Value;
			}
		}

		property float OrigFOV
		{
			float get()
			{
				return Native->m_OrigFOV;
			}
			void set(float Value)
			{
				Native->m_OrigFOV = Value;
			}
		}

		property float NearClippingPlane
		{
			float get()
			{
				return Native->m_NearClipPlane;
			}
			void set(float Value)
			{
				Native->m_NearClipPlane = Value;
			}
		}

		property float FarClippingPlane
		{
			float get()
			{
				return Native->m_FarClipPlane;
			}
			void set(float Value)
			{
				Native->m_FarClipPlane = Value;
			}
		}


	internal:
		property C3DCamera* Native
		{
			C3DCamera* get() { return (C3DCamera*)m_Native; };
		}

	};

}}};

