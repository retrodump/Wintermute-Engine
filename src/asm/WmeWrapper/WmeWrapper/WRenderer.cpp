// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WRenderer.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WRenderer::WRenderer(WGame^ Game):WBase()
{
	m_Game = Game;	
	m_Native = new CBRenderer(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WRenderer::WRenderer(CBRenderer* Native):WBase(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WRenderer::~WRenderer(void)
{
}
