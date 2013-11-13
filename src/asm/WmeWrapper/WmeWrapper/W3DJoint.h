// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

namespace DeadCode { namespace WME { namespace Core
{

	public ref class W3DJoint
	{
	internal:
		W3DJoint(C3DJoint* Native)
		{
			m_Native = Native;
		}

		property String^ Name
		{
			String^ get()
			{
				if(m_Native->m_Name) return gcnew String(m_Native->m_Name);
				else return nullptr;
			}
		}

	private:
		C3DJoint* m_Native;
	};
}}};
