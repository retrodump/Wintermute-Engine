// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WPlugin.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WPlugin::WPlugin(WGame^ Game):WBase()
{
	m_Game = Game;	
	m_Native = new CBPlugin(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WPlugin::WPlugin(CBPlugin* Native):WBase(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WPlugin::~WPlugin(void)
{
}
