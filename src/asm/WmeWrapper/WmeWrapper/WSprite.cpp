// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WSprite.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WSprite::WSprite(WGame^ Game):WScriptHolder()
{
	m_Game = Game;
	m_Native = new CBSprite(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WSprite::WSprite(CBSprite* Native):WScriptHolder(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WSprite::~WSprite(void)
{
}

//////////////////////////////////////////////////////////////////////////
WObject^ WSprite::Owner::get()
{
	if(!Native->m_Owner) return nullptr;
	else return WUtils::CastObject(Native->m_Owner);
}

//////////////////////////////////////////////////////////////////////////
void WSprite::Owner::set(WObject^ Value)
{
	if(Value==nullptr) Native->m_Owner = NULL;
	else Native->m_Owner = Value->Native;
}

//////////////////////////////////////////////////////////////////////////
bool WSprite::Display(int X, int Y, WObject^ Register, float ScaleX, float ScaleY, System::Drawing::Color Color, float Rotate)
{
	CBObject* Reg = NULL;
	if(Register!=nullptr) Reg = Register->Native;

	return SUCCEEDED(Native->Display(X, Y, Reg, ScaleX, ScaleY, Color.ToArgb(), Rotate));
}
