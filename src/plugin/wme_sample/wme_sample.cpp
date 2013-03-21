#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wme_plugin.h>
#include <stdio.h>
#include "Sample.h"
#include "AnotherSample.h"


BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// return some info about this plugin
extern "C" __declspec(dllexport) void GetPluginInfo(SWmePluginInfo* PluginInfo)
{
	strcpy(PluginInfo->Description, "WME sample plugin");
	sprintf(PluginInfo->WmeVersion, "%d.%d.%d", WME_VERSION_MAJOR, WME_VERSION_MINOR, WME_VERSION_BUILD);
	sprintf(PluginInfo->PluginVersion, "%d.%d.%d", 1, 0, 0);
}

//////////////////////////////////////////////////////////////////////////
// tell WME what scripting classes are offered by this plugin
extern "C" __declspec(dllexport) const char* GetSupportedClasses()
{
	return "Sample,AnotherSample";
}

//////////////////////////////////////////////////////////////////////////
// establish the interface between WME and plugin
// i.e. tell WME which functions to call in different situations
extern "C" __declspec(dllexport) void GetClassDefinition(const char* ClassName, SWmeClassDef* ClassDef)
{
	if(strcmp(ClassName, "Sample")==0)
	{
		ClassDef->Construct      = CSample::Construct;
		ClassDef->ConstructEmpty = CSample::ConstructEmpty;
		ClassDef->Destruct       = CSample::Destruct;
		ClassDef->CallMethod     = CSample::CallMethod;
		ClassDef->SetProperty    = CSample::SetProperty;
		ClassDef->GetProperty    = CSample::GetProperty;
		ClassDef->Serialize      = CSample::Serialize;
		ClassDef->ReceiveEvent   = CSample::ReceiveEvent;
	}
	else if(strcmp(ClassName, "AnotherSample")==0)
	{
		ClassDef->Construct      = CAnotherSample::Construct;
		ClassDef->ConstructEmpty = CAnotherSample::ConstructEmpty;
		ClassDef->Destruct       = CAnotherSample::Destruct;
		ClassDef->CallMethod     = CAnotherSample::CallMethod;
		ClassDef->SetProperty    = CAnotherSample::SetProperty;
		ClassDef->GetProperty    = CAnotherSample::GetProperty;
		ClassDef->Serialize      = CAnotherSample::Serialize;
		ClassDef->ReceiveEvent   = CAnotherSample::ReceiveEvent;
	}
}
