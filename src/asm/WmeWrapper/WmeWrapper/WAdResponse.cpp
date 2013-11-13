// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WAdResponse.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WAdResponse::WAdResponse(WGame^ Game):WObject()
{
	m_Game = Game;
	m_Native = new CAdResponse(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WAdResponse::WAdResponse(CAdResponse* Native):WObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WAdResponse::~WAdResponse(void)
{
}
