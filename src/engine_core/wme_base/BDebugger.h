// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BBase.h"
#include "wme_debugger.h"

class CBDebugger : public CBBase, public IWmeDebugServer
{
public:
	CBDebugger(CBGame* inGame);
	virtual ~CBDebugger(void);

	// initialization
	bool m_Enabled;
	HRESULT Initialize();
	HRESULT Shutdown();

	// internal interface
	HRESULT OnGameInit();
	HRESULT OnGameShutdown();
	HRESULT OnGameTick();
	HRESULT OnLog(unsigned int ErrorCode, const char* Text);
	HRESULT OnScriptInit(CScScript* Script);
	HRESULT OnScriptEventThreadInit(CScScript* Script, CScScript* ParentScript, const char* Name);
	HRESULT OnScriptMethodThreadInit(CScScript* Script, CScScript* ParentScript, const char* Name);

	HRESULT OnScriptShutdown(CScScript* Script);
	HRESULT OnScriptChangeLine(CScScript* Script, int Line);
	HRESULT OnScriptChangeScope(CScScript* Script, CScValue* Scope);
	HRESULT OnScriptShutdownScope(CScScript* Script, CScValue* Scope);
	HRESULT OnVariableInit(EWmeDebuggerVariableType Type, CScScript* Script, CScValue* Scope, CScValue* Var, const char* VariableName);
	HRESULT OnVariableChangeValue(CScValue* Var, CScValue* Value);

	HRESULT OnScriptHitBreakpoint(CScScript* Script);

	// IWmeDebugServer interface
	virtual bool AttachClient(IWmeDebugClient* Client);
	virtual bool DetachClient(IWmeDebugClient* Client);
	virtual bool QueryData(IWmeDebugClient* Client);

	virtual int GetPropInt(const char* PropName);
	virtual double GetPropFloat(const char* PropName);
	virtual const char* GetPropString(const char* PropName);
	virtual bool GetPropBool(const char* PropName);

	virtual bool SetProp(const char* PropName, int PropValue);
	virtual bool SetProp(const char* PropName, double PropValue);
	virtual bool SetProp(const char* PropName, const char* PropValue);
	virtual bool SetProp(const char* PropName, bool PropValue);

	virtual bool ResolveFilename(const char* RelativeFilename, char* AbsFilenameBuf, int AbsBufSize);

	virtual bool AddBreakpoint(const char* ScriptFilename, int Line);
	virtual bool RemoveBreakpoint(const char* ScriptFilename, int Line);

	virtual bool ContinueExecution();
private:
	CBArray<IWmeDebugClient*, IWmeDebugClient*> m_Clients;
	bool IsClientValid(IWmeDebugClient* Client);
	HMODULE m_Dll;

	WMEDBG_INITIALIZE m_InitializeProc;
	WMEDBG_SHUTDOWN m_ShutdownProc;
};
