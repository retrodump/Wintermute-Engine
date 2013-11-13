// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WAdRegion.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WAdRegion::WAdRegion(WGame^ Game):WRegion()
{
	m_Game = Game;
	m_Native = new CAdRegion(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WAdRegion::WAdRegion(CAdRegion* Native):WRegion(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WAdRegion::~WAdRegion(void)
{
}
