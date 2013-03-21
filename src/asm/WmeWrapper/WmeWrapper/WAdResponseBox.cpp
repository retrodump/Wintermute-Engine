// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WAdResponseBox.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WAdResponseBox::WAdResponseBox(WGame^ Game):WObject()
{
	m_Game = Game;
	m_Native = new CAdResponseBox(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WAdResponseBox::WAdResponseBox(CAdResponseBox* Native):WObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WAdResponseBox::~WAdResponseBox(void)
{
}
