// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BKeyboardState.h"


IMPLEMENT_PERSISTENT(CBKeyboardState, false);

//////////////////////////////////////////////////////////////////////////
CBKeyboardState::CBKeyboardState(CBGame* inGame):CBScriptable(inGame)
{
	m_CurrentPrintable = false;
	m_CurrentCharCode = 0;
	m_CurrentKeyData = 0;

	m_CurrentShift = false;
	m_CurrentAlt = false;
	m_CurrentControl = false;
}


//////////////////////////////////////////////////////////////////////////
CBKeyboardState::~CBKeyboardState()
{

}


//////////////////////////////////////////////////////////////////////////
// high level scripting interface
//////////////////////////////////////////////////////////////////////////
HRESULT CBKeyboardState::ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name)
{
	//////////////////////////////////////////////////////////////////////////
	// IsKeyDown
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "IsKeyDown")==0){
		Stack->CorrectParams(1);
		CScValue* val = Stack->Pop();
		int vKey;

		if(val->m_Type==VAL_STRING && strlen(val->GetString())>0){
			char* str = val->GetString();
			if(str[0]>='a' && str[0]<='z') str[0] -= ('a' - 'A');
			vKey = (int)str[0];
		}
		else vKey = val->GetInt();

		Stack->PushBool((GetAsyncKeyState(vKey) & 0x8000)!=0);
		return S_OK;
	}

	else return CBScriptable::ScCallMethod(Script, Stack, ThisStack, Name);
}


//////////////////////////////////////////////////////////////////////////
CScValue* CBKeyboardState::ScGetProperty(char *Name)
{
	m_ScValue->SetNULL();

	//////////////////////////////////////////////////////////////////////////
	// Type
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Type")==0){
		m_ScValue->SetString("keyboard");
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// Key
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Key")==0){
		if(m_CurrentPrintable){
			char key[2];
			key[0] = (char)m_CurrentCharCode;
			key[1] = '\0';
			m_ScValue->SetString(key);
		}
		else m_ScValue->SetString("");

		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// Printable
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Printable")==0){
		m_ScValue->SetBool(m_CurrentPrintable);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// KeyCode
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "KeyCode")==0){
		m_ScValue->SetInt(m_CurrentCharCode);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// IsShift
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "IsShift")==0){
		m_ScValue->SetBool(m_CurrentShift);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// IsAlt
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "IsAlt")==0){
		m_ScValue->SetBool(m_CurrentAlt);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// IsControl
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "IsControl")==0){
		m_ScValue->SetBool(m_CurrentControl);
		return m_ScValue;
	}

	else return CBScriptable::ScGetProperty(Name);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBKeyboardState::ScSetProperty(char *Name, CScValue *Value)
{
	/*
	//////////////////////////////////////////////////////////////////////////
	// Name
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Name")==0){
		SetName(Value->GetString());
		if(m_Renderer) SetWindowText(m_Renderer->m_Window, m_Name);
		return S_OK;
	}

	else*/ return CBScriptable::ScSetProperty(Name, Value);
}


//////////////////////////////////////////////////////////////////////////
char* CBKeyboardState::ScToString()
{
	return "[keyboard state]";
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBKeyboardState::ReadKey(bool Printable, DWORD CharCode, DWORD KeyData)
{
	m_CurrentPrintable = Printable;
	m_CurrentCharCode = CharCode;
	m_CurrentKeyData = KeyData;

	m_CurrentControl = ((GetAsyncKeyState(VK_CONTROL) & 0x8000)!=0 || (GetAsyncKeyState(VK_RCONTROL) & 0x8000)!=0);
	m_CurrentAlt     = ((GetAsyncKeyState(VK_MENU)    & 0x8000)!=0 || (GetAsyncKeyState(VK_RMENU)    & 0x8000)!=0);
	m_CurrentShift   = ((GetAsyncKeyState(VK_SHIFT)   & 0x8000)!=0 || (GetAsyncKeyState(VK_RSHIFT)   & 0x8000)!=0);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBKeyboardState::Persist(CBPersistMgr *PersistMgr)
{
	//if(!PersistMgr->m_Saving) Cleanup();
	CBScriptable::Persist(PersistMgr);

	PersistMgr->Transfer(TMEMBER(m_CurrentAlt));
	PersistMgr->Transfer(TMEMBER(m_CurrentCharCode));
	PersistMgr->Transfer(TMEMBER(m_CurrentControl));
	PersistMgr->Transfer(TMEMBER(m_CurrentKeyData));
	PersistMgr->Transfer(TMEMBER(m_CurrentPrintable));
	PersistMgr->Transfer(TMEMBER(m_CurrentShift));

	return S_OK;
}
