// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "Stdafx.h"
#include "WBase.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WBase::WBase(void)
{
	m_Game = nullptr;
	m_Native = NULL;
	m_SelfDestroy = false;
}

//////////////////////////////////////////////////////////////////////////
WBase::WBase(CBBase* Native)
{
	m_Native = Native;
	m_SelfDestroy = false;
	m_Game = gcnew WGame(m_Native->Game);
}

//////////////////////////////////////////////////////////////////////////
WBase::WBase(WGame^ Game)
{
	m_Game = Game;
	//m_Native = new CBBase(Game->Native);
	m_Native = new CBBase(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WBase::~WBase()
{
	if(m_SelfDestroy) SAFE_DELETE(m_Native);
}

//////////////////////////////////////////////////////////////////////////
IntPtr WBase::NativeObject::get()
{
	return (IntPtr)m_Native;
}

//////////////////////////////////////////////////////////////////////////
WGame^ WBase::Game::get()
{
	return m_Game;
}
