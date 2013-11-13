// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WScEngine.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WScEngine::WScEngine(WGame^ Game):WBase()
{
	m_Game = Game;
	m_Native = new CScEngine(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WScEngine::WScEngine(CScEngine* Native):WBase(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WScEngine::~WScEngine(void)
{
}
