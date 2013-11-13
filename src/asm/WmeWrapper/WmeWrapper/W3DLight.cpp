// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "W3DLight.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
W3DLight::W3DLight(WGame^ Game):WScriptable()
{
	m_Game = Game;
	m_Native = new C3DLight(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
W3DLight::W3DLight(C3DLight* Native):WScriptable(Native)
{
}

//////////////////////////////////////////////////////////////////////////
W3DLight::~W3DLight(void)
{
}
