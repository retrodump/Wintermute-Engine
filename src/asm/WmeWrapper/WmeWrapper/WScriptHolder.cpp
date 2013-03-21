// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WScriptHolder.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WScriptHolder::WScriptHolder(WGame^ Game):WScriptable()
{
	m_Game = Game;
	m_Native = new CBScriptHolder(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WScriptHolder::WScriptHolder(CBScriptHolder* Native):WScriptable(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WScriptHolder::~WScriptHolder(void)
{
}

//////////////////////////////////////////////////////////////////////////
String^ WScriptHolder::Filename::get()
{
	return gcnew String(Native->m_Filename);
}

//////////////////////////////////////////////////////////////////////////
void WScriptHolder::Filename::set(String^ value)
{
	const char* S = WUtils::GetString(value);
	CBUtils::SetString(&Native->m_Filename, (char*)S);
	WUtils::FreeString(S);
}
