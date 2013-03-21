// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WAdActor.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WAdActor::WAdActor(WGame^ Game):WAdTalkHolder()
{
	m_Game = Game;
	m_Native = new CAdActor(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WAdActor::WAdActor(CAdActor* Native):WAdTalkHolder(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WAdActor::~WAdActor(void)
{
}
