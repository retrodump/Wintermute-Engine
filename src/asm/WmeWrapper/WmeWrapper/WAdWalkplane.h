// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WDynArray.h"

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public ref class WAdWalkplane : public WScriptable
	{
	public:
		WAdWalkplane(){};
		WAdWalkplane(WGame^ Game);
		WAdWalkplane(CAdWalkplane* Native);
		virtual ~WAdWalkplane(void);

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

		property W3DMesh^ Mesh
		{
			W3DMesh^ get()
			{
				if(Native->m_Mesh) return gcnew W3DMesh(Native->m_Mesh);
				else return nullptr;
			}
		}

	internal:
		property CAdWalkplane* Native
		{
			CAdWalkplane* get() { return (CAdWalkplane*)m_Native; };
		}

	};

}}};

