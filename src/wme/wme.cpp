// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include "PlatformWin.h"
#include "dcgf_ad.h"
#include "resource.h"

#ifdef WME_D3D9
	#if _DEBUG
		#pragma comment(lib, "wme_ad_d3d9_d.lib")
	#else
		#pragma comment(lib, "wme_ad_d3d9.lib")
	#endif
#else
	#if _DEBUG
		#pragma comment(lib, "wme_ad_d.lib")
	#else
		#pragma comment(lib, "wme_ad.lib")
	#endif
#endif




int RunGame(CBGame* Game, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow);

//////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
	//_CrtSetBreakAlloc(0);
#endif

	CAdGame* Game = new CAdGame;
	Game->m_SmartCache = true;

	return RunGame(Game, hInstance, hPrevInstance, lpszCmdLine, nCmdShow);
}

//////////////////////////////////////////////////////////////////////////
int RunGame(CBGame* Game, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
#ifndef _DEBUG
	__try
	{
#endif
		if(!CBPlatform::Initialize(Game, hInstance, hPrevInstance, lpszCmdLine, nCmdShow))
			return FALSE;
#ifndef _DEBUG
	}
	__except(CBPlatform::HandleCrashReport(Game, GetExceptionInformation()))
	{
		return FALSE;
	}
#endif
	return CBPlatform::MessageLoop();
}