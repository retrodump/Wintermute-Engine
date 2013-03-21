// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WAdTalkHolder.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WAdTalkHolder::WAdTalkHolder(WGame^ Game):WAdObject()
{
	m_Game = Game;
	m_Native = new CAdTalkHolder(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WAdTalkHolder::WAdTalkHolder(CAdTalkHolder* Native):WAdObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WAdTalkHolder::~WAdTalkHolder(void)
{
}
