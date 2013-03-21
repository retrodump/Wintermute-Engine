// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WPluginMgr.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WPluginMgr::WPluginMgr(WGame^ Game):WBase()
{
	m_Game = Game;	
	m_Native = new CBPluginMgr(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WPluginMgr::WPluginMgr(CBPluginMgr* Native):WBase(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WPluginMgr::~WPluginMgr(void)
{
}
