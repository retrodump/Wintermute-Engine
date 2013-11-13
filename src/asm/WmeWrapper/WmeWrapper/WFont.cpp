// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "StdAfx.h"
#include "WFont.h"
#include "WUtils.h"
#include "WFontBitmap.h"
#include "WFontTT.h"

using namespace System;
using namespace DeadCode::WME::Core;


//////////////////////////////////////////////////////////////////////////
WFont::WFont(WGame^ Game):WObject()
{
	m_Game = Game;
	m_Native = new CBFont(Game->Native);
	m_SelfDestroy = true;
}

//////////////////////////////////////////////////////////////////////////
WFont::WFont(CBFont* Native):WObject(Native)
{
}

//////////////////////////////////////////////////////////////////////////
WFont::~WFont(void)
{
}


//////////////////////////////////////////////////////////////////////////
WFont^ WFont::CreateFromFile(WGame^ Game, String^ Filename)
{
	char* S = (char*)WUtils::GetString(Filename);
	CBFont* Font = CBFont::CreateFromFile(Game->Native, S);
	WUtils::FreeString((char*)S);
	if(!Font) return nullptr;

	if(stricmp(Font->GetClassName(), "CBFontTT")==0) return gcnew WFontTT((CBFontTT*)Font);
	else return gcnew WFontBitmap((CBFontBitmap*)Font);
}
