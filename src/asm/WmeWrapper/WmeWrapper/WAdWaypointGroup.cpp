// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WAdWaypointGroup.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WAdWaypointGroup::WAdWaypointGroup(WGame^ Game):WObject()
{
	m_Game = Game;
	m_Native = new CAdWaypointGroup(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WAdWaypointGroup::WAdWaypointGroup(CAdWaypointGroup* Native):WObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WAdWaypointGroup::~WAdWaypointGroup(void)
{
}
