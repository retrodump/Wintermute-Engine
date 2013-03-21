// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WScScript.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WScScript::WScScript(CScScript* Native):WBase(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WScScript::~WScScript(void)
{
}


//////////////////////////////////////////////////////////////////////////
WScEngine^ WScScript::ScriptEngine::get()
{
	if(Native->m_Engine) return gcnew WScEngine(Native->m_Engine);
	else return nullptr;
}


//////////////////////////////////////////////////////////////////////////
WScriptHolder^ WScScript::Owner::get()
{
	if(Native->m_Owner) return gcnew WScriptHolder(Native->m_Owner);
	else return nullptr;
}


//////////////////////////////////////////////////////////////////////////
bool WScScript::WaitFor(WObject^ Object)
{
	if(Object==nullptr) return false;
	else return SUCCEEDED(Native->WaitFor(Object->Native));
}

//////////////////////////////////////////////////////////////////////////
bool WScScript::WaitForExclusive(WObject^ Object)
{
	if(Object==nullptr) return false;
	else return SUCCEEDED(Native->WaitForExclusive(Object->Native));
}
