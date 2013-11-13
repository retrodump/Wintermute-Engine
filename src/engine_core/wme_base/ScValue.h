// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_SCVALUE_H__B4CF28AB_AF07_4708_8CAD_43DB73085871__INCLUDED_)
#define AFX_SCVALUE_H__B4CF28AB_AF07_4708_8CAD_43DB73085871__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "BBase.h"
#include "dcscript.h"	// Added by ClassView
#include "wme_plugin.h"
#include <map>
#include <string>
#include "wme_debugger.h"

using namespace std;

class CScValue : public CBBase, public IWmeValue, public IWmeDebugProp
{
public:
	HRESULT DbgSendVariables(IWmeDebugClient* Client, EWmeDebuggerVariableType Type, CScScript* Script, unsigned int ScopeID);

	static int Compare(CScValue* Val1, CScValue* Val2);
	static int CompareStrict(CScValue* Val1, CScValue* Val2);
	TValType GetTypeTolerant();
	void Cleanup(bool IgnoreNatives = false);
	DECLARE_PERSISTENT(CScValue, CBBase);

	bool m_IsConstVar;
	HRESULT SaveAsText(CBDynBuffer* Buffer, int Indent);
	void SetValue(CScValue* Val);
	bool m_Persistent;
	bool PropExists(char* Name);
	void Copy(CScValue* orig, bool CopyWhole = false);
	void SetStringVal(char* Val);
	TValType GetType();
	bool GetBool(bool Default=false);
	int GetInt(int Default=0);
	double GetFloat(double Default=0.0f);
	char* GetString();
	void* GetMemBuffer();
	CBScriptable* GetNative();
	HRESULT DeleteProp(char *Name);
	void DeleteProps();
	void CleanProps(bool IncludingNatives);
	void SetBool(bool Val);
	void SetInt(int Val);
	void SetFloat(double Val);
	void SetString(char* Val);
	void SetNULL();
	void SetNative(CBScriptable* Val, bool Persistent=false);
	void SetObject();
	void SetReference(CScValue* Val);
	bool IsNULL();
	bool IsNative();
	bool IsString();
	bool IsBool();
	bool IsFloat();
	bool IsInt();
	bool IsObject();
	HRESULT SetProp(char* Name, CScValue* Val, bool CopyWhole = false, bool SetAsConst=false);
	CScValue* GetProp(char* Name);
	CBScriptable* m_ValNative;
	CScValue* m_ValRef;
	bool m_ValBool;
	int m_ValInt;
	double m_ValFloat;
	char* m_ValString;
	TValType m_Type;
	CScValue(CBGame* inGame);
	CScValue(CBGame* inGame, bool Val);
	CScValue(CBGame* inGame, int Val);
	CScValue(CBGame* inGame, double Val);
	CScValue(CBGame* inGame, char* Val);
	virtual ~CScValue();
	map<string, CScValue*> m_ValObject;
	map<string, CScValue*>::iterator m_ValIter;

// IWmeValue interface implementation
public:
	// getters
	int GetValInt();
	const char* GetValString();
	double GetValFloat();
	bool GetValBool();
	IWmeObject* GetValNative();

	// setters
	bool SetVal(int Value);
	bool SetVal(const char* Value);
	bool SetVal(double Value);
	bool SetVal(bool Value);
	bool SetVal(IWmeObject* Value);
	bool SetVal(IWmeValue* Value);
	bool SetVal();

	// type
	bool IsValInt();
	bool IsValString();
	bool IsValFloat();
	bool IsValBool();
	bool IsValNative();
	bool IsValObject();
	bool IsValNull();

	// properties
	IWmeValue* GetProperty(const char* PropName);
	bool SetProperty(const char* PropName, int Value);
	bool SetProperty(const char* PropName, const char* Value);
	bool SetProperty(const char* PropName, double Value);
	bool SetProperty(const char* PropName, bool Value);
	bool SetProperty(const char* PropName, IWmeObject* Value);
	bool SetProperty(const char* PropName, IWmeValue* Value);
	bool SetProperty(const char* PropName);

// IWmeDebugProp interface implementation
public:
	virtual EWmeDebuggerPropType DbgGetType();

	// getters	
	virtual int DbgGetValInt();
	virtual double DbgGetValFloat();
	virtual bool DbgGetValBool();
	virtual const char* DbgGetValString();	
	virtual IWmeDebugObject* DbgGetValNative();	

	// setters
	virtual bool DbgSetVal(int Value);
	virtual bool DbgSetVal(double Value);
	virtual bool DbgSetVal(bool Value);
	virtual bool DbgSetVal(const char* Value);	
	virtual bool DbgSetVal();

	// properties
	virtual int DbgGetNumProperties();
	virtual bool DbgGetProperty(int Index, const char** Name, IWmeDebugProp** Value);

	virtual bool DbgGetDescription(char* Buf, int BufSize);
};


#endif // !defined(AFX_SCVALUE_H__B4CF28AB_AF07_4708_8CAD_43DB73085871__INCLUDED_)
