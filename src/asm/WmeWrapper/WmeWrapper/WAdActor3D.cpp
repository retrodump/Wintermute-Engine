// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WAdActor3D.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WAdActor3D::WAdActor3D(WGame^ Game):WAdObject3D()
{
	m_Game = Game;
	m_Native = new CAdActor3D(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WAdActor3D::WAdActor3D(CAdActor3D* Native):WAdObject3D(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WAdActor3D::~WAdActor3D(void)
{
}
