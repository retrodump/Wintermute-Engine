// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\DlgPageAccess.h"
#include "resource.h"


//////////////////////////////////////////////////////////////////////////
CDlgPageAccess::CDlgPageAccess(CBGame* inGame) : CDlgPage(inGame)
{
	m_BtnTTSEnabled = NULL;
	m_BtnTTSTalk = NULL;
	m_BtnTTSCaptions = NULL;
	m_BtnTTSKeypress = NULL;
	m_BtnKeyboardEnabled = NULL;
	m_BtnKeyboardCursorSkip = NULL;
	m_BtnKeyboardPause = NULL;
}

//////////////////////////////////////////////////////////////////////////
CDlgPageAccess::~CDlgPageAccess(void)
{
}


//////////////////////////////////////////////////////////////////////////
char* CDlgPageAccess::GetTabName()
{
	return "/SYSENG0030/Accessibility";
}

//////////////////////////////////////////////////////////////////////////
HRESULT CDlgPageAccess::RegisterControls()
{
	Game->m_StringTable->LocalizeWindow(m_Hwnd);

	// events
	RegisterEvent(IDC_TTS_ENABLED, BN_CLICKED, (EVENT_HANDLER_PROC)&CDlgPageAccess::SetState);
	RegisterEvent(IDC_KEYBOARD_ENABLED, BN_CLICKED, (EVENT_HANDLER_PROC)&CDlgPageAccess::SetState);

	// controls
	m_BtnTTSEnabled = RegisterButton(IDC_TTS_ENABLED);
	m_BtnTTSTalk = RegisterButton(IDC_TTS_TALK);
	m_BtnTTSCaptions = RegisterButton(IDC_TTS_CAPTIONS);
	m_BtnTTSKeypress = RegisterButton(IDC_TTS_KEYPRESS);
	m_BtnKeyboardEnabled = RegisterButton(IDC_KEYBOARD_ENABLED);
	m_BtnKeyboardCursorSkip = RegisterButton(IDC_KEYBOARD_CURSOR_SKIP);
	m_BtnKeyboardPause = RegisterButton(IDC_KEYBOARD_PAUSE);


	// initialize
	m_BtnTTSEnabled->SetCheck(Game->m_AccessTTSEnabled);
	m_BtnTTSTalk->SetCheck(Game->m_AccessTTSTalk);
	m_BtnTTSCaptions->SetCheck(Game->m_AccessTTSCaptions);
	m_BtnTTSKeypress->SetCheck(Game->m_AccessTTSKeypress);
	m_BtnKeyboardEnabled->SetCheck(Game->m_AccessKeyboardEnabled);
	m_BtnKeyboardCursorSkip->SetCheck(Game->m_AccessKeyboardCursorSkip);
	m_BtnKeyboardPause->SetCheck(Game->m_AccessKeyboardPause);

	SetState();

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
void CDlgPageAccess::SetState()
{
	m_BtnTTSTalk->EnableWindow(m_BtnTTSEnabled->GetCheck());
	m_BtnTTSCaptions->EnableWindow(m_BtnTTSEnabled->GetCheck());
	m_BtnTTSKeypress->EnableWindow(m_BtnTTSEnabled->GetCheck());

	m_BtnKeyboardCursorSkip->EnableWindow(m_BtnKeyboardEnabled->GetCheck());
}

//////////////////////////////////////////////////////////////////////////
HRESULT CDlgPageAccess::OnConfirm()
{
	Game->m_AccessTTSEnabled = m_BtnTTSEnabled->GetCheck();
	Game->m_AccessTTSTalk = m_BtnTTSTalk->GetCheck();
	Game->m_AccessTTSCaptions = m_BtnTTSCaptions->GetCheck();
	Game->m_AccessTTSKeypress = m_BtnTTSKeypress->GetCheck();
	Game->m_AccessKeyboardEnabled = m_BtnKeyboardEnabled->GetCheck();
	Game->m_AccessKeyboardCursorSkip = m_BtnKeyboardCursorSkip->GetCheck();
	Game->m_AccessKeyboardPause = m_BtnKeyboardPause->GetCheck();

	return S_OK;
}