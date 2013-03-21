// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BScriptable.h"


IMPLEMENT_PERSISTENT(CBScriptable, false);

//////////////////////////////////////////////////////////////////////////
CBScriptable::CBScriptable(CBGame* inGame, bool NoValue, bool Persistable):CBNamedObject(inGame)
{
	m_RefCount = 0;

	if(NoValue) m_ScValue = NULL;
	else m_ScValue = new CScValue(Game);

	m_Persistable = Persistable;

	m_ScProp = NULL;
}


//////////////////////////////////////////////////////////////////////////
CBScriptable::~CBScriptable()
{
	//if(m_RefCount>0) Game->LOG(0, "Warning: Destroying object, m_RefCount=%d", m_RefCount);
	SAFE_DELETE(m_ScValue);
	SAFE_DELETE(m_ScProp);
}


//////////////////////////////////////////////////////////////////////////
// high level scripting interface
//////////////////////////////////////////////////////////////////////////
HRESULT CBScriptable::ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name)
{
	/*
	Stack->CorrectParams(0);
	Stack->PushNULL();
	Script->RuntimeError("Call to undefined method '%s'.", Name);

	return S_OK;
	*/
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
CScValue* CBScriptable::ScGetProperty(char *Name)
{
	if(!m_ScProp) m_ScProp = new CScValue(Game);
	if(m_ScProp) return m_ScProp->GetProp(Name);
	else return NULL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBScriptable::ScSetProperty(char *Name, CScValue *Value)
{
	if(!m_ScProp) m_ScProp = new CScValue(Game);
	if(m_ScProp) return m_ScProp->SetProp(Name, Value);
	else return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
char* CBScriptable::ScToString()
{
	return "[native object]";
}

//////////////////////////////////////////////////////////////////////////
void* CBScriptable::ScToMemBuffer()
{
	return (void*)NULL;
}


//////////////////////////////////////////////////////////////////////////
int CBScriptable::ScToInt()
{
	return 0;
}


//////////////////////////////////////////////////////////////////////////
double CBScriptable::ScToFloat()
{
	return 0.0f;
}


//////////////////////////////////////////////////////////////////////////
bool CBScriptable::ScToBool()
{
	return false;
}


//////////////////////////////////////////////////////////////////////////
void CBScriptable::ScSetString(char* Val)
{
}


//////////////////////////////////////////////////////////////////////////
void CBScriptable::ScSetInt(int Val)
{
}


//////////////////////////////////////////////////////////////////////////
void CBScriptable::ScSetFloat(double Val)
{
}


//////////////////////////////////////////////////////////////////////////
void CBScriptable::ScSetBool(bool Val)
{
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBScriptable::Persist(CBPersistMgr *PersistMgr)
{
	PersistMgr->Transfer(TMEMBER(Game));
	PersistMgr->Transfer(TMEMBER(m_RefCount));
	PersistMgr->Transfer(TMEMBER(m_ScProp));
	PersistMgr->Transfer(TMEMBER(m_ScValue));

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
int CBScriptable::ScCompare(CBScriptable *Val)
{
	if(this < Val) return -1;
	else if(this > Val) return 1;
	else return 0;
}

//////////////////////////////////////////////////////////////////////////
void CBScriptable::ScDebuggerDesc(char* Buf, int BufSize)
{
	strcpy_s(Buf, BufSize, ScToString());
}

//////////////////////////////////////////////////////////////////////////
bool CBScriptable::CanHandleMethod(char* EventMethod)
{
	return false;
}


//////////////////////////////////////////////////////////////////////////
CScScript* CBScriptable::InvokeMethodThread(char *MethodName)
{
	return NULL;
}


//////////////////////////////////////////////////////////////////////////
// IWmeObject
//////////////////////////////////////////////////////////////////////////
bool CBScriptable::SendEvent(const char* EventName)
{
	//return SUCCEEDED(ApplyEvent((char*)EventName));
	return false;
}

//////////////////////////////////////////////////////////////////////////
IWmeValue* CBScriptable::CallMethod(const char* MethodName, IWmeParamSet* Params)
{
	CScStack* Stack = new CScStack(Game);
	if(Params) ((CBParamSet*)Params)->PushToStack(Stack);
	else Stack->PushInt(0);

	CScScript* Script = new CScScript(Game, Game->m_ScEngine);
	CBUtils::SetString(&Script->m_Filename, "<temp script>");

	ScCallMethod(Script, Stack, NULL, (char*)MethodName);
	m_ScValue->Copy(Stack->Pop(), true);

	delete Stack;
	delete Script;

	return m_ScValue;
}

//////////////////////////////////////////////////////////////////////////
bool CBScriptable::SetProperty(const char* PropName, IWmeValue* Value)
{
	return SUCCEEDED(ScSetProperty((char*)PropName, (CScValue*)Value));
}

//////////////////////////////////////////////////////////////////////////
bool CBScriptable::SetProperty(const char* PropName, int Value)
{
	CScValue* Val = new CScValue(Game, Value);
	bool Ret = SUCCEEDED(ScSetProperty((char*)PropName, Val));
	delete Val;

	return Ret;
}

//////////////////////////////////////////////////////////////////////////
bool CBScriptable::SetProperty(const char* PropName, const char* Value)
{
	CScValue* Val = new CScValue(Game, (char*)Value);
	bool Ret = SUCCEEDED(ScSetProperty((char*)PropName, Val));
	delete Val;

	return Ret;
}

//////////////////////////////////////////////////////////////////////////
bool CBScriptable::SetProperty(const char* PropName, double Value)
{
	CScValue* Val = new CScValue(Game, Value);
	bool Ret = SUCCEEDED(ScSetProperty((char*)PropName, Val));
	delete Val;

	return Ret;
}

//////////////////////////////////////////////////////////////////////////
bool CBScriptable::SetProperty(const char* PropName, bool Value)
{
	CScValue* Val = new CScValue(Game, Value);
	bool Ret = SUCCEEDED(ScSetProperty((char*)PropName, Val));
	delete Val;

	return Ret;
}

//////////////////////////////////////////////////////////////////////////
bool CBScriptable::SetProperty(const char* PropName, IWmeObject* Value)
{
	CScValue* Val = new CScValue(Game);
	Val->SetNative((CBScriptable*)Value);
	bool Ret = SUCCEEDED(ScSetProperty((char*)PropName, Val));
	delete Val;

	return Ret;
}

//////////////////////////////////////////////////////////////////////////
bool CBScriptable::SetProperty(const char* PropName)
{
	CScValue* Val = new CScValue(Game);
	Val->SetNULL();
	bool Ret = SUCCEEDED(ScSetProperty((char*)PropName, Val));
	delete Val;

	return Ret;
}

//////////////////////////////////////////////////////////////////////////
IWmeValue* CBScriptable::GetProperty(const char* PropName)
{
	return ScGetProperty((char*)PropName);
}

//////////////////////////////////////////////////////////////////////////
void* CBScriptable::GetInterface(const char* ClassName)
{
	if(stricmp(ClassName, "IWmeSubFrame")==0) return (IWmeSubFrame*)((CBSubFrame*)this);
	if(stricmp(ClassName, "IWmeGame")==0) return (IWmeGame*)((CBGame*)this);
	else return this;
}

//////////////////////////////////////////////////////////////////////////
// IWmeDebugObject
//////////////////////////////////////////////////////////////////////////
const char* CBScriptable::DbgGetNativeClass()
{
	return GetClassName();
}

//////////////////////////////////////////////////////////////////////////
IWmeDebugProp* CBScriptable::DbgGetProperty(const char* Name)
{
	return ScGetProperty((char*)Name);
}
