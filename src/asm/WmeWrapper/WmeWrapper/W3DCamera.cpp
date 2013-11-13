// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "W3DCamera.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
W3DCamera::W3DCamera(WGame^ Game):WNamedObject()
{
	m_Game = Game;
	m_Native = new C3DCamera(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
W3DCamera::W3DCamera(C3DCamera* Native):WNamedObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
W3DCamera::~W3DCamera(void)
{
}
