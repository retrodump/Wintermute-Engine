// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WSubFrame.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WSubFrame::WSubFrame(WGame^ Game):WScriptable()
{
	m_Game = Game;
	m_Native = new CBSubFrame(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WSubFrame::WSubFrame(CBSubFrame* Native):WScriptable(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WSubFrame::~WSubFrame(void)
{
}

//////////////////////////////////////////////////////////////////////////
bool WSubFrame::Display(int X, int Y, WObject^ Register, float ScaleX, float ScaleY, bool PixelPerfect, System::Drawing::Color Color, float Rotate)
{
	CBObject* Reg = NULL;
	if(Register!=nullptr) Reg = Register->Native;

	return SUCCEEDED(Native->Draw(X, Y, Reg, ScaleX, ScaleY, PixelPerfect, Color.ToArgb(), Rotate));
}
