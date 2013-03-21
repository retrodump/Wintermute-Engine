// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BSCRIPTABLE_H__A338DE51_E8EA_4602_9F7C_D8B30DAD25B1__INCLUDED_)
#define AFX_BSCRIPTABLE_H__A338DE51_E8EA_4602_9F7C_D8B30DAD25B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BNamedObject.h"
#include "wme_debugger.h"

class CScValue;
class CScStack;
class CScScript;

class CBScriptable : public CBNamedObject, public IWmeObject, public IWmeDebugObject
{
public:
	virtual CScScript* InvokeMethodThread(char* MethodName);
	DECLARE_PERSISTENT(CBScriptable, CBNamedObject);

	CBScriptable(CBGame* inGame, bool NoValue = false, bool Persistable=true);
	virtual ~CBScriptable();

	// high level scripting interface
	virtual bool CanHandleMethod(char* EventMethod);
	virtual HRESULT ScSetProperty(char* Name, CScValue* Value);
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack* Stack, CScStack* ThisStack, char* Name);
	virtual char* ScToString();
	virtual void* ScToMemBuffer();
	virtual int ScToInt();
	virtual double ScToFloat();
	virtual bool ScToBool();
	virtual void ScSetString(char* Val);
	virtual void ScSetInt(int Val);
	virtual void ScSetFloat(double Val);
	virtual void ScSetBool(bool Val);
	virtual int ScCompare(CBScriptable* Val);
	virtual void ScDebuggerDesc(char* Buf, int BufSize);
	int m_RefCount;
	CScValue* m_ScValue;
	CScValue* m_ScProp;


	// IWmeObject
public:
	virtual bool SendEvent(const char* EventName);
	IWmeValue* CallMethod(const char* MethodName, IWmeParamSet* Params=0);
	bool SetProperty(const char* PropName, IWmeValue* Value);
	bool SetProperty(const char* PropName, int Value);
	bool SetProperty(const char* PropName, const char* Value);
	bool SetProperty(const char* PropName, double Value);
	bool SetProperty(const char* PropName, bool Value);
	bool SetProperty(const char* PropName, IWmeObject* Value);
	bool SetProperty(const char* PropName);
	IWmeValue* GetProperty(const char* PropName);
	void* GetInterface(const char* ClassName);

	// IWmeDebugObject
	const char* DbgGetNativeClass();
	IWmeDebugProp* DbgGetProperty(const char* Name);

};

#endif // !defined(AFX_BSCRIPTABLE_H__A338DE51_E8EA_4602_9F7C_D8B30DAD25B1__INCLUDED_)
