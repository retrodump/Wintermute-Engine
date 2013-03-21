// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WAdInventoryBox.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WAdInventoryBox::WAdInventoryBox(WGame^ Game):WObject()
{
	m_Game = Game;
	m_Native = new CAdInventoryBox(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WAdInventoryBox::WAdInventoryBox(CAdInventoryBox* Native):WObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WAdInventoryBox::~WAdInventoryBox(void)
{
}
