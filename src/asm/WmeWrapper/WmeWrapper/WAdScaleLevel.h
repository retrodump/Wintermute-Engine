// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once


namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WAdScaleLevel : public WObject
	{
	public:
		WAdScaleLevel(){};
		WAdScaleLevel(WGame^ Game);
		WAdScaleLevel(CAdScaleLevel* Native);
		virtual ~WAdScaleLevel(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property float Scale
		{
			float get()
			{
				return Native->m_Scale;
			}
			void set(float Value)
			{
				Native->m_Scale = Value;
			}
		}

	internal:
		property CAdScaleLevel* Native
		{
			CAdScaleLevel* get() { return (CAdScaleLevel*)m_Native; };
		}

	};

}}};

