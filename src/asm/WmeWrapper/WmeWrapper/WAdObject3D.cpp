// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WAdObject3D.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WAdObject3D::WAdObject3D(WGame^ Game):WAdObject()
{
	m_Game = Game;
	m_Native = new CAdObject3D(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WAdObject3D::WAdObject3D(CAdObject3D* Native):WAdObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WAdObject3D::~WAdObject3D(void)
{
}
