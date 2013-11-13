// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WAdLayer.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WAdLayer::WAdLayer(WGame^ Game):WObject()
{
	m_Game = Game;
	m_Native = new CAdLayer(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WAdLayer::WAdLayer(CAdLayer* Native):WObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WAdLayer::~WAdLayer(void)
{
}
