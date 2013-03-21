// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "W3DModel.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
W3DModel::W3DModel(WGame^ Game, WObject^ Owner):WObject()
{
	m_Game = Game;

	CBObject* ObjOwner = NULL;
	if(Owner!=nullptr) ObjOwner = Owner->Native;

	m_Native = new C3DModel(Game->Native, ObjOwner);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
W3DModel::W3DModel(C3DModel* Native):WObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
W3DModel::~W3DModel(void)
{
}
