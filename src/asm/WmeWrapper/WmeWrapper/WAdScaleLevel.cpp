// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WAdScaleLevel.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WAdScaleLevel::WAdScaleLevel(WGame^ Game):WObject()
{
	m_Game = Game;
	m_Native = new CAdScaleLevel(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WAdScaleLevel::WAdScaleLevel(CAdScaleLevel* Native):WObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WAdScaleLevel::~WAdScaleLevel(void)
{
}
