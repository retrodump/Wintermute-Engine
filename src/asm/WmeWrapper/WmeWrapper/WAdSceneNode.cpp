// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WAdSceneNode.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WAdSceneNode::WAdSceneNode(WGame^ Game):WObject()
{
	m_Game = Game;
	m_Native = new CAdSceneNode(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WAdSceneNode::WAdSceneNode(CAdSceneNode* Native):WObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WAdSceneNode::~WAdSceneNode(void)
{
}
