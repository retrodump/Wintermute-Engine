// SpritePropPage.cpp : implementation file
//

#include "stdafx.h"
#include "SpriteEdit.h"
#include "SpritePropPage.h"
#include "MainFrm.h"
#include "ScriptDlg.h"
#include ".\spriteproppage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpritePropPage dialog


//////////////////////////////////////////////////////////////////////////
CSpritePropPage::CSpritePropPage(CWnd* pParent /*=NULL*/)
	: CDialog(CSpritePropPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpritePropPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_View = NULL;
}


//////////////////////////////////////////////////////////////////////////
void CSpritePropPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpritePropPage)
	DDX_Control(pDX, IDC_STREAMED, m_Streamed);
	DDX_Control(pDX, IDC_STREAMED_KEEP_LOADED, m_KeepLoaded);
	DDX_Control(pDX, IDC_MUTED, m_Muted);
	DDX_Control(pDX, IDC_PRECISE, m_Precise);
	DDX_Control(pDX, IDC_NAME, m_Name);
	DDX_Control(pDX, IDC_LOOPING, m_Looping);
	DDX_Control(pDX, IDC_CONTINUOS, m_Continuous);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SCRIPTS, m_Sprites);
}


BEGIN_MESSAGE_MAP(CSpritePropPage, CDialog)
	//{{AFX_MSG_MAP(CSpritePropPage)
	ON_EN_CHANGE(IDC_NAME, OnChangeName)
	ON_BN_CLICKED(IDC_LOOPING, OnLooping)
	ON_BN_CLICKED(IDC_PRECISE, OnPrecise)
	ON_BN_CLICKED(IDC_CONTINUOS, OnContinuos)
	ON_BN_CLICKED(IDC_MUTED, OnMuted)
	ON_BN_CLICKED(IDC_STREAMED, OnStreamed)
	ON_BN_CLICKED(IDC_STREAMED_KEEP_LOADED, OnKeepLoaded)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SCRIPTS, OnBnClickedScripts)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpritePropPage message handlers


//////////////////////////////////////////////////////////////////////////
void CSpritePropPage::OnOK() 
{
	
}


//////////////////////////////////////////////////////////////////////////
void CSpritePropPage::OnCancel() 
{
	// do nothing
}


//////////////////////////////////////////////////////////////////////////
void CSpritePropPage::SetCurrentView(CSpriteView *View)
{
	if(View==m_View) return;
	
	m_View = View;
	Redraw();
}


//////////////////////////////////////////////////////////////////////////
void CSpritePropPage::Redraw()
{
	SetState();

	if(m_View){
		m_Name.SetWindowText(m_View->m_Sprite->m_Name);
		m_Looping.SetCheck(m_View->m_Sprite->m_Looping);
		m_Continuous.SetCheck(m_View->m_Sprite->m_Continuous);
		m_Precise.SetCheck(m_View->m_Sprite->m_Precise);
		m_Muted.SetCheck(m_View->m_Sprite->m_EditorMuted);
		m_Streamed.SetCheck(m_View->m_Sprite->m_Streamed);
		m_KeepLoaded.SetCheck(m_View->m_Sprite->m_StreamedKeepLoaded);
	}
}


//////////////////////////////////////////////////////////////////////////
void CSpritePropPage::SetState()
{
	m_Name.EnableWindow(m_View!=NULL);
	m_Looping.EnableWindow(m_View!=NULL);
	m_Continuous.EnableWindow(m_View!=NULL);
	m_Precise.EnableWindow(m_View!=NULL);
	m_Muted.EnableWindow(m_View!=NULL);
	m_Streamed.EnableWindow(m_View!=NULL);
	m_KeepLoaded.EnableWindow(m_View!=NULL);
	m_Sprites.EnableWindow(m_View!=NULL);

	if(m_View && m_View->m_Sprite && m_View->m_Sprite->m_Scripts.GetSize()>0)
		m_Sprites.SetFont(&m_BoldFont);
	else
		m_Sprites.SetFont(GetFont());

}


//////////////////////////////////////////////////////////////////////////
BOOL CSpritePropPage::OnInitDialog() 
{
	CDialog::OnInitDialog();

	LOGFONT logFont;
	CFont* pFont = GetFont();
	pFont->GetLogFont(&logFont);
	logFont.lfWeight = FW_BOLD;
	m_BoldFont.CreateFontIndirect(&logFont);

	SetState();
	
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
void CSpritePropPage::OnChangeName() 
{
	if(m_View){
		CString str;
		m_Name.GetWindowText(str);
		if(str.Compare(m_View->m_Sprite->m_Name)!=0){
			m_View->SetUndoState("Change name");
			m_View->m_Sprite->SetName((char*)LPCSTR(str));
		}
	}	
}


//////////////////////////////////////////////////////////////////////////
void CSpritePropPage::OnLooping() 
{
	m_View->SetUndoState("Change looping");
	m_View->m_Sprite->m_Looping = m_Looping.GetCheck()!=FALSE;
	((CMainFrame*)AfxGetMainWnd())->m_wndWorkSpace.Refresh();
}


//////////////////////////////////////////////////////////////////////////
void CSpritePropPage::OnPrecise() 
{
	m_View->SetUndoState("Change precise");
	m_View->m_Sprite->m_Precise = m_Precise.GetCheck()!=FALSE;
}


//////////////////////////////////////////////////////////////////////////
void CSpritePropPage::OnContinuos() 
{
	m_View->SetUndoState("Change continuous");
	m_View->m_Sprite->m_Continuous = m_Continuous.GetCheck()!=FALSE;
}


//////////////////////////////////////////////////////////////////////////
void CSpritePropPage::OnMuted() 
{
	m_View->SetUndoState("Change mute");
	m_View->m_Sprite->m_EditorMuted = m_Muted.GetCheck()!=FALSE;
}


//////////////////////////////////////////////////////////////////////////
void CSpritePropPage::OnStreamed() 
{
	m_View->SetUndoState("Change streaming");
	m_View->m_Sprite->m_Streamed = m_Streamed.GetCheck()!=FALSE;
}

//////////////////////////////////////////////////////////////////////////
void CSpritePropPage::OnKeepLoaded() 
{
	m_View->SetUndoState("Change keep loaded");
	m_View->m_Sprite->m_StreamedKeepLoaded = m_KeepLoaded.GetCheck()!=FALSE;
}

//////////////////////////////////////////////////////////////////////////
void CSpritePropPage::OnBnClickedScripts()
{
	if(m_View && m_View->m_Sprite){
		CScriptDlg dlg;
		dlg.m_Object = m_View->m_Sprite;
		dlg.m_View = m_View;
		if(dlg.DoModal()==IDOK) SetState();
	}
}
