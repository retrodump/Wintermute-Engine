// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WUITiledImage.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WUITiledImage::WUITiledImage(WGame^ Game):WObject()
{
	m_Game = Game;
	m_Native = new CUITiledImage(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WUITiledImage::WUITiledImage(CUITiledImage* Native):WObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WUITiledImage::~WUITiledImage(void)
{
}
