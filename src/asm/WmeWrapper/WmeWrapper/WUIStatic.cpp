// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WUIStatic.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WUIStatic::WUIStatic(WGame^ Game):WUIObject()
{
	m_Game = Game;
	m_Native = new CUIText(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WUIStatic::WUIStatic(CUIText* Native):WUIObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WUIStatic::~WUIStatic(void)
{
}
