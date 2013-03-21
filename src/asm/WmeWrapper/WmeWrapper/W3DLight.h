// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WDynArray.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class W3DLight : public WScriptable
	{
	public:
		W3DLight(){};
		W3DLight(WGame^ Game);
		W3DLight(C3DLight* Native);
		virtual ~W3DLight(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property WVector3^ Postition
		{
			WVector3^ get()
			{
				return gcnew WVector3(Native->m_Pos.x, Native->m_Pos.y, Native->m_Pos.z);
			}
			void set(WVector3^ Value)
			{
				Native->m_Pos.x = Value->X;
				Native->m_Pos.y = Value->Y;
				Native->m_Pos.z = Value->Z;
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

		property System::Drawing::Color Color
		{
			System::Drawing::Color get()
			{
				return System::Drawing::Color::FromArgb(Native->m_DiffuseColor);
			}
			void set(System::Drawing::Color Value)
			{
				Native->m_DiffuseColor = Value.ToArgb();
			}
		}

		property bool Spotlight
		{
			bool get()
			{
				return Native->m_IsSpotlight;
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

		property float Falloff
		{
			float get()
			{
				return Native->m_Falloff;
			}
			void set(float Value)
			{
				Native->m_Falloff = Value;
			}
		}


	internal:
		property C3DLight* Native
		{
			C3DLight* get() { return (C3DLight*)m_Native; };
		}

	};

}}};

