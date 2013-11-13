// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WScStack.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WScStack::WScStack(WGame^ Game):WBase()
{
	m_Game = Game;	
	m_Native = new CScStack(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WScStack::WScStack(CScStack* Native):WBase(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WScStack::~WScStack(void)
{
}
