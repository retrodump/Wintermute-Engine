// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "stdafx.h"
#include "wme_console.h"

using namespace wme_console;

#pragma unmanaged


//////////////////////////////////////////////////////////////////////////
BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	return TRUE;
}

#pragma managed
 
//////////////////////////////////////////////////////////////////////////
bool Initialize (IWmeDebugServer* Server)
{
	g_DebugClient = new CDebugClient(Server);
	Server->AttachClient(g_DebugClient);
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool Shutdown (IWmeDebugServer* Server)
{
	if(g_DebugClient)
	{
		Server->DetachClient(g_DebugClient);
		delete g_DebugClient;
		g_DebugClient = NULL;
	}
	return true;
}
