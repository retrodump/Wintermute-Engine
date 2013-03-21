// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WAdItem.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WAdItem::WAdItem(WGame^ Game):WAdTalkHolder()
{
	m_Game = Game;
	m_Native = new CAdItem(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WAdItem::WAdItem(CAdItem* Native):WAdTalkHolder(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WAdItem::~WAdItem(void)
{
}
