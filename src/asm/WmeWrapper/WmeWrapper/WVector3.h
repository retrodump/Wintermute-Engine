// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

namespace DeadCode { namespace WME { namespace Core
{

	public ref class WVector3
	{
	public:
		WVector3(void)
		{
			m_X = m_Y = m_Z = 0;
		}

		WVector3(float X, float Y, float Z)
		{
			m_X = X;
			m_Y = Y;
			m_Z = Z;
		}

		~WVector3(void){};

		property float X
		{
			float get()
			{
				return m_X;
			}
		}

		property float Y
		{
			float get()
			{
				return m_Y;
			}
		}

		property float Z
		{
			float get()
			{
				return m_Z;
			}
		}

	private:
		float m_X;
		float m_Y;
		float m_Z;
	};
}}};

