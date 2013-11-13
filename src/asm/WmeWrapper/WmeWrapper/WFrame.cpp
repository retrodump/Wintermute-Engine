// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WFrame.h"
#include "WUtils.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WFrame::WFrame(WGame^ Game):WScriptable()
{
	m_Game = Game;
	m_Native = new CBFrame(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WFrame::WFrame(CBFrame* Native):WScriptable(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WFrame::~WFrame(void)
{
}


//////////////////////////////////////////////////////////////////////////
WSound^ WFrame::Sound::get()
{
	if(Native->m_Sound) return gcnew WSound(Native->m_Sound);
	else return nullptr;
}

//////////////////////////////////////////////////////////////////////////
void WFrame::Sound::set(WSound^ Value)
{
	SAFE_DELETE(Native->m_Sound);
	if(Value!=nullptr) Native->m_Sound = Value->Native;
}

//////////////////////////////////////////////////////////////////////////
bool WFrame::Display(int X, int Y, WObject^ Register, float ScaleX, float ScaleY, bool PixelPerfect, System::Drawing::Color Color, bool AllFrames, float Rotate)
{
	CBObject* Reg = NULL;
	if(Register!=nullptr) Reg = Register->Native;

	return SUCCEEDED(Native->Draw(X, Y, Reg, ScaleX, ScaleY, PixelPerfect, Color.ToArgb(), AllFrames, Rotate));
}
