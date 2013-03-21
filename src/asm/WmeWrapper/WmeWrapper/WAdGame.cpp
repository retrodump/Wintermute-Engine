// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WAdGame.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WAdGame::WAdGame():WGame(NULL)
{
	m_Game = this;
	m_Native = new CAdGame();
	m_SelfDestroy = true;

	InitializeInternal();
}

//////////////////////////////////////////////////////////////////////////
WAdGame::WAdGame(CAdGame* Native):WGame(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WAdGame::~WAdGame(void)
{
}
