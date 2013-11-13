// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WUIEntity.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WUIEntity::WUIEntity(WGame^ Game):WUIObject()
{
	m_Game = Game;
	m_Native = new CUIEntity(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WUIEntity::WUIEntity(CUIEntity* Native):WUIObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WUIEntity::~WUIEntity(void)
{
}
