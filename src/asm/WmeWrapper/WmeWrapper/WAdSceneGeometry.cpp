// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WAdSceneGeometry.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WAdSceneGeometry::WAdSceneGeometry(WGame^ Game):WObject()
{
	m_Game = Game;
	m_Native = new CAdSceneGeometry(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WAdSceneGeometry::WAdSceneGeometry(CAdSceneGeometry* Native):WObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WAdSceneGeometry::~WAdSceneGeometry(void)
{
}
