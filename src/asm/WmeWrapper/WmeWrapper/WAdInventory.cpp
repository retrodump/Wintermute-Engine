// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WAdInventory.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WAdInventory::WAdInventory(WGame^ Game):WObject()
{
	m_Game = Game;
	m_Native = new CAdInventory(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WAdInventory::WAdInventory(CAdInventory* Native):WObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WAdInventory::~WAdInventory(void)
{
}
