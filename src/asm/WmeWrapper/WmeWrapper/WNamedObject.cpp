// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WNamedObject.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WNamedObject::WNamedObject(WGame^ Game):WBase()
{
	m_Game = Game;	
	m_Native = new CBNamedObject(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WNamedObject::WNamedObject(CBNamedObject* Native):WBase(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WNamedObject::~WNamedObject(void)
{
}

//////////////////////////////////////////////////////////////////////////
String^ WNamedObject::Name::get()
{
	return gcnew String(Native->m_Name);
}

//////////////////////////////////////////////////////////////////////////
void WNamedObject::Name::set(String^ value)
{
	const char* S = WUtils::GetString(value);
	CBUtils::SetString(&Native->m_Name, (char*)S);
	WUtils::FreeString(S);
}
