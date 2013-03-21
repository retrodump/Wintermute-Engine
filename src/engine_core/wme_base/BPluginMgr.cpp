// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include <io.h>
#include ".\BPluginMgr.h"


//////////////////////////////////////////////////////////////////////////
CBPluginMgr::CBPluginMgr(CBGame* inGame):CBBase(inGame)
{
	m_IsDisposing = false;
}

//////////////////////////////////////////////////////////////////////////
CBPluginMgr::~CBPluginMgr(void)
{
	int i;
	m_IsDisposing = true;

	// destroy plugin objects
	for(i=0; i<m_Objects.GetSize(); i++) delete m_Objects[i];
	m_Objects.RemoveAll();

	for(i=0; i<m_Plugins.GetSize(); i++) delete m_Plugins[i];
	m_Plugins.RemoveAll();

	m_IsDisposing = false;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBPluginMgr::RegisterPlugins()
{
	Game->LOG(0, "Loading plugins...");

	Game->m_FileManager->RestoreCurrentDir();

	char ExePath[MAX_PATH];
	char CurrPath[MAX_PATH];

#ifdef _UNICODE
	wchar_t ExePathW[MAX_PATH];
	GetModuleFileName(NULL, ExePathW, MAX_PATH);
	WideCharToMultiByte(CP_ACP, 0, ExePathW, -1, ExePath, MAX_PATH, NULL, NULL);
#else
	GetModuleFileName(NULL, ExePath, MAX_PATH);
#endif

	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	_splitpath(ExePath, drive, dir, NULL, NULL);
	sprintf(ExePath, "%s%s", drive, dir);

#ifndef _WIN32_WINCE
	GetCurrentDirectory(MAX_PATH, CurrPath);
	if(strlen(CurrPath)>0 && CurrPath[strlen(CurrPath)-1]!='\\') strcat(CurrPath, "\\");
#else
	strcpy(CurrPath, ExePath);
#endif

	// register plugins in all paths
	RegisterPlugins(ExePath);
	if(stricmp(ExePath, CurrPath)!=0) RegisterPlugins(CurrPath);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBPluginMgr::RegisterPlugins(char* Path)
{
	char Mask[MAX_PATH];
	strcpy(Mask, Path);
	strcat(Mask, PLUGIN_MASK);

	struct _finddata_t find_file;
	long hFile;

	// scan files
	if((hFile = _findfirst(Mask, &find_file )) != -1L)
	{
		if(!(find_file.attrib & _A_SUBDIR))
		{
			RegisterPlugin(Path, find_file.name);
		}
		while(_findnext(hFile, &find_file) == 0)
		{
			if(!(find_file.attrib & _A_SUBDIR)){
				RegisterPlugin(Path, find_file.name);
			}
		}
		_findclose( hFile );
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBPluginMgr::RegisterPlugin(char* Path, char* Filename)
{
	if(stricmp(Filename, "wme_report.dll")==0) return S_OK;
	if(stricmp(Filename, "wme_console.dll")==0) return S_OK;

	char FullPath[MAX_PATH];
	sprintf(FullPath, "%s%s", Path, Filename);

	HMODULE DllHandle = ::LoadLibrary(FullPath);
	if(DllHandle)
	{
		PLUGIN_GETPLUGININFO GetPluginInfo = (PLUGIN_GETPLUGININFO)::GetProcAddress(DllHandle, "GetPluginInfo");
		PLUGIN_GETSUPPORTEDCLASSES GetSupportedClasses = (PLUGIN_GETSUPPORTEDCLASSES)::GetProcAddress(DllHandle, "GetSupportedClasses");
		PLUGIN_GETCLASSDEFINITION GetClassDefiniton = (PLUGIN_GETCLASSDEFINITION)::GetProcAddress(DllHandle, "GetClassDefinition");

		if(GetPluginInfo && GetSupportedClasses && GetClassDefiniton)
		{
			CBPlugin* Plugin = new CBPlugin(Game);
			if(Plugin && SUCCEEDED(Plugin->Create(FullPath, DllHandle)))
			{
				Game->LOG(0, "  %s", Filename);
				m_Plugins.Add(Plugin);
			}
			else SAFE_DELETE(Plugin);
		}
		else ::FreeLibrary(DllHandle);
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBPluginMgr::RegisterClasses()
{
	if(!Game->m_ScEngine || !Game->m_ScEngine->m_CompilerAvailable) return E_FAIL;

	for(int i=0; i<m_Plugins.GetSize(); i++)
	{
		CBPlugin* Plugin = m_Plugins[i];
		for(int j=0; j<Plugin->m_Classes.GetSize(); j++)
		{
			Game->m_ScEngine->ExtDefineFunction(Plugin->m_Classes[j]);
		}
	}

	return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBPluginMgr::HandleExternalCall(CScScript* Script, CScStack* Stack, CScStack* ThisStack, char* Name)
{
	for(int i=0; i<m_Plugins.GetSize(); i++)
	{
		if(SUCCEEDED(m_Plugins[i]->HandleExternalCall(Script, Stack, ThisStack, Name))) return S_OK;
	}
	return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
HMODULE CBPluginMgr::GetDllHandle(const char* ClassName)
{
	for(int i=0; i<m_Plugins.GetSize(); i++)
	{
		CBPlugin* Plugin = m_Plugins[i];
		for(int j=0; j<Plugin->m_Classes.GetSize(); j++)
		{
			if(strcmp(ClassName, Plugin->m_Classes[j])==0)
			{
				return Plugin->m_DllHandle;
			}
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBPluginMgr::AddObject(CSXPluginObject* Obj)
{
	for(int i=0; i<m_Objects.GetSize(); i++)
	{
		if(m_Objects[i]==Obj) return S_OK;
	}
	m_Objects.Add(Obj);
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBPluginMgr::RemoveObject(CSXPluginObject* Obj)
{
	if(m_IsDisposing) return S_OK;

	for(int i=0; i<m_Objects.GetSize(); i++)
	{
		if(m_Objects[i]==Obj)
		{
			m_Objects.RemoveAt(i);
			return S_OK;
		}
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
bool CBPluginMgr::SubscribeEvent(IWmeObject* Object, EWmeEvent Event)
{
	if(Event<0 || Event>=WME_EVENT_MAX) return false;

	for(int i=0; i<m_Objects.GetSize(); i++)
	{
		if(m_Objects[i]==Object)
		{
			m_Objects[i]->m_SubscribedEvents[Event] = true;
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
bool CBPluginMgr::UnsubscribeEvent(IWmeObject* Object, EWmeEvent Event)
{
	if(Event<0 || Event>=WME_EVENT_MAX) return false;

	for(int i=0; i<m_Objects.GetSize(); i++)
	{
		if(m_Objects[i]==Object)
		{
			m_Objects[i]->m_SubscribedEvents[Event] = false;
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
bool CBPluginMgr::ApplyEvent(EWmeEvent Event, void* EventData1, void* EventData2)
{
	bool Ret = false;
	for(int i=0; i<m_Objects.GetSize(); i++)
	{
		if(m_Objects[i]->ApplyEvent(Event, EventData1, EventData2)) Ret = true;
	}
	return Ret;
}

//////////////////////////////////////////////////////////////////////////
bool CBPluginMgr::ValidObject(CBObject* Object)
{
	for(int i=0; i<m_Objects.GetSize(); i++)
	{
		if(m_Objects[i]==Object) return true;
	}
	return false;
}
