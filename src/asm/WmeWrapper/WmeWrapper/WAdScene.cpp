// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WAdScene.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WAdScene::WAdScene(WGame^ Game):WObject()
{
	m_Game = Game;
	m_Native = new CAdScene(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WAdScene::WAdScene(CAdScene* Native):WObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WAdScene::~WAdScene(void)
{
}
