// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include <vcclr.h>
#include "WmeScript.h"
#include "WmeValue.h"
#include "DebugServer.h"

using namespace wme_console;
using namespace System;
using namespace System::Collections::Generic;
using namespace DeadCode::WME::Debugger;

class CDebugClient : public IWmeDebugClient
{
public:
	//////////////////////////////////////////////////////////////////////////
	CDebugClient(IWmeDebugServer* Server)
	{
		m_Server = Server;
		m_Client = gcnew DebugClient(gcnew DebugServer(Server, this));
	}

	//////////////////////////////////////////////////////////////////////////
	virtual ~CDebugClient(void)
	{
		m_Client->~DebugClient();
	}

// IWmeDebugClient interface implementation
public:
	//////////////////////////////////////////////////////////////////////////
	virtual bool OnGameInit()
	{
		return m_Client->GameInit();
	}

	//////////////////////////////////////////////////////////////////////////
	virtual bool OnGameShutdown()
	{
		return m_Client->GameShutdown();
	}

	//////////////////////////////////////////////////////////////////////////
	virtual bool OnGameTick()
	{
		return m_Client->GameTick();
	}

	//////////////////////////////////////////////////////////////////////////
	virtual bool OnLog(unsigned int ErrorCode, const char* Text)
	{
		return m_Client->Log(ErrorCode, gcnew String(Text));
	}

	//////////////////////////////////////////////////////////////////////////
	virtual bool OnScriptInit(IWmeDebugScript* Script)
	{
		return m_Client->ScriptInit(gcnew WmeScript(Script));
	}

	//////////////////////////////////////////////////////////////////////////
	virtual bool OnScriptEventThreadInit(IWmeDebugScript* Script, IWmeDebugScript* ParentScript, const char* EventName)
	{
		return m_Client->ScriptEventThreadInit(gcnew WmeScript(Script), IntPtr(ParentScript), gcnew String(EventName));
	}

	//////////////////////////////////////////////////////////////////////////
	virtual bool OnScriptMethodThreadInit(IWmeDebugScript* Script, IWmeDebugScript* ParentScript, const char* MethodName)
	{
		return m_Client->ScriptEventThreadInit(gcnew WmeScript(Script), IntPtr(ParentScript), gcnew String(MethodName));
	}

	//////////////////////////////////////////////////////////////////////////
	virtual bool OnScriptShutdown(IWmeDebugScript* Script)
	{
		return m_Client->ScriptShutdown(IntPtr(Script));
	}

	//////////////////////////////////////////////////////////////////////////
	virtual bool OnScriptChangeLine(IWmeDebugScript* Script, int Line)
	{
		return m_Client->ScriptChangeLine(IntPtr(Script), Line);
	}

	//////////////////////////////////////////////////////////////////////////
	virtual bool OnScriptChangeScope(IWmeDebugScript* Script, unsigned int ScopeID)
	{
		return m_Client->ScriptChangeScope(IntPtr(Script), ScopeID);
	}

	//////////////////////////////////////////////////////////////////////////
	virtual bool OnScriptShutdownScope(IWmeDebugScript* Script, unsigned int ScopeID)
	{
		return m_Client->ScriptShutdownScope(IntPtr(Script), ScopeID);
	}

	//////////////////////////////////////////////////////////////////////////
	virtual bool OnVariableInit(EWmeDebuggerVariableType Type, IWmeDebugScript* Script, unsigned int ScopeID, IWmeDebugProp* Variable, const char* VariableName)
	{
		switch(Type)
		{
		case WME_DBGVAR_GLOBAL:
			return m_Client->GlobalVariableInit(gcnew WmeValue(Variable, VariableName));
		case WME_DBGVAR_SCRIPT:
			return m_Client->ScriptVariableInit(gcnew WmeValue(Variable, VariableName), IntPtr(Script));
		case WME_DBGVAR_SCOPE:
			return m_Client->ScopeVariableInit(gcnew WmeValue(Variable, VariableName), IntPtr(Script), ScopeID);
		}
		return false;	
	}

	//////////////////////////////////////////////////////////////////////////
	virtual bool OnVariableChangeValue(IWmeDebugProp* Variable, IWmeDebugProp* Value)
	{
		return m_Client->VariableChangeValue(gcnew WmeValue(Variable), gcnew WmeValue(Value));
	}

	//////////////////////////////////////////////////////////////////////////
	virtual bool OnScriptHitBreakpoint(IWmeDebugScript* Script, int Line)
	{
		return m_Client->ScriptHitBreakpoint(IntPtr(Script), Line);
	}

private:
	// native server
	IWmeDebugServer* m_Server;
	// managed client
	gcroot<DebugClient^> m_Client;
};
