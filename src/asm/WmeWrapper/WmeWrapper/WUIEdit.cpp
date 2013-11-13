// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WUIEdit.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WUIEdit::WUIEdit(WGame^ Game):WUIObject()
{
	m_Game = Game;
	m_Native = new CUIEdit(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WUIEdit::WUIEdit(CUIEdit* Native):WUIObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WUIEdit::~WUIEdit(void)
{
}
