// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\DlgAdvanced.h"

//////////////////////////////////////////////////////////////////////////
CDlgAdvanced::CDlgAdvanced(CBGame* inGame, CHWManager* HwMgr) : CWinDialog()
{
	Game = inGame;
	m_HwMgr = HwMgr;

	m_Btn16Bit = NULL;
	m_BtnSubdivide = NULL;
}

//////////////////////////////////////////////////////////////////////////
CDlgAdvanced::~CDlgAdvanced(void)
{
}

//////////////////////////////////////////////////////////////////////////
HRESULT CDlgAdvanced::RegisterControls()
{
	Game->m_StringTable->LocalizeWindow(m_Hwnd);

	// controls
	m_Btn16Bit = RegisterButton(IDC_16BIT_TEXTURES);
	m_BtnSubdivide = RegisterButton(IDC_TEXTURE_SUBDIVISION);

	// events
	RegisterEvent(IDOK, BN_CLICKED, (EVENT_HANDLER_PROC)&CDlgAdvanced::OnOK);
	RegisterEvent(IDCANCEL, BN_CLICKED, (EVENT_HANDLER_PROC)&CDlgAdvanced::OnCancel);

	// setup
	m_Btn16Bit->SetCheck(m_HwMgr->m_Force16bitTextures);
	m_BtnSubdivide->SetCheck(m_HwMgr->m_ForceTextureSubdivision);		

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
void CDlgAdvanced::OnOK()
{
	m_HwMgr->m_Force16bitTextures = m_Btn16Bit->GetCheck();
	m_HwMgr->m_ForceTextureSubdivision = m_BtnSubdivide->GetCheck();

	EndDialog(IDOK);
}

//////////////////////////////////////////////////////////////////////////
void CDlgAdvanced::OnCancel()
{
	EndDialog(IDCANCEL);
}
