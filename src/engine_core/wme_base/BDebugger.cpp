// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\BDebugger.h"

//////////////////////////////////////////////////////////////////////////
CBDebugger::CBDebugger(CBGame* inGame) : CBBase(inGame)
{
	m_Enabled = false;
	
	m_Dll = NULL;
	m_InitializeProc = NULL;
	m_ShutdownProc = NULL;
}

//////////////////////////////////////////////////////////////////////////
CBDebugger::~CBDebugger(void)
{
	Shutdown();
	m_Clients.RemoveAll();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBDebugger::Initialize()
{
	m_Dll = CBPlatform::LoadLibrary("wme_console.dll");
	if(m_Dll)
	{
		m_InitializeProc = (WMEDBG_INITIALIZE)CBPlatform::GetProcAddress(m_Dll, "Initialize");
		m_ShutdownProc = (WMEDBG_SHUTDOWN)CBPlatform::GetProcAddress(m_Dll, "Shutdown");

		if(!m_InitializeProc || ! m_ShutdownProc)
		{
			CBPlatform::FreeLibrary(m_Dll);
			m_Dll = NULL;
		}
	}
	if(!m_Dll)
	{
		Game->LOG(0, "Error loading 'wme_console.dll'. Make sure the file is present and .NET framework 2.0 is installed.");
		m_Enabled = false;
	}
	else
	{
		m_Enabled = m_InitializeProc(this);
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBDebugger::Shutdown()
{
	if(!m_Enabled) return S_OK;

	if(m_Dll)
	{
		m_ShutdownProc(this);
		CBPlatform::FreeLibrary(m_Dll);
		m_Dll = NULL;
	}
	m_Enabled = false;
	m_InitializeProc = NULL;
	m_ShutdownProc = NULL;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
bool CBDebugger::IsClientValid(IWmeDebugClient* Client)
{
	for(int i=0; i<m_Clients.GetSize(); i++)
	{
		if(m_Clients[i]==Client) return true;
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBDebugger::OnGameInit()
{
	if(!m_Enabled) return S_OK;
	for(int i=0; i<m_Clients.GetSize(); i++)
	{
		m_Clients[i]->OnGameInit();
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBDebugger::OnGameShutdown()
{
	if(!m_Enabled) return S_OK;
	for(int i=0; i<m_Clients.GetSize(); i++)
	{
		m_Clients[i]->OnGameShutdown();
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBDebugger::OnGameTick()
{
	if(!m_Enabled) return S_OK;
	for(int i=0; i<m_Clients.GetSize(); i++)
	{
		m_Clients[i]->OnGameTick();
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBDebugger::OnLog(unsigned int ErrorCode, const char* Text)
{
	if(!m_Enabled) return S_OK;
	for(int i=0; i<m_Clients.GetSize(); i++)
	{
		m_Clients[i]->OnLog(ErrorCode, Text);
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBDebugger::OnScriptInit(CScScript* Script)
{
	if(!m_Enabled) return S_OK;
	for(int i=0; i<m_Clients.GetSize(); i++)
	{
		m_Clients[i]->OnScriptInit(Script);
		Script->DbgSendVariables(m_Clients[i]);
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBDebugger::OnScriptEventThreadInit(CScScript* Script, CScScript* ParentScript, const char* Name)
{
	if(!m_Enabled) return S_OK;
	for(int i=0; i<m_Clients.GetSize(); i++)
	{
		m_Clients[i]->OnScriptEventThreadInit(Script, ParentScript, Name);
		Script->DbgSendVariables(m_Clients[i]);
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBDebugger::OnScriptMethodThreadInit(CScScript* Script, CScScript* ParentScript, const char* Name)
{
	if(!m_Enabled) return S_OK;
	for(int i=0; i<m_Clients.GetSize(); i++)
	{
		m_Clients[i]->OnScriptMethodThreadInit(Script, ParentScript, Name);
		Script->DbgSendVariables(m_Clients[i]);
	}
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBDebugger::OnScriptShutdown(CScScript* Script)
{
	if(!m_Enabled) return S_OK;
	for(int i=0; i<m_Clients.GetSize(); i++)
	{
		m_Clients[i]->OnScriptShutdown(Script);
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBDebugger::OnScriptChangeLine(CScScript* Script, int Line)
{
	if(!m_Enabled) return S_OK;
	for(int i=0; i<m_Clients.GetSize(); i++)
	{
		m_Clients[i]->OnScriptChangeLine(Script, Line);
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBDebugger::OnScriptChangeScope(CScScript* Script, CScValue* Scope)
{
	if(!m_Enabled) return S_OK;
	for(int i=0; i<m_Clients.GetSize(); i++)
	{
		m_Clients[i]->OnScriptChangeScope(Script, (unsigned int)Scope);
	}
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBDebugger::OnScriptShutdownScope(CScScript* Script, CScValue* Scope)
{
	if(!m_Enabled) return S_OK;
	for(int i=0; i<m_Clients.GetSize(); i++)
	{
		m_Clients[i]->OnScriptShutdownScope(Script, (unsigned int)Scope);
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBDebugger::OnVariableInit(EWmeDebuggerVariableType Type, CScScript* Script, CScValue* Scope, CScValue* Var, const char* VariableName)
{
	if(!m_Enabled) return S_OK;
	for(int i=0; i<m_Clients.GetSize(); i++)
	{
		m_Clients[i]->OnVariableInit(Type, Script, (unsigned int)Scope, Var, VariableName);
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBDebugger::OnVariableChangeValue(CScValue* Var, CScValue* Value)
{
	if(!m_Enabled) return S_OK;
	for(int i=0; i<m_Clients.GetSize(); i++)
	{
		m_Clients[i]->OnVariableChangeValue(Var, Value);
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBDebugger::OnScriptHitBreakpoint(CScScript* Script)
{
	if(!m_Enabled) return S_OK;
	for(int i=0; i<m_Clients.GetSize(); i++)
	{
		m_Clients[i]->OnScriptHitBreakpoint(Script, Script->m_CurrentLine);
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
// IWmeDebugServer interface implementation
bool CBDebugger::AttachClient(IWmeDebugClient* Client)
{
	for(int i=0; i<m_Clients.GetSize(); i++)
	{
		if(m_Clients[i]==Client) return true;
	}
	m_Clients.Add(Client);
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CBDebugger::DetachClient(IWmeDebugClient* Client)
{
	for(int i=0; i<m_Clients.GetSize(); i++)
	{
		if(m_Clients[i]==Client)
		{
			m_Clients.RemoveAt(i);
			return true;
		}
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////
bool CBDebugger::QueryData(IWmeDebugClient* Client)
{
	if(!IsClientValid(Client)) return false;
	if(!Game->m_ScEngine) return false;

	return SUCCEEDED(Game->m_ScEngine->DbgSendScripts(Client));
}


//////////////////////////////////////////////////////////////////////////
int CBDebugger::GetPropInt(const char* PropName)
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////
double CBDebugger::GetPropFloat(const char* PropName)
{
	return 0.0;
}

//////////////////////////////////////////////////////////////////////////
const char* CBDebugger::GetPropString(const char* PropName)
{
	if(strcmp(PropName, "RegistryPath")==0)
	{
		return Game->m_Registry->GetBasePath();
	}
	return "";
}

//////////////////////////////////////////////////////////////////////////
bool CBDebugger::GetPropBool(const char* PropName)
{
	return false;
}

//////////////////////////////////////////////////////////////////////////
bool CBDebugger::SetProp(const char* PropName, int PropValue)
{
	return false;
}

//////////////////////////////////////////////////////////////////////////
bool CBDebugger::SetProp(const char* PropName, double PropValue)
{
	return false;
}

//////////////////////////////////////////////////////////////////////////
bool CBDebugger::SetProp(const char* PropName, const char* PropValue)
{
	return false;
}

//////////////////////////////////////////////////////////////////////////
bool CBDebugger::SetProp(const char* PropName, bool PropValue)
{
	return false;
}

//////////////////////////////////////////////////////////////////////////
bool CBDebugger::ResolveFilename(const char* RelativeFilename, char* AbsFilenameBuf, int AbsBufSize)
{
	return Game->m_FileManager->MakeAbsolutePath((char*)RelativeFilename, AbsFilenameBuf, AbsBufSize);
}

//////////////////////////////////////////////////////////////////////////
bool CBDebugger::AddBreakpoint(const char* ScriptFilename, int Line)
{
	return SUCCEEDED(Game->m_ScEngine->AddBreakpoint((char*)ScriptFilename, Line));
}

//////////////////////////////////////////////////////////////////////////
bool CBDebugger::RemoveBreakpoint(const char* ScriptFilename, int Line)
{
	return SUCCEEDED(Game->m_ScEngine->RemoveBreakpoint((char*)ScriptFilename, Line));
}

//////////////////////////////////////////////////////////////////////////
bool CBDebugger::ContinueExecution()
{
	return CBPlatform::SetForegroundWindow(Game->m_Renderer->m_Window) != FALSE;	
}
