// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WAdRotLevel.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WAdRotLevel::WAdRotLevel(WGame^ Game):WObject()
{
	m_Game = Game;
	m_Native = new CAdRotLevel(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WAdRotLevel::WAdRotLevel(CAdRotLevel* Native):WObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WAdRotLevel::~WAdRotLevel(void)
{
}
