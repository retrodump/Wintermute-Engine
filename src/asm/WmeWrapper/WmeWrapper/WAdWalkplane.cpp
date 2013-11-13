// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WAdWalkplane.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WAdWalkplane::WAdWalkplane(WGame^ Game):WScriptable()
{
	m_Game = Game;
	m_Native = new CAdWalkplane(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WAdWalkplane::WAdWalkplane(CAdWalkplane* Native):WScriptable(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WAdWalkplane::~WAdWalkplane(void)
{
}
