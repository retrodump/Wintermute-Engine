// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WRegion.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WRegion::WRegion(WGame^ Game):WObject()
{
	m_Game = Game;
	m_Native = new CBRegion(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WRegion::WRegion(CBRegion* Native):WObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WRegion::~WRegion(void)
{
}
