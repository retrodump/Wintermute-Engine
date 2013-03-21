// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WSound.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WSound::WSound(WGame^ Game):WBase()
{
	m_Game = Game;	
	m_Native = new CBSound(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WSound::WSound(CBSound* Native):WBase(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WSound::~WSound(void)
{
}
