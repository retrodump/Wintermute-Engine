// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\DlgPageSound.h"
#include "resource.h"


//////////////////////////////////////////////////////////////////////////
CDlgPageSound::CDlgPageSound(CBGame* inGame, CHWManager* HwMgr) : CDlgPage(inGame)
{
	m_HwMgr = HwMgr;

	m_ComboDevice = NULL;

	m_StWarnText = NULL;
	m_StWarnIcon = NULL;

}

//////////////////////////////////////////////////////////////////////////
CDlgPageSound::~CDlgPageSound(void)
{
	m_HwMgr = NULL; // reg only
}

//////////////////////////////////////////////////////////////////////////
char* CDlgPageSound::GetTabName()
{
	return "/SYSENG0003/Sound";
}

//////////////////////////////////////////////////////////////////////////
HRESULT CDlgPageSound::RegisterControls()
{
	Game->m_StringTable->LocalizeWindow(m_Hwnd);

	// events
	RegisterEvent(IDC_AUDIO_DEVICE, CBN_SELCHANGE, (EVENT_HANDLER_PROC)&CDlgPageSound::OnDeviceSelected);

	// controls
	m_ComboDevice = RegisterCombo(IDC_AUDIO_DEVICE);
	m_StWarnText = RegisterStatic(IDC_WARN_TEXT);
	m_StWarnIcon = RegisterStatic(IDC_WARN_ICON);

	// setup
	UpdateDevices();

	if(m_HwMgr->m_ReqSound && m_HwMgr->m_AudioDevices.GetSize()==0)
	{
		m_StWarnText->ShowWindow();
		m_StWarnIcon->ShowWindow();

		m_StWarnText->SetWindowText((char*)Game->m_StringTable->ExpandStatic("/SYSENG0028/This game requires a sound card which doesn't seem to be available. Please make sure you have a sound card and that you have the latest audio drivers installed."), Game->m_TextEncoding);
	}
	else
	{
		m_StWarnText->ShowWindow(SW_HIDE);
		m_StWarnIcon->ShowWindow(SW_HIDE);
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
bool CDlgPageSound::IsValid()
{
	return m_HwMgr->m_SelectedAudioDevice!=NULL;
}

//////////////////////////////////////////////////////////////////////////
void CDlgPageSound::UpdateDevices()
{
	int ToSet = 0;
	int Item;

	m_ComboDevice->ResetContent();

	for(int i=0; i<m_HwMgr->m_AudioDevices.GetSize(); i++)
	{
		Item = m_ComboDevice->AddString(m_HwMgr->m_AudioDevices[i]->m_Descript, TEXT_ANSI);
		m_ComboDevice->SetItemData(Item, (void*)i);

		if(m_HwMgr->m_AudioDevices[i] == m_HwMgr->m_SelectedAudioDevice) ToSet = Item;
	}
	m_ComboDevice->SetCurSel(ToSet);
	Item = m_ComboDevice->GetCurSel();
	if(Item==CB_ERR) m_HwMgr->m_SelectedAudioDevice = NULL;
	else m_HwMgr->m_SelectedAudioDevice = m_HwMgr->m_AudioDevices[(int)m_ComboDevice->GetItemData(Item)];
}

//////////////////////////////////////////////////////////////////////////
void CDlgPageSound::OnDeviceSelected()
{
	int Item = m_ComboDevice->GetCurSel();
	if(Item==CB_ERR) m_HwMgr->m_SelectedAudioDevice = NULL;
	else m_HwMgr->m_SelectedAudioDevice = m_HwMgr->m_AudioDevices[(int)m_ComboDevice->GetItemData(Item)];
}
