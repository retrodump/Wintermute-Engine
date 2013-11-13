#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wme_plugin.h>
#include <stdio.h>
#include "Snow.h"


//////////////////////////////////////////////////////////////////////////
// the standard Windows DLL entry point
BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    return TRUE;
}


//////////////////////////////////////////////////////////////////////////
// return some info about this plugin
extern "C" __declspec(dllexport) void GetPluginInfo(SWmePluginInfo* PluginInfo)
{
	strcpy(PluginInfo->Description, "Sample plugin - snow generator");
	sprintf(PluginInfo->WmeVersion, "%d.%d.%d", WME_VERSION_MAJOR, WME_VERSION_MINOR, WME_VERSION_BUILD);
	sprintf(PluginInfo->PluginVersion, "%d.%d.%d", 1, 0, 0);
}

//////////////////////////////////////////////////////////////////////////
// tell WME what scripting classes are offered by  this plugin
extern "C" __declspec(dllexport) const char* GetSupportedClasses()
{
	return "Snow";
}

//////////////////////////////////////////////////////////////////////////
// establish the interface between WME and plugin
// i.e. tell WME which functions to call in different situations
extern "C" __declspec(dllexport) void GetClassDefinition(const char* ClassName, SWmeClassDef* ClassDef)
{
	if(strcmp(ClassName, "Snow")==0)
	{
		ClassDef->Construct      = CSnow::Construct;
		ClassDef->ConstructEmpty = CSnow::ConstructEmpty;
		ClassDef->Destruct       = CSnow::Destruct;
		ClassDef->CallMethod     = CSnow::CallMethod;
		ClassDef->SetProperty    = CSnow::SetProperty;
		ClassDef->GetProperty    = CSnow::GetProperty;
		ClassDef->Serialize      = CSnow::Serialize;
		ClassDef->ReceiveEvent   = CSnow::ReceiveEvent;
	}
}
