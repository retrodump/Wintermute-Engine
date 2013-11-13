// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\BParamSet.h"

//////////////////////////////////////////////////////////////////////////
CBParamSet::CBParamSet(CBGame* inGame) : CBBase(inGame)
{
}

//////////////////////////////////////////////////////////////////////////
CBParamSet::~CBParamSet(void)
{
	Clean();
}

//////////////////////////////////////////////////////////////////////////
bool CBParamSet::AddParam(int Value)
{
	CScValue* Val = new CScValue(Game, Value);
	m_Params.Add(Val);
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CBParamSet::AddParam(const char* Value)
{
	CScValue* Val = new CScValue(Game, (char*)Value);
	m_Params.Add(Val);
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CBParamSet::AddParam(double Value)
{
	CScValue* Val = new CScValue(Game, Value);
	m_Params.Add(Val);
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CBParamSet::AddParam(bool Value)
{
	CScValue* Val = new CScValue(Game, Value);
	m_Params.Add(Val);
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CBParamSet::AddParam(IWmeObject* Value)
{
	CScValue* Val = new CScValue(Game);
	Val->SetNative((CBObject*)Value);
	m_Params.Add(Val);
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CBParamSet::AddParam(IWmeValue* Value)
{
	CScValue* Val = new CScValue(Game);
	Val->Copy((CScValue*)Value, true);
	m_Params.Add(Val);
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CBParamSet::AddParam()
{
	CScValue* Val = new CScValue(Game);
	Val->SetNULL();
	m_Params.Add(Val);
	return true;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBParamSet::PushToStack(CScStack* Stack)
{
	for(int i=m_Params.GetSize()-1; i>=0; i--)
	{
		Stack->Push(m_Params[i]);
	}
	Stack->PushInt(m_Params.GetSize());

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
bool CBParamSet::Clean()
{
	for(int i=0; i<m_Params.GetSize(); i++)
	{
		delete m_Params[i];
	}
	m_Params.RemoveAll();

	return true;
}
