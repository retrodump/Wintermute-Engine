// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "ScStack.h"


IMPLEMENT_PERSISTENT(CScStack, false);

//////////////////////////////////////////////////////////////////////////
CScStack::CScStack(CBGame* inGame):CBBase(inGame)
{
	m_SP = -1;
}


//////////////////////////////////////////////////////////////////////////
CScStack::~CScStack()
{

#if _DEBUG
	//Game->LOG(0, "STAT: Stack size: %d, SP=%d", m_Values.GetSize(), m_SP);
#endif

	for(int i=0; i<m_Values.GetSize(); i++){
		delete m_Values[i];
	}
	m_Values.RemoveAll();
}


//////////////////////////////////////////////////////////////////////////
CScValue* CScStack::Pop()
{
	if(m_SP<0){
		Game->LOG(0, "Fatal: Stack underflow");
		return NULL;
	}

	return m_Values[m_SP--];
}


//////////////////////////////////////////////////////////////////////////
void CScStack::Push(CScValue* Val)
{
	m_SP++;
	
	if(m_SP < m_Values.GetSize()){
		m_Values[m_SP]->Cleanup();
		m_Values[m_SP]->Copy(Val);
	}
	else{
		CScValue* val = new CScValue(Game);
		val->Copy(Val);
		m_Values.Add(val);
	}
}


//////////////////////////////////////////////////////////////////////////
CScValue* CScStack::GetPushValue()
{
	m_SP++;
	
	if(m_SP >= m_Values.GetSize()){
		CScValue* val = new CScValue(Game);
		m_Values.Add(val);
	}
	m_Values[m_SP]->Cleanup();
	return m_Values[m_SP];
}



//////////////////////////////////////////////////////////////////////////
CScValue* CScStack::GetTop()
{
	if(m_SP<0 || m_SP >= m_Values.GetSize()) return NULL;
	else return m_Values[m_SP];
}


//////////////////////////////////////////////////////////////////////////
CScValue* CScStack::GetAt(int Index)
{
	Index = m_SP - Index;
	if(Index<0 || Index >= m_Values.GetSize()) return NULL;
	else return m_Values[Index];
}


//////////////////////////////////////////////////////////////////////////
void CScStack::CorrectParams(DWORD expected_params)
{
	int num_params = Pop()->GetInt();

	if(expected_params < num_params){ // too many params
		while(expected_params < num_params){
			//Pop();
			delete m_Values[m_SP - expected_params];
			m_Values.RemoveAt(m_SP - expected_params);
			num_params--;
			m_SP--;
		}
	}
	else if(expected_params > num_params){ // need more params
		while(expected_params > num_params){
			//Push(null_val);
			CScValue* null_val = new CScValue(Game);
			null_val->SetNULL();
			m_Values.InsertAt(m_SP - num_params+1, null_val);
			num_params++;
			m_SP++;

			if(m_Values.GetSize()>m_SP+1){
				delete m_Values[m_Values.GetSize()-1];
				m_Values.RemoveAt(m_Values.GetSize()-1);
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CScStack::PushNULL()
{
	/*
	CScValue* val = new CScValue(Game);
	val->SetNULL();
	Push(val);
	delete val;
	*/
	GetPushValue()->SetNULL();
}


//////////////////////////////////////////////////////////////////////////
void CScStack::PushInt(int Val)
{
	/*
	CScValue* val = new CScValue(Game);
	val->SetInt(Val);
	Push(val);
	delete val;
	*/
	GetPushValue()->SetInt(Val);
}


//////////////////////////////////////////////////////////////////////////
void CScStack::PushFloat(double Val)
{
	/*
	CScValue* val = new CScValue(Game);
	val->SetFloat(Val);
	Push(val);
	delete val;
	*/
	GetPushValue()->SetFloat(Val);
}


//////////////////////////////////////////////////////////////////////////
void CScStack::PushBool(bool Val)
{
	/*
	CScValue* val = new CScValue(Game);
	val->SetBool(Val);
	Push(val);
	delete val;
	*/
	GetPushValue()->SetBool(Val);
}


//////////////////////////////////////////////////////////////////////////
void CScStack::PushString(char *Val)
{
	/*
	CScValue* val = new CScValue(Game);
	val->SetString(Val);
	Push(val);
	delete val;
	*/
	GetPushValue()->SetString(Val);
}


//////////////////////////////////////////////////////////////////////////
void CScStack::PushNative(CBScriptable *Val, bool Persistent)
{
	/*
	CScValue* val = new CScValue(Game);
	val->SetNative(Val, Persistent);
	Push(val);
	delete val;
	*/

	GetPushValue()->SetNative(Val, Persistent);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CScStack::Persist(CBPersistMgr* PersistMgr){
	
	PersistMgr->Transfer(TMEMBER(Game));

	PersistMgr->Transfer(TMEMBER(m_SP));
	m_Values.Persist(PersistMgr);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
// IWmeStack
//////////////////////////////////////////////////////////////////////////
IWmeValue* CScStack::PopValue()
{
	return Pop();
}

//////////////////////////////////////////////////////////////////////////
bool CScStack::PushValue(int Value)
{
	PushInt(Value);
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CScStack::PushValue(const char* Value)
{
	PushString((char*)Value);
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CScStack::PushValue(double Value)
{
	PushFloat(Value);
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CScStack::PushValue(bool Value)
{
	PushBool(Value);
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CScStack::PushValue(IWmeObject* Value)
{
	PushNative((CBObject*)Value, true);
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CScStack::PushValue(IWmeValue* Value)
{
	Push((CScValue*)Value);
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CScStack::PushValue()
{
	PushNULL();
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CScStack::ExpectParams(int NumParams)
{
	CorrectParams(NumParams);
	return true;
}
