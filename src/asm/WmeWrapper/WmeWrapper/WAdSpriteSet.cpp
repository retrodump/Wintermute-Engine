// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WAdSpriteSet.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WAdSpriteSet::WAdSpriteSet(WGame^ Game):WObject()
{
	m_Game = Game;
	m_Native = new CAdSpriteSet(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WAdSpriteSet::WAdSpriteSet(CAdSpriteSet* Native):WObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WAdSpriteSet::~WAdSpriteSet(void)
{
}
