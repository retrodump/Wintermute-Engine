// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WUIButton.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WUIButton::WUIButton(WGame^ Game):WUIObject()
{
	m_Game = Game;
	m_Native = new CUIButton(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WUIButton::WUIButton(CUIButton* Native):WUIObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WUIButton::~WUIButton(void)
{
}
