// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WScValue.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WScValue::WScValue(WGame^ Game):WBase()
{
	m_Game = Game;	
	m_Native = new CScValue(Game->Native);
	m_SelfDestroy = true;
	PropertyName = nullptr;
}

//////////////////////////////////////////////////////////////////////////
WScValue::WScValue(CScValue* Native):WBase(Native)
{
	PropertyName = nullptr;
}

//////////////////////////////////////////////////////////////////////////
WScValue::~WScValue(void)
{
}
