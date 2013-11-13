// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "3DEffectParams.h"

#ifdef WME_D3D9

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
C3DEffectParams::C3DEffectParam::C3DEffectParam()
{
	SetDefaultValues();
}

//////////////////////////////////////////////////////////////////////////
C3DEffectParams::C3DEffectParam::C3DEffectParam(char* paramName)
{
	SetDefaultValues();
	CBUtils::SetString(&m_ParamName, paramName);
}

//////////////////////////////////////////////////////////////////////////
C3DEffectParams::C3DEffectParam::~C3DEffectParam()
{
	SAFE_DELETE_ARRAY(m_ParamName);
	SAFE_DELETE_ARRAY(m_ValString);
}

//////////////////////////////////////////////////////////////////////////
void C3DEffectParams::C3DEffectParam::SetValue(char* val)
{
	m_Type = EP_STRING;
	CBUtils::SetString(&m_ValString, val);
}

//////////////////////////////////////////////////////////////////////////
void C3DEffectParams::C3DEffectParam::SetValue(int val)
{
	m_Type = EP_INT;
	m_ValInt = val;
}

//////////////////////////////////////////////////////////////////////////
void C3DEffectParams::C3DEffectParam::SetValue(float val)
{
	m_Type = EP_FLOAT;
	m_ValFloat = val;
}

//////////////////////////////////////////////////////////////////////////
void C3DEffectParams::C3DEffectParam::SetValue(bool val)
{
	m_Type = EP_BOOL;
	m_ValBool = val;
}

//////////////////////////////////////////////////////////////////////////
void C3DEffectParams::C3DEffectParam::SetValue(D3DXVECTOR4 val)
{
	m_Type = EP_VECTOR;
	m_ValVector = val;
}

//////////////////////////////////////////////////////////////////////////
void C3DEffectParams::C3DEffectParam::SetDefaultValues()
{
	m_ParamName = NULL;
	m_ValString = NULL;
	m_ValInt = 0;
	m_ValFloat = 0;
	m_ValBool = 0;
	m_ValVector = D3DXVECTOR4(0, 0, 0, 0);

	m_Type = EP_UNKNOWN;

	m_Initialized = false;
	m_Handle = NULL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT C3DEffectParams::C3DEffectParam::Persist(CBPersistMgr* PersistMgr)
{
	PersistMgr->Transfer(TMEMBER(m_ParamName));
	PersistMgr->Transfer(TMEMBER_INT(m_Type));
	PersistMgr->Transfer(TMEMBER(m_ValString));
	PersistMgr->Transfer(TMEMBER(m_ValInt));
	PersistMgr->Transfer(TMEMBER(m_ValFloat));
	PersistMgr->Transfer(TMEMBER(m_ValVector));
	PersistMgr->Transfer(TMEMBER(m_ValBool));

	if (!PersistMgr->m_Saving)
	{
		m_Initialized = false;
		m_Handle = NULL;
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
void C3DEffectParams::C3DEffectParam::SetEffectParam(ID3DXEffect* effect)
{
	if (!m_Initialized)
	{
		m_Handle = effect->GetParameterByName(NULL, m_ParamName);
	}

	if (!m_Handle) return;

	switch (m_Type)
	{
	case EP_STRING:
		effect->SetString(m_Handle, m_ValString);
		break;

	case EP_FLOAT:
		effect->SetFloat(m_Handle, m_ValFloat);
		break;

	case EP_INT:
		effect->SetInt(m_Handle, m_ValInt);
		break;

	case EP_BOOL:
		effect->SetBool(m_Handle, m_ValBool);
		break;

	case EP_VECTOR:
		effect->SetVector(m_Handle, &m_ValVector);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
C3DEffectParams::C3DEffectParams()
{
}

//////////////////////////////////////////////////////////////////////////
C3DEffectParams::~C3DEffectParams()
{
	Clear();
}

//////////////////////////////////////////////////////////////////////////
void C3DEffectParams::Clear()
{
	for (int i = 0; i < m_Params.GetSize(); i++)
	{
		SAFE_DELETE(m_Params[i]);
	}
	m_Params.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
HRESULT C3DEffectParams::Persist(CBPersistMgr* PersistMgr)
{
	if (PersistMgr->m_Saving)
	{
		int numItems = m_Params.GetSize();
		PersistMgr->Transfer(TMEMBER(numItems));

		for (int i = 0; i < numItems; i++)
		{
			m_Params[i]->Persist(PersistMgr);
		}
	}
	else
	{
		int numItems = 0;
		PersistMgr->Transfer(TMEMBER(numItems));

		for (int i = 0; i < numItems; i++)
		{
			C3DEffectParam* param = new C3DEffectParam();
			param->Persist(PersistMgr);

			m_Params.Add(param);
		}
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
void C3DEffectParams::SetEffectParams(ID3DXEffect* effect)
{
	if (!effect) return;

	for (int i = 0; i < m_Params.GetSize(); i++)
	{
		m_Params[i]->SetEffectParam(effect);
	}
}

//////////////////////////////////////////////////////////////////////////
void C3DEffectParams::SetParam(char* ParamName, CScValue* Val)
{
	C3DEffectParam* param = GetParamByName(ParamName);

	switch (Val->GetType())
	{
	case VAL_INT:
		param->SetValue(Val->GetInt());
		break;
	case VAL_FLOAT:
		param->SetValue((float)Val->GetFloat());
		break;
	case VAL_BOOL:
		param->SetValue(Val->GetBool());
		break;
	default:
		param->SetValue(Val->GetString());
	}

}


//////////////////////////////////////////////////////////////////////////
void C3DEffectParams::SetParam(char* ParamName, D3DXVECTOR4 Val)
{
	C3DEffectParam* param = GetParamByName(ParamName);
	param->SetValue(Val);
}

//////////////////////////////////////////////////////////////////////////
C3DEffectParams::C3DEffectParam* C3DEffectParams::GetParamByName(char* paramName)
{
	C3DEffectParam* param = NULL;

	for (int i = 0; i < m_Params.GetSize(); i++)
	{
		if (m_Params[i]->GetParamName() && strcmp(paramName, m_Params[i]->GetParamName()) == 0)
		{
			param = m_Params[i];
			break;
		}
	}

	if (!param)
	{
		param = new C3DEffectParam(paramName);
		m_Params.Add(param);
	}
	
	return param;
}

#endif // WME_D3D9