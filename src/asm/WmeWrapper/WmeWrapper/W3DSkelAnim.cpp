// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "W3DSkelAnim.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
W3DSkelAnim::W3DSkelAnim(WGame^ Game):WNamedObject()
{
	m_Game = Game;
	m_Native = new C3DSkelAnim(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
W3DSkelAnim::W3DSkelAnim(C3DSkelAnim* Native):WNamedObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
W3DSkelAnim::~W3DSkelAnim(void)
{
}
