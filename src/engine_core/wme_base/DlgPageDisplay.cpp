// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\DlgPageDisplay.h"
#include "DlgAdvanced.h"
#include "resource.h"

//////////////////////////////////////////////////////////////////////////
CDlgPageDisplay::CDlgPageDisplay(CBGame* inGame, CHWManager* HwMgr) : CDlgPage(inGame)
{
	m_HwMgr = HwMgr;

	m_BtnAccelerated = NULL;
	m_BtnWindowed = NULL;
	m_BtnTL = NULL;
	m_BtnAspect = NULL;
	m_BtnDesktop = NULL;
	m_BtnMore = NULL;
	m_ComboDevice = NULL;
	m_ComboColors = NULL;
	m_ComboFSAA = NULL;
	m_StFSAALabel = NULL;

	m_StWarnText = NULL;
	m_StWarnIcon = NULL;
}

//////////////////////////////////////////////////////////////////////////
CDlgPageDisplay::~CDlgPageDisplay(void)
{
	m_HwMgr = NULL; // reg only
}

//////////////////////////////////////////////////////////////////////////
char* CDlgPageDisplay::GetTabName()
{
	return "/SYSENG0002/Display";
}

//////////////////////////////////////////////////////////////////////////
HRESULT CDlgPageDisplay::RegisterControls()
{
	Game->m_StringTable->LocalizeWindow(m_Hwnd);

	// events
	RegisterEvent(IDC_VIDEO_DEVICE, CBN_SELCHANGE, (EVENT_HANDLER_PROC)&CDlgPageDisplay::OnDeviceSelected);
	RegisterEvent(IDC_COLORS, CBN_SELCHANGE, (EVENT_HANDLER_PROC)&CDlgPageDisplay::OnColorsSelected);
	RegisterEvent(IDC_MULTISAMPLE, CBN_SELCHANGE, (EVENT_HANDLER_PROC)&CDlgPageDisplay::OnFSAASelected);
	RegisterEvent(IDC_ACCELERATED, BN_CLICKED, (EVENT_HANDLER_PROC)&CDlgPageDisplay::OnAccelerated);
	RegisterEvent(IDC_WINDOWED, BN_CLICKED, (EVENT_HANDLER_PROC)&CDlgPageDisplay::OnWindowed);
	RegisterEvent(IDC_TL, BN_CLICKED, (EVENT_HANDLER_PROC)&CDlgPageDisplay::OnTL);
	RegisterEvent(IDC_MAINTAIN_ASPECT, BN_CLICKED, (EVENT_HANDLER_PROC)&CDlgPageDisplay::OnAspect);
	RegisterEvent(IDC_USE_DESKTOP_RES, BN_CLICKED, (EVENT_HANDLER_PROC)&CDlgPageDisplay::OnDesktop);
	RegisterEvent(IDC_ADVANCED, BN_CLICKED, (EVENT_HANDLER_PROC)&CDlgPageDisplay::OnMore);


	// controls
	m_BtnAccelerated = RegisterButton(IDC_ACCELERATED);
	m_BtnWindowed = RegisterButton(IDC_WINDOWED);
	m_BtnTL = RegisterButton(IDC_TL);
	m_BtnAspect = RegisterButton(IDC_MAINTAIN_ASPECT);
	m_BtnDesktop = RegisterButton(IDC_USE_DESKTOP_RES);
	m_BtnMore = RegisterButton(IDC_ADVANCED);
	m_ComboDevice = RegisterCombo(IDC_VIDEO_DEVICE);
	m_ComboColors = RegisterCombo(IDC_COLORS);
	m_ComboFSAA = RegisterCombo(IDC_MULTISAMPLE);
	m_StFSAALabel = RegisterStatic(IDC_MULTISAMPLE_LABEL);

	m_StWarnText = RegisterStatic(IDC_WARN_TEXT);
	m_StWarnIcon = RegisterStatic(IDC_WARN_ICON);



	// initialize
	bool AccelAvail=false, CompatAvail=false;
	for(int i=0; i<m_HwMgr->m_VideoDevices.GetSize(); i++)
	{
		if(m_HwMgr->m_VideoDevices[i]->m_Accelerated) AccelAvail = true;
		else CompatAvail = true;
	}

	// enable accelerated
	m_BtnAccelerated->EnableWindow(!m_HwMgr->m_ReqAcclerated && AccelAvail && CompatAvail);
	m_BtnAccelerated->ShowWindow(m_HwMgr->m_ReqAcclerated?SW_HIDE:SW_SHOW);

	// check accelerated
	if(	m_HwMgr->m_ReqAcclerated ||
		(m_HwMgr->m_SelectedVideoDevice && m_HwMgr->m_SelectedVideoDevice->m_Accelerated)||
		(!m_HwMgr->m_SelectedVideoDevice && AccelAvail))
		m_BtnAccelerated->SetCheck(true);

	// windowed/fullscreen only
	if(m_HwMgr->m_ReqWindowedOnly)
	{
		m_BtnWindowed->SetCheck(true);
		m_HwMgr->m_SelectedWindowed = true;
	}
	if(m_HwMgr->m_ReqFullscreenOnly)
	{
		m_BtnWindowed->SetCheck(false);
		m_BtnWindowed->ShowWindow(SW_HIDE);
		m_HwMgr->m_SelectedWindowed = false;		
	}

	UpdateDevices();

	if(m_HwMgr->m_ReqAcclerated && m_HwMgr->m_VideoDevices.GetSize()==0)
	{
		m_StWarnText->ShowWindow();
		m_StWarnIcon->ShowWindow();

		m_StWarnText->SetWindowText((char*)Game->m_StringTable->ExpandStatic("/SYSENG0025/This game requires 3D acceleration which doesn't seem to be supported by your video card. Please make sure you have a 3D accelerated video card and that you have the latest video drivers installed."), Game->m_TextEncoding);
	}
	else
	{
		m_StWarnText->ShowWindow(SW_HIDE);
		m_StWarnIcon->ShowWindow(SW_HIDE);
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
bool CDlgPageDisplay::IsValid()
{
	return m_HwMgr->m_SelectedVideoDevice!=NULL;
}


//////////////////////////////////////////////////////////////////////////
void CDlgPageDisplay::UpdateDevices()
{
	bool Accelerated = m_BtnAccelerated->GetCheck();
	//m_BtnMore->ShowWindow(Accelerated?SW_SHOW:SW_HIDE);
	m_BtnMore->ShowWindow(SW_HIDE);

	int ToSet = 0;
	int Item;

	m_ComboDevice->ResetContent();

	for(int i=0; i<m_HwMgr->m_VideoDevices.GetSize(); i++)
	{
		if(m_HwMgr->m_VideoDevices[i]->m_Accelerated != Accelerated) continue;

		// remember device index
		Item = m_ComboDevice->AddString(m_HwMgr->m_VideoDevices[i]->m_Descript, TEXT_ANSI);
		m_ComboDevice->SetItemData(Item, (void*)i);

		if(	m_HwMgr->m_SelectedVideoDevice &&
			m_HwMgr->m_VideoDevices[i]->m_Guid == m_HwMgr->m_SelectedVideoDevice->m_Guid &&
			m_HwMgr->m_VideoDevices[i]->m_AdapterID == m_HwMgr->m_SelectedVideoDevice->m_AdapterID)
			ToSet = Item;
	}
	m_ComboDevice->SetCurSel(ToSet);

	Item = m_ComboDevice->GetCurSel();
	if(Item==CB_ERR) m_HwMgr->m_SelectedVideoDevice = NULL;
	else m_HwMgr->m_SelectedVideoDevice = m_HwMgr->m_VideoDevices[(int)m_ComboDevice->GetItemData(Item)];

	UpdateSettings();
}


//////////////////////////////////////////////////////////////////////////
void CDlgPageDisplay::UpdateSettings()
{
	int ToSet = 0;
	int Item;

	// colors
	m_ComboColors->ResetContent();

	if(m_HwMgr->m_SelectedVideoDevice)
	{
		if(m_HwMgr->m_SelectedVideoDevice->m_Supports16bit)
		{
			Item = m_ComboColors->AddString((char*)Game->m_StringTable->ExpandStatic("/syseng0017/16 bit"), Game->m_TextEncoding);
			m_ComboColors->SetItemData(Item, (void*)1);
			if(m_HwMgr->m_Selected16bitColors) ToSet = Item;
		}
		if(m_HwMgr->m_SelectedVideoDevice->m_Supports32bit)
		{
			Item = m_ComboColors->AddString((char*)m_HwMgr->Game->m_StringTable->ExpandStatic("/syseng0018/32 bit"), Game->m_TextEncoding);
			m_ComboColors->SetItemData(Item, (void*)0);
			if(!m_HwMgr->m_Selected16bitColors) ToSet = Item;
		}
	}
	m_ComboColors->SetCurSel(ToSet);
	Item = m_ComboColors->GetCurSel();
	if(Item==CB_ERR) m_HwMgr->m_Selected16bitColors = true;
	else m_HwMgr->m_Selected16bitColors = (m_ComboColors->GetItemData(Item)!=0);

	// windowed
	if(	m_HwMgr->m_SelectedVideoDevice &&
		m_HwMgr->m_SelectedVideoDevice->m_SupportsWindowed &&
		m_HwMgr->m_SelectedVideoDevice->m_SupportsFullscreen &&
		!m_HwMgr->m_ReqFullscreenOnly &&
		!m_HwMgr->m_ReqWindowedOnly)
	{
		m_BtnWindowed->EnableWindow(true);
	}
	else
	{
		m_BtnWindowed->EnableWindow(false);
	}

	if(m_HwMgr->m_SelectedVideoDevice && !m_HwMgr->m_SelectedVideoDevice->m_SupportsFullscreen)
		m_HwMgr->m_SelectedWindowed = true;

	m_BtnWindowed->SetCheck(m_HwMgr->m_SelectedWindowed);

	// T&L
	if(m_HwMgr->m_SelectedVideoDevice && m_HwMgr->m_SelectedVideoDevice->m_SupportsTL && m_HwMgr->m_TLMode == 2)
	{
		m_BtnTL->EnableWindow(true);
		m_BtnTL->SetCheck(m_HwMgr->m_SelectedTL);
	}
	else
	{
		m_BtnTL->EnableWindow(false);
		m_BtnTL->SetCheck(false);

		m_HwMgr->m_SelectedTL = false;
	}

	m_BtnAspect->SetCheck(m_HwMgr->m_SelectedMaintainAspect);
	m_BtnDesktop->SetCheck(m_HwMgr->m_SelectedDesktopRes);


	UpdateState();
	UpdateMultisample();
}


//////////////////////////////////////////////////////////////////////////
void CDlgPageDisplay::UpdateState()
{
	if(m_HwMgr->m_SelectedVideoDevice && m_HwMgr->m_SelectedVideoDevice->m_Accelerated && m_HwMgr->m_AllowAdvanced3DSettings)
	{
		m_ComboFSAA->ShowWindow(SW_SHOW);
		m_StFSAALabel->ShowWindow(SW_SHOW);

		if(m_HwMgr->m_TLMode==2) m_BtnTL->ShowWindow(SW_SHOW);
		else m_BtnTL->ShowWindow(SW_HIDE);
	}
	else
	{
		m_ComboFSAA->ShowWindow(SW_HIDE);
		m_StFSAALabel->ShowWindow(SW_HIDE);
		m_BtnTL->ShowWindow(SW_HIDE);
	}
	m_ComboColors->EnableWindow(!m_HwMgr->m_SelectedWindowed);


	if(m_HwMgr->m_SelectedVideoDevice && m_HwMgr->m_SelectedVideoDevice->m_HasAltRes)
	{
		m_BtnAspect->ShowWindow(SW_SHOW);
		m_BtnAspect->EnableWindow(!m_HwMgr->m_SelectedWindowed);
	}
	else m_BtnAspect->ShowWindow(SW_HIDE);


	if(m_HwMgr->m_SelectedVideoDevice && m_HwMgr->m_SelectedVideoDevice->m_HasDesktopRes)
	{
		m_BtnDesktop->ShowWindow(SW_SHOW);
		m_BtnDesktop->EnableWindow(!m_HwMgr->m_SelectedWindowed);

		if(!m_HwMgr->m_SelectedWindowed && !m_HwMgr->m_SelectedVideoDevice->m_HasRequestedRes && !m_HwMgr->m_SelectedVideoDevice->m_HasBackupRes)
		{
			m_BtnDesktop->EnableWindow(false);
			m_BtnDesktop->SetCheck(true);
		}
	}
	else m_BtnDesktop->ShowWindow(SW_HIDE);

}


//////////////////////////////////////////////////////////////////////////
void CDlgPageDisplay::UpdateMultisample()
{
	// multisample
	bool multisample[17];
	memset(multisample, 0, sizeof(multisample));

	int ToSet=0;
	int Item;

	m_ComboFSAA->ResetContent();

	if(m_HwMgr->m_SelectedVideoDevice)
	{
		CHWDeviceVideo* dev = m_HwMgr->m_SelectedVideoDevice;
		for(int m=0; m<dev->m_Modes.GetSize(); m++)
		{
			if((m_HwMgr->m_Selected16bitColors && dev->m_Modes[m]->m_BPP!=16) || (!m_HwMgr->m_Selected16bitColors && dev->m_Modes[m]->m_BPP!=32)) continue;
			for(int i=0; i<=16; i++)
			{
				if(dev->m_Modes[m]->m_Multisample[i]) multisample[i] = true;
			}
			multisample[0] = true;
		}

		char sample[512];
		for(int i=0; i<=16; i++)
		{
			if(!multisample[i]) continue;

			if(i==0)
			{
				sprintf(sample, "%s", Game->m_StringTable->ExpandStatic("/syseng0020/none"));
			}
			else 
			{
				sprintf(sample, "%d %s", i, Game->m_StringTable->ExpandStatic("/syseng0023/x"));
			}
			Item = m_ComboFSAA->AddString(sample, Game->m_TextEncoding);
			m_ComboFSAA->SetItemData(Item, (void*)i);

			if(m_HwMgr->m_SelectedMultisample==i) ToSet = Item;
		}
	}

	m_ComboFSAA->SetCurSel(ToSet);
	Item = m_ComboFSAA->GetCurSel();
	if(Item==CB_ERR) m_HwMgr->m_SelectedMultisample = 0;
	else m_HwMgr->m_SelectedMultisample = (int)m_ComboFSAA->GetItemData(Item);
}


//////////////////////////////////////////////////////////////////////////
void CDlgPageDisplay::OnDeviceSelected()
{
	int Item = m_ComboDevice->GetCurSel();
	if(Item==CB_ERR) m_HwMgr->m_SelectedVideoDevice = NULL;
	else m_HwMgr->m_SelectedVideoDevice = m_HwMgr->m_VideoDevices[(int)m_ComboDevice->GetItemData(Item)];

	UpdateSettings();
}

//////////////////////////////////////////////////////////////////////////
void CDlgPageDisplay::OnColorsSelected()
{
	int Item = m_ComboColors->GetCurSel();
	if(Item==CB_ERR) m_HwMgr->m_Selected16bitColors = true;
	else m_HwMgr->m_Selected16bitColors = (m_ComboColors->GetItemData(Item)!=0);
}

//////////////////////////////////////////////////////////////////////////
void CDlgPageDisplay::OnFSAASelected()
{
	int Item = m_ComboFSAA->GetCurSel();
	if(Item==CB_ERR) m_HwMgr->m_SelectedMultisample = 0;
	else m_HwMgr->m_SelectedMultisample = (int)m_ComboFSAA->GetItemData(Item);
}

//////////////////////////////////////////////////////////////////////////
void CDlgPageDisplay::OnAccelerated()
{
	UpdateDevices();
}

//////////////////////////////////////////////////////////////////////////
void CDlgPageDisplay::OnWindowed()
{
	m_HwMgr->m_SelectedWindowed = m_BtnWindowed->GetCheck();
	UpdateState();
}

//////////////////////////////////////////////////////////////////////////
void CDlgPageDisplay::OnTL()
{
	m_HwMgr->m_SelectedTL = m_BtnTL->GetCheck();
}

//////////////////////////////////////////////////////////////////////////
void CDlgPageDisplay::OnAspect()
{
	m_HwMgr->m_SelectedMaintainAspect = m_BtnAspect->GetCheck();
	if(m_HwMgr->m_SelectedMaintainAspect)
	{
		m_HwMgr->m_SelectedDesktopRes = false;
		m_BtnDesktop->SetCheck(false);
	}
}

//////////////////////////////////////////////////////////////////////////
void CDlgPageDisplay::OnDesktop()
{
	m_HwMgr->m_SelectedDesktopRes = m_BtnDesktop->GetCheck();
	if(m_HwMgr->m_SelectedDesktopRes)
	{
		m_HwMgr->m_SelectedMaintainAspect = false;
		m_BtnAspect->SetCheck(false);
	}
}


//////////////////////////////////////////////////////////////////////////
void CDlgPageDisplay::OnMore()
{
	CDlgAdvanced* Dlg = new CDlgAdvanced(Game, m_HwMgr);
	Dlg->CreateIndirect(IDD_ADVANCED, NULL, NULL, Game->m_TextRTL);
	Dlg->DoModal();
	delete Dlg;
}
