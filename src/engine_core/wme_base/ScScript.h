// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_SCSCRIPT_H__B86C03A1_2450_49CB_8001_FE162E070EA4__INCLUDED_)
#define AFX_SCSCRIPT_H__B86C03A1_2450_49CB_8001_FE162E070EA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"
#include "dcscript.h"	// Added by ClassView
#include "wme_plugin.h"
#include "wme_debugger.h"

class CScEngine;
class CScScript : public CBBase, public IWmeScript, public IWmeDebugScript
{
public:
	HRESULT DbgSendScript(IWmeDebugClient* Client);
	HRESULT DbgSendVariables(IWmeDebugClient* Client);

	CBArray<int, int> m_Breakpoints;
	bool m_TracingMode;

	CScScript* m_ParentScript;
	bool m_Unbreakable;
	HRESULT FinishThreads();
	HRESULT CopyParameters(CScStack* Stack);
	static DWORD Call_cdecl(const void* args, size_t sz, DWORD func, bool* StackCorrupt);
	static DWORD Call_stdcall(const void* args, size_t sz, DWORD func, bool* StackCorrupt);
	static DWORD GetST0(void);
	static double GetST0Double(void);

	CScValue* m_Operand;
	CScValue* m_Reg1;
	bool m_Freezable;
	HRESULT Resume();
	HRESULT Pause();
	bool CanHandleEvent(char* EventName);
	bool CanHandleMethod(char* MethodName);
	HRESULT CreateThread(CScScript* Original, DWORD InitIP, char* EventName);
	HRESULT CreateMethodThread(CScScript *Original, char* MethodName);
	CScScript* InvokeEventHandler(char* EventName, bool Unbreakable=false);
	DWORD m_TimeSlice;
	DECLARE_PERSISTENT(CScScript, CBBase);
	void AfterLoad();
	void __cdecl CScScript::RuntimeError(LPSTR fmt, ...);
	HRESULT Run();
	HRESULT Finish(bool IncludingThreads = false);
	HRESULT Sleep(DWORD Duration);
	HRESULT WaitForExclusive(CBObject* Object);
	HRESULT WaitFor(CBObject* Object);
	DWORD m_WaitTime;
	bool m_WaitFrozen;
	CBObject* m_WaitObject;
	CScScript* m_WaitScript;
	TScriptState m_State;
	TScriptState m_OrigState;
	CScValue* GetVar(char* Name);
	DWORD GetFuncPos(char* Name);
	DWORD GetEventPos(char* Name);
	DWORD GetMethodPos(char* Name);
	typedef struct{
		DWORD magic;
		DWORD version;
		DWORD code_start;
		DWORD func_table;
		DWORD symbol_table;
		DWORD event_table;
		DWORD externals_table;
		DWORD method_table;
	}TScriptHeader;


	typedef struct{
		char* name;
		DWORD pos;
	}TFunctionPos;

	typedef struct{
		char* name;
		DWORD pos;
	}TMethodPos;

	typedef struct{
		char* name;
		DWORD pos;
	}TEventPos;

	typedef struct{
		char* name;
		char* dll_name;
		TCallType call_type;
		TExternalType returns;
		int num_params;
		TExternalType* params;
	}TExternalFunction;


	CScStack* m_CallStack;
	CScStack* m_ThisStack;
	CScStack* m_ScopeStack;
	CScStack* m_Stack;
	CScValue* m_Globals;
	CScEngine* m_Engine;
	int m_CurrentLine;
	HRESULT ExecuteInstruction();
	char* GetString();
	DWORD GetDWORD();
	double GetFloat();
	void Cleanup();
	HRESULT Create(char* Filename, BYTE* Buffer, DWORD Size, CBScriptHolder* Owner);
	DWORD m_IP;
	DWORD m_BufferSize;
	BYTE* m_Buffer;
	CScScript(CBGame* inGame, CScEngine* Engine);
	virtual ~CScScript();
	char* m_Filename;
	char** m_Symbols;
	int m_NumSymbols;
	TFunctionPos* m_Functions;
	TMethodPos* m_Methods;
	TEventPos* m_Events;
	int m_NumExternals;
	TExternalFunction* m_Externals;
	int m_NumFunctions;
	int m_NumMethods;
	int m_NumEvents;
	bool m_Thread;
	bool m_MethodThread;
	char* m_ThreadEvent;
	CBScriptHolder* m_Owner;
	CScScript::TExternalFunction* GetExternal(char* Name);
	HRESULT ExternalCall(CScStack* Stack, CScStack* ThisStack, CScScript::TExternalFunction* Function);
private:
	HRESULT InitScript();
	HRESULT InitTables();

// IWmeScript interface implementation
public:
	const char* GetFilename();
	int GetCurrentLine();
	void ReportRuntimeError(const char* Description);

	IWmeValue* GetVariable(const char* VarName);
	bool SetVariable(const char* VarName, int Value);
	bool SetVariable(const char* VarName, const char* Value);
	bool SetVariable(const char* VarName, double Value);
	bool SetVariable(const char* VarName, bool Value);
	bool SetVariable(const char* VarName, IWmeObject* Value);
	bool SetVariable(const char* VarName, IWmeValue* Value);
	bool SetVariable(const char* VarName);

// IWmeDebugScript interface implementation
public:
	virtual int DbgGetLine();
	virtual const char* DbgGetFilename();
	virtual TScriptState DbgGetState();
	virtual int DbgGetNumBreakpoints();
	virtual int DbgGetBreakpoint(int Index);

	virtual bool DbgSetTracingMode(bool IsTracing);
	virtual bool DbgGetTracingMode();
};

#endif // !defined(AFX_SCSCRIPT_H__B86C03A1_2450_49CB_8001_FE162E070EA4__INCLUDED_)
