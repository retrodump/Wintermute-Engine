// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WAdWaypointGroup3D.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WAdWaypointGroup3D::WAdWaypointGroup3D(WGame^ Game):WBase()
{
	m_Game = Game;
	m_Native = new CAdWaypointGroup3D(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WAdWaypointGroup3D::WAdWaypointGroup3D(CAdWaypointGroup3D* Native):WBase(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WAdWaypointGroup3D::~WAdWaypointGroup3D(void)
{
}
