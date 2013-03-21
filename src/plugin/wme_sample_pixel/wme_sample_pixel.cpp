#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wme_plugin.h>
#include <stdio.h>
#include "SamplePixel.h"


BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// return some info about this plugin
extern "C" __declspec(dllexport) void GetPluginInfo(SWmePluginInfo* PluginInfo)
{
	strcpy(PluginInfo->Description, "WME sample pixel plugin");
	sprintf(PluginInfo->WmeVersion, "%d.%d.%d", WME_VERSION_MAJOR, WME_VERSION_MINOR, WME_VERSION_BUILD);
	sprintf(PluginInfo->PluginVersion, "%d.%d.%d", 1, 0, 0);
}

//////////////////////////////////////////////////////////////////////////
// tell WME what scripting classes are offered by  this plugin
extern "C" __declspec(dllexport) const char* GetSupportedClasses()
{
	return "SamplePixel";
}

//////////////////////////////////////////////////////////////////////////
// establish the interface between WME and plugin
// i.e. tell WME which functions to call in different situations
extern "C" __declspec(dllexport) void GetClassDefinition(const char* ClassName, SWmeClassDef* ClassDef)
{
	if(strcmp(ClassName, "SamplePixel")==0)
	{
		ClassDef->Construct      = CSamplePixel::Construct;
		ClassDef->ConstructEmpty = CSamplePixel::ConstructEmpty;
		ClassDef->Destruct       = CSamplePixel::Destruct;
		ClassDef->CallMethod     = CSamplePixel::CallMethod;
		ClassDef->SetProperty    = CSamplePixel::SetProperty;
		ClassDef->GetProperty    = CSamplePixel::GetProperty;
		ClassDef->Serialize      = CSamplePixel::Serialize;
		ClassDef->ReceiveEvent   = CSamplePixel::ReceiveEvent;
	}
}
