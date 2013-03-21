// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BSCRIPTHOLDER_H__2B6A5040_3ED9_11D4_B319_BCE8B5119713__INCLUDED_)
#define AFX_BSCRIPTHOLDER_H__2B6A5040_3ED9_11D4_B319_BCE8B5119713__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CBScriptHolder : public CBScriptable /*, public IWmeObject*/
{
public:
	DECLARE_PERSISTENT(CBScriptHolder, CBScriptable);

	CBScriptHolder(CBGame* inGame);
	virtual ~CBScriptHolder();

	virtual CScScript* InvokeMethodThread(char* MethodName);
	virtual void MakeFreezable(bool Freezable);
	bool CanHandleEvent(char* EventName);
	virtual bool CanHandleMethod(char* EventMethod);
	HRESULT Cleanup();
	HRESULT RemoveScript(CScScript* Script);
	HRESULT AddScript(char* Filename);
	virtual HRESULT SaveAsText(CBDynBuffer* Buffer, int Indent);	
	virtual HRESULT Listen(CBScriptHolder* param1, DWORD param2);	
	HRESULT ApplyEvent(char* EventName, bool Unbreakable=false);
	void SetFilename(char* Filename);
	HRESULT ParseProperty(BYTE* Buffer, bool Complete=true);

	char* m_Filename;
	bool m_Freezable;
	bool m_Ready;
	CBArray<CScScript*, CScScript*> m_Scripts;

	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char *Name, CScValue *Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	virtual char* ScToString();
	virtual void ScDebuggerDesc(char* Buf, int BufSize);

	// IWmeObject
public:
	virtual bool SendEvent(const char* EventName);
};

#endif // !defined(AFX_BSCRIPTHOLDER_H__2B6A5040_3ED9_11D4_B319_BCE8B5119713__INCLUDED_)
