// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WPoint.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WPoint::WPoint(int X, int Y)
{
	m_Game = nullptr;
	m_Native = new CBPoint(X, Y);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WPoint::WPoint(CBPoint* Native):WBase(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WPoint::~WPoint(void)
{
}
