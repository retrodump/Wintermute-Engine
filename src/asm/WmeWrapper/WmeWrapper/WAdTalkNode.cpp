// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WAdTalkNode.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WAdTalkNode::WAdTalkNode(WGame^ Game):WBase()
{
	m_Game = Game;	
	m_Native = new CAdTalkNode(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WAdTalkNode::WAdTalkNode(CAdTalkNode* Native):WBase(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WAdTalkNode::~WAdTalkNode(void)
{
}
