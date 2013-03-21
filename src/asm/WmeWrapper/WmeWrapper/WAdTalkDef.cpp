// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WAdTalkDef.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WAdTalkDef::WAdTalkDef(WGame^ Game):WObject()
{
	m_Game = Game;
	m_Native = new CAdTalkDef(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WAdTalkDef::WAdTalkDef(CAdTalkDef* Native):WObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WAdTalkDef::~WAdTalkDef(void)
{
}
