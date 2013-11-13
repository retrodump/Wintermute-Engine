// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "3DEffect_D3D8.h"

#ifndef WME_D3D9

//////////////////////////////////////////////////////////////////////////
C3DEffect::C3DEffect(CBGame* inGame) : CBBase(inGame)
{
}

//////////////////////////////////////////////////////////////////////////
C3DEffect::~C3DEffect(void)
{
}

//////////////////////////////////////////////////////////////////////////
HRESULT C3DEffect::CreateFromFile(char* Filename)
{
	Game->LOG(0, "Error: Effect files are not supported by Direct3D 8 version of the engine.");
	return E_FAIL;
}

#endif