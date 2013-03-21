// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once


namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WAdRotLevel : public WObject
	{
	public:
		WAdRotLevel(){};
		WAdRotLevel(WGame^ Game);
		WAdRotLevel(CAdRotLevel* Native);
		virtual ~WAdRotLevel(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property float Rotation
		{
			float get()
			{
				return Native->m_Rotation;
			}
			void set(float Value)
			{
				Native->m_Rotation = Value;
			}
		}


	internal:
		property CAdRotLevel* Native
		{
			CAdRotLevel* get() { return (CAdRotLevel*)m_Native; };
		}

	};

}}};

