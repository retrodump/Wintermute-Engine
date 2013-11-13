// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "W3DMesh.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
W3DMesh::W3DMesh(WGame^ Game):WNamedObject()
{
	m_Game = Game;
	m_Native = new C3DMesh(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
W3DMesh::W3DMesh(C3DMesh* Native):WNamedObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
W3DMesh::~W3DMesh(void)
{
}
