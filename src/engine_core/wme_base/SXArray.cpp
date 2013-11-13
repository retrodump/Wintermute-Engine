// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "SXArray.h"


IMPLEMENT_PERSISTENT(CSXArray, false);

//////////////////////////////////////////////////////////////////////////
CSXArray::CSXArray(CBGame* inGame, CScStack* Stack):CBScriptable(inGame)
{
	m_Length = 0;
	m_Values = new CScValue(Game);

	int NumParams = Stack->Pop()->GetInt(0);

	if(NumParams==1) m_Length = Stack->Pop()->GetInt(0);
	else if(NumParams>1){
		m_Length = NumParams;
		char ParamName[20];
		for(int i=0; i<NumParams; i++){
			sprintf(ParamName, "%d", i);
			m_Values->SetProp(ParamName, Stack->Pop());
		}
	}
}

//////////////////////////////////////////////////////////////////////////
CSXArray::CSXArray(CBGame* inGame):CBScriptable(inGame)
{
	m_Length = 0;
	m_Values = new CScValue(Game);
}


//////////////////////////////////////////////////////////////////////////
CSXArray::~CSXArray()
{
	SAFE_DELETE(m_Values);
}


//////////////////////////////////////////////////////////////////////////
char* CSXArray::ScToString()
{
	static char Dummy[32768];
	strcpy(Dummy, "");
	char PropName[20];
	for(int i=0; i<m_Length; i++){
		sprintf(PropName, "%d", i);
		CScValue* val = m_Values->GetProp(PropName);
		if(val){
			if(strlen(Dummy) + strlen(val->GetString()) < 32768){
				strcat(Dummy, val->GetString());
			}
		}

		if(i<m_Length-1 && strlen(Dummy)+1 < 32768) strcat(Dummy, ",");
	}
	return Dummy;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CSXArray::ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name)
{	
	//////////////////////////////////////////////////////////////////////////
	// Push
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Push")==0){
		int NumParams = Stack->Pop()->GetInt(0);
		char ParamName[20];
		
		for(int i=0; i<NumParams; i++){
			m_Length++;
			sprintf(ParamName, "%d", m_Length-1);			
			m_Values->SetProp(ParamName, Stack->Pop(), true);
		}
		Stack->PushInt(m_Length);

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// Pop
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Pop")==0){
		
		Stack->CorrectParams(0);
		
		if(m_Length>0){
			char ParamName[20];
			sprintf(ParamName, "%d", m_Length-1);
			Stack->Push(m_Values->GetProp(ParamName));
			m_Values->DeleteProp(ParamName);
			m_Length--;
		}
		else Stack->PushNULL();

		return S_OK;
	}

	else return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
CScValue* CSXArray::ScGetProperty(char *Name)
{
	m_ScValue->SetNULL();

	//////////////////////////////////////////////////////////////////////////
	// Type
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Type")==0){
		m_ScValue->SetString("array");
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// Length
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Length")==0){
		m_ScValue->SetInt(m_Length);
		return m_ScValue;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// [number]
	//////////////////////////////////////////////////////////////////////////
	else{
		char ParamName[20];
		if(ValidNumber(Name, ParamName)){
			return m_Values->GetProp(ParamName);
		}
		else return m_ScValue;
	}	
}


//////////////////////////////////////////////////////////////////////////
HRESULT CSXArray::ScSetProperty(char *Name, CScValue *Value)
{
	//////////////////////////////////////////////////////////////////////////
	// Length
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Length")==0){
		int OrigLength = m_Length;
		m_Length = max(Value->GetInt(0), 0);
		
		char PropName[20];
		if(m_Length < OrigLength){
			for(int i=m_Length; i<OrigLength; i++){
				sprintf(PropName, "%d", i);
				m_Values->DeleteProp(PropName);
			}
		}
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// [number]
	//////////////////////////////////////////////////////////////////////////
	else{
		char ParamName[20];
		if(ValidNumber(Name, ParamName)){
			int Index = atoi(ParamName);
			if(Index>=m_Length) m_Length = Index + 1;
			return m_Values->SetProp(ParamName, Value);
		}
		else return E_FAIL;
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT CSXArray::Persist(CBPersistMgr* PersistMgr)
{
	CBScriptable::Persist(PersistMgr);

	PersistMgr->Transfer(TMEMBER(m_Length));
	PersistMgr->Transfer(TMEMBER(m_Values));

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
bool CSXArray::ValidNumber(const char *OrigStr, char *OutStr)
{
	bool IsNumber = true;
	for(int i=0; i<strlen(OrigStr); i++){
		if(!isdigit(OrigStr[i])){
			IsNumber = false;
			break;
		}
	}

	if(IsNumber){
		int Index = atoi(OrigStr);
		sprintf(OutStr, "%d", Index);
		return true;
	}
	else return false;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CSXArray::Push(CScValue* Val)
{
	char ParamName[20];
	m_Length++;
	sprintf(ParamName, "%d", m_Length-1);
	m_Values->SetProp(ParamName, Val, true);
	return S_OK;
}