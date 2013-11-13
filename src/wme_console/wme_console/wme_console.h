// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once

#include "DebugClient.h"

using namespace System;

extern "C" __declspec(dllexport) bool Initialize (IWmeDebugServer* Server);
extern "C" __declspec(dllexport) bool Shutdown (IWmeDebugServer* Server);

CDebugClient* g_DebugClient;

namespace wme_console
{
}
