// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once


namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;

	ref class WGame;

	public ref class W3DMesh : public WNamedObject
	{
	public:
		W3DMesh(){};
		W3DMesh(WGame^ Game);
		W3DMesh(C3DMesh* Native);
		virtual ~W3DMesh(void);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property bool Visible
		{
			bool get()
			{
				return Native->m_Visible;
			}
			void set(bool Value)
			{
				Native->m_Visible = Value;
			}
		}

		property int MaterialIndex
		{
			int get()
			{
				return Native->m_MaterialIndex;
			}
			void set(int Value)
			{
				Native->m_MaterialIndex = Value;
			}
		}


	internal:
		property C3DMesh* Native
		{
			C3DMesh* get() { return (C3DMesh*)m_Native; };
		}

	};

}}};

