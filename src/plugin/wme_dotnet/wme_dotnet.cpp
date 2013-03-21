#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wme_plugin.h>
#include <stdio.h>
#include "Wrapper.h"


#pragma unmanaged

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    return TRUE;
}


#pragma managed

//////////////////////////////////////////////////////////////////////////
// return some info about this plugin
extern "C" __declspec(dllexport) void GetPluginInfo(SWmePluginInfo* PluginInfo)
{
	strcpy(PluginInfo->Description, "WME .NET Wrapper");
	sprintf(PluginInfo->WmeVersion, "%d.%d.%d", WME_VERSION_MAJOR, WME_VERSION_MINOR, WME_VERSION_BUILD);
	sprintf(PluginInfo->PluginVersion, "%d.%d.%d", 1, 0, 0);
}

//////////////////////////////////////////////////////////////////////////
// tell WME what scripting classes are offered by this plugin
extern "C" __declspec(dllexport) const char* GetSupportedClasses()
{
	return "DotNet";
}

//////////////////////////////////////////////////////////////////////////
// establish the interface between WME and plugin
// i.e. tell WME which functions to call in different situations
extern "C" __declspec(dllexport) void GetClassDefinition(const char* ClassName, SWmeClassDef* ClassDef)
{
	if(strcmp(ClassName, "DotNet")==0)
	{
		ClassDef->Construct      = CWrapper::Construct;
		ClassDef->ConstructEmpty = CWrapper::ConstructEmpty;
		ClassDef->Destruct       = CWrapper::Destruct;
		ClassDef->CallMethod     = CWrapper::CallMethod;
		ClassDef->SetProperty    = CWrapper::SetProperty;
		ClassDef->GetProperty    = CWrapper::GetProperty;
		ClassDef->Serialize      = CWrapper::Serialize;
		ClassDef->ReceiveEvent   = CWrapper::ReceiveEvent;
	}
}
