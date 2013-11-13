// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WDynBuffer.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WDynBuffer::WDynBuffer(WGame^ Game):WBase()
{
	m_Game = Game;	
	m_Native = new CBDynBuffer(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WDynBuffer::WDynBuffer(CBDynBuffer* Native):WBase(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WDynBuffer::~WDynBuffer(void)
{
}
