// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WUIWindow.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WUIWindow::WUIWindow(WGame^ Game):WUIObject()
{
	m_Game = Game;
	m_Native = new CUIWindow(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WUIWindow::WUIWindow(CUIWindow* Native):WUIObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WUIWindow::~WUIWindow(void)
{
}
