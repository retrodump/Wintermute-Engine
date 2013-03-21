// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WAdEntity.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WAdEntity::WAdEntity(WGame^ Game):WAdTalkHolder()
{
	m_Game = Game;
	m_Native = new CAdEntity(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WAdEntity::WAdEntity(CAdEntity* Native):WAdTalkHolder(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WAdEntity::~WAdEntity(void)
{
}
