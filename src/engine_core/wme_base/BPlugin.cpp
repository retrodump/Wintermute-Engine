// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\BPlugin.h"
#include "SXPluginObject.h"

//////////////////////////////////////////////////////////////////////////
CBPlugin::CBPlugin(CBGame* inGame):CBBase(inGame)
{
	m_DllPath = NULL;
	m_DllHandle = NULL;

	memset(&m_PluginInfo, 0, sizeof(SWmePluginInfo));
}

//////////////////////////////////////////////////////////////////////////
CBPlugin::~CBPlugin(void)
{
	for(int i=0; i<m_Classes.GetSize(); i++)
	{
		delete [] m_Classes[i];
	}
	m_Classes.RemoveAll();

	SAFE_DELETE_ARRAY(m_DllPath);

	if(m_DllHandle) ::FreeLibrary(m_DllHandle);
	m_DllHandle = NULL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBPlugin::Create(char* Path, HMODULE Handle)
{
	CBUtils::SetString(&m_DllPath, Path);
	m_DllHandle = Handle;

	// get plugin info
	PLUGIN_GETPLUGININFO GetPluginInfo = (PLUGIN_GETPLUGININFO)::GetProcAddress(m_DllHandle, "GetPluginInfo");
	if(GetPluginInfo) GetPluginInfo(&m_PluginInfo);

	// check if version is > 1.5.1
	int VerMajor = 0;
	int VerMinor = 0;
	int VerBuild = 0;
	char* Temp = NULL;
	Temp = CBUtils::StrEntry(0, m_PluginInfo.WmeVersion, '.');
	if(Temp)
	{
		VerMajor = atoi(Temp);
		delete [] Temp;
	}
	Temp = CBUtils::StrEntry(1, m_PluginInfo.WmeVersion, '.');
	if(Temp)
	{
		VerMinor = atoi(Temp);
		delete [] Temp;
	}
	Temp = CBUtils::StrEntry(2, m_PluginInfo.WmeVersion, '.');
	if(Temp)
	{
		VerBuild = atoi(Temp);
		delete [] Temp;
	}
	bool VerOk = true;
	if(VerMajor<1) VerOk = false;
	else if(VerMajor==1 && VerMinor<5) VerOk = false;
	else if(VerMajor==1 && VerMinor==5 && VerBuild<2) VerOk = false;

	if(!VerOk)
	{
		Game->LOG(0, "  Error: plugin '%s' is now obsolete, please recompile.", Path);
		return E_FAIL;
	}


	// get supported classes
	PLUGIN_GETSUPPORTEDCLASSES GetSupportedClasses = (PLUGIN_GETSUPPORTEDCLASSES)::GetProcAddress(m_DllHandle, "GetSupportedClasses");
	if(GetSupportedClasses)
	{
		const char* ClassList = GetSupportedClasses();
		if(ClassList)
		{
			const char* Start = ClassList;
			for(int i=0; i<=strlen(ClassList); i++)
			{
				if(ClassList[i]==',' || ClassList[i]=='\0')
				{
					int Len = ClassList + i - Start;
					if(Len>0)
					{
						char* Name = new char[Len+1];
						memset(Name, 0, Len+1);
						strncpy(Name, Start, Len);
						m_Classes.Add(Name);
					}
					Start = ClassList + i + 1;
				}
			}
		}

		return S_OK;
	}
	else return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBPlugin::HandleExternalCall(CScScript* Script, CScStack* Stack, CScStack* ThisStack, char* Name)
{
	for(int i=0; i<m_Classes.GetSize(); i++)
	{
		if(strcmp(Name, m_Classes[i])==0)
		{
			CScValue* this_obj = ThisStack->GetTop();
			CSXPluginObject* Obj = new CSXPluginObject(Game);
			if(!Obj || FAILED(Obj->Initialize(Name, m_DllHandle)))
			{
				SAFE_DELETE(Obj);
				return E_FAIL;
			}
			Obj->Construct(Script, Stack);

			this_obj->SetNative(Obj);
			Stack->PushNULL();

			return S_OK;
		}
	}
	return E_FAIL;
}
