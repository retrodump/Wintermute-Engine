// EventPropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SceneEdit.h"
#include "EventPropDlg.h"
#include "../mfc_shared/utils_mfc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEventPropDlg dialog


//////////////////////////////////////////////////////////////////////////
CEventPropDlg::CEventPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEventPropDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEventPropDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


//////////////////////////////////////////////////////////////////////////
CEventPropDlg::~CEventPropDlg()
{
	m_Supported.RemoveAll();
}


//////////////////////////////////////////////////////////////////////////
void CEventPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventPropDlg)
	DDX_Control(pDX, IDC_SCRIPT, m_Script);
	DDX_Control(pDX, IDC_EVENT, m_Event);
	DDX_Control(pDX, IDC_BROWSE_SCRIPT, m_BrovseScript);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEventPropDlg, CDialog)
	//{{AFX_MSG_MAP(CEventPropDlg)
	ON_BN_CLICKED(IDC_BROWSE_SCRIPT, OnBrowseScript)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEventPropDlg message handlers


//////////////////////////////////////////////////////////////////////////
BOOL CEventPropDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	for(int i=0; i<m_Supported.GetSize(); i++){
		int index = m_Event.AddString(CBEvent::GetEventName(m_Supported[i]));
		if(index!=CB_ERR){
			m_Event.SetItemData(index, (DWORD)m_Supported[i]);
		}
	}

	if(m_EventType==EVENT_NONE) m_Event.SetCurSel(0);
	else m_Event.SelectString(-1, CBEvent::GetEventName(m_EventType));

	m_Script.SetWindowText(m_ScriptFile);
	
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
void CEventPropDlg::OnBrowseScript() 
{
	CString OrigFile = m_ScriptFile;
	CString NewFile = GetRelativeFilename(m_Game, OrigFile, "Script Files (*.dcs; *.script)|*.dcs;*.script", ((CSceneEditApp*)AfxGetApp())->m_LastDirScript, this);
	if(NewFile!=""){
		m_ScriptFile = NewFile;
		m_Script.SetWindowText(NewFile);
	}
}


//////////////////////////////////////////////////////////////////////////
void CEventPropDlg::OnOK() 
{
	m_EventType = EVENT_NONE;

	int index = m_Event.GetCurSel();
	if(index!=CB_ERR){
		m_EventType = (TEventType)m_Event.GetItemData(index);
	}

	if(m_EventType==EVENT_NONE){
		MessageBox("Please select an event.", NULL, MB_OK|MB_ICONERROR);
		return;
	}

	if(m_ScriptFile==""){
		MessageBox("Please select a script file.", NULL, MB_OK|MB_ICONERROR);
		return;
	}

	CDialog::OnOK();
}
