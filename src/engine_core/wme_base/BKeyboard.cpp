// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BKeyboard.h"


//////////////////////////////////////////////////////////////////////
CBKeyboard::CBKeyboard(CBGame* inGame):CBBase(inGame)
{
	m_DIKeyboard = NULL;
}


//////////////////////////////////////////////////////////////////////
CBKeyboard::~CBKeyboard()
{
	if(m_DIKeyboard) m_DIKeyboard->Release();
}


//////////////////////////////////////////////////////////////////////
HRESULT CBKeyboard::Create(HINSTANCE hInst, HWND hWnd)
{
	HRESULT rval;

	rval = Game->m_DirectX.DirectInputCreate(hInst, DIRECTINPUT_VERSION, &m_DirectInput, NULL);
	if(FAILED(rval)) return rval;

	// Create the keyboard device
	rval = m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_DIKeyboard, NULL);
	if(rval == DI_OK)
	{
		rval = m_DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
		if(FAILED(rval)) return rval;

		rval = m_DIKeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		if(FAILED(rval)) return rval;

		rval = m_DIKeyboard->Acquire();
		if(FAILED(rval)) return rval;
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////
void CBKeyboard::ReAcquire()
{
	m_DIKeyboard->Acquire();
}


//////////////////////////////////////////////////////////////////////
void CBKeyboard::UnAcquire()
{
	m_DIKeyboard->Unacquire();
}


//////////////////////////////////////////////////////////////////////
void CBKeyboard::Update()
{
	HRESULT ret;

	ret = m_DIKeyboard->GetDeviceState(256, &Keys);

	if(ret == DIERR_INPUTLOST || ret == DIERR_NOTACQUIRED)
		m_DIKeyboard->Acquire();
}


//////////////////////////////////////////////////////////////////////
bool CBKeyboard::Key(BYTE Key)
{
	return ((Keys[Key] & 0x80) != 0);
}
