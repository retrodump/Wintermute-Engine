// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WScriptable.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WScriptable::WScriptable(WGame^ Game):WNamedObject()
{
	m_Game = Game;
	m_Native = new CBScriptable(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WScriptable::WScriptable(CBScriptable* Native):WNamedObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WScriptable::~WScriptable(void)
{
}

//////////////////////////////////////////////////////////////////////////
WScValue^ WScriptable::Properties::get()
{
	if(!Native->m_ScProp) Native->m_ScProp = new CScValue(Native->Game);
	return gcnew WScValue(Native->m_ScProp);
}
