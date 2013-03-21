// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "3DKeyframe.h"


//////////////////////////////////////////////////////////////////////////
C3DKeyframe::C3DKeyframe()
{
	m_JointIndex = -1;
	m_Time = 0.0f;
	m_Parameter = D3DXVECTOR3(0, 0, 0);
}


//////////////////////////////////////////////////////////////////////////
C3DKeyframe::~C3DKeyframe()
{

}
