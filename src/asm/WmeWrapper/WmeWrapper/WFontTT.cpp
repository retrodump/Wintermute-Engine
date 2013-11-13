// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WFontTT.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WFontTT::WFontTT(WGame^ Game):WFont()
{
	m_Game = Game;	
	m_Native = new CBFontTT(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WFontTT::WFontTT(CBFontTT* Native):WFont(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WFontTT::~WFontTT(void)
{
}
