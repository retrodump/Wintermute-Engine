// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WFontBitmap.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WFontBitmap::WFontBitmap(WGame^ Game):WFont()
{
	m_Game = Game;	
	m_Native = new CBFontBitmap(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WFontBitmap::WFontBitmap(CBFontBitmap* Native):WFont(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WFontBitmap::~WFontBitmap(void)
{
}
