// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WAdBlock.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WAdBlock::WAdBlock(WGame^ Game):WScriptable()
{
	m_Game = Game;
	m_Native = new CAdBlock(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WAdBlock::WAdBlock(CAdBlock* Native):WScriptable(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WAdBlock::~WAdBlock(void)
{
}
