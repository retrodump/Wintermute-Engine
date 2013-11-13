// ScriptDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpriteEdit.h"
#include "ScriptDlg.h"
#include "../mfc_shared/utils_mfc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScriptDlg dialog


CScriptDlg::CScriptDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScriptDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScriptDlg)
	//}}AFX_DATA_INIT

	m_Object = NULL;
	m_View = NULL;
}


//////////////////////////////////////////////////////////////////////////
void CScriptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScriptDlg)
	DDX_Control(pDX, IDC_EDIT, m_EditScript);
	DDX_Control(pDX, IDC_SCRIPTS, m_Scripts);
	DDX_Control(pDX, IDC_SCRIPT_UP, m_ScriptUp);
	DDX_Control(pDX, IDC_SCRIPT_DOWN, m_ScriptDown);
	DDX_Control(pDX, IDC_REMOVE_SCRIPT, m_RemoveScript);
	DDX_Control(pDX, IDC_ADD_SCRIPT, m_AddScript);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScriptDlg, CDialog)
	//{{AFX_MSG_MAP(CScriptDlg)
	ON_LBN_SELCHANGE(IDC_SCRIPTS, OnSelchangeScripts)
	ON_BN_CLICKED(IDC_ADD_SCRIPT, OnAddScript)
	ON_BN_CLICKED(IDC_REMOVE_SCRIPT, OnRemoveScript)
	ON_BN_CLICKED(IDC_SCRIPT_UP, OnScriptUp)
	ON_BN_CLICKED(IDC_SCRIPT_DOWN, OnScriptDown)
	ON_BN_DOUBLECLICKED(IDC_SCRIPT_DOWN, OnScriptDown)
	ON_BN_DOUBLECLICKED(IDC_SCRIPT_UP, OnScriptUp)
	ON_BN_CLICKED(IDC_EDIT, OnEditScript)
	ON_LBN_DBLCLK(IDC_SCRIPTS, OnDblclkScripts)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScriptDlg message handlers



//////////////////////////////////////////////////////////////////////////
BOOL CScriptDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetupButton(&m_ScriptUp,         "Script up",         IDB_MOVE_UP);
	SetupButton(&m_ScriptDown,       "Script down",       IDB_MOVE_DOWN);
	SetupButton(&m_AddScript,        "Add script",        IDB_BCGBARRES_NEW);
	SetupButton(&m_EditScript,       "Edit script",       IDB_PROPERTIES);
	SetupButton(&m_RemoveScript,     "Remove script",     IDB_BCGBARRES_DELETE);

	if(m_Object){
		for(int i=0; i<m_Object->m_Scripts.GetSize(); i++){
			m_ScriptNames.Add(CString(m_Object->m_Scripts[i]->m_Filename));
		}

		if(m_Object->m_Name) SetWindowText(CString("Scripts for the \"" + CString(m_Object->m_Name) + "\" object"));
	}

	Redraw();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//////////////////////////////////////////////////////////////////////////
void CScriptDlg::Redraw(int sel)
{
	if(sel<0) sel = m_Scripts.GetCurSel();
	m_Scripts.ResetContent();

	for(int i=0; i<m_ScriptNames.GetSize(); i++) m_Scripts.AddString(LPCSTR(m_ScriptNames[i]));
	if(sel==LB_ERR && m_Scripts.GetCount()>0) m_Scripts.SetCurSel(0);
	else if(sel>=m_Scripts.GetCount()) m_Scripts.SetCurSel(m_Scripts.GetCount()-1);
	else m_Scripts.SetCurSel(sel);

	SetState();
}


//////////////////////////////////////////////////////////////////////////
void CScriptDlg::SetState()
{
	int sel = m_Scripts.GetCurSel();
	
	m_Scripts.EnableWindow(sel != LB_ERR);

	m_ScriptUp.EnableWindow(sel != LB_ERR && sel > 0);
	m_ScriptDown.EnableWindow(sel != LB_ERR && sel < m_Scripts.GetCount()-1);
	m_Scripts.EnableWindow(TRUE);
	m_RemoveScript.EnableWindow(sel != LB_ERR);
	m_EditScript.EnableWindow(sel != LB_ERR);
}


//////////////////////////////////////////////////////////////////////////
void CScriptDlg::OnSelchangeScripts() 
{
	SetState();
}


//////////////////////////////////////////////////////////////////////////
void CScriptDlg::OnOK() 
{
	if(!m_View || !m_Object) return;

	m_View->SetUndoState("Change scripts");
	int i;

	for(i=0; i<m_Object->m_Scripts.GetSize(); i++){
		m_Object->m_Scripts[i]->m_Owner = NULL;
		m_Object->m_Scripts[i]->Finish();
	}
	m_Object->m_Scripts.RemoveAll();
	m_Object->Game->m_ScEngine->EditorCleanup();

	for(i=0; i<m_ScriptNames.GetSize(); i++){
		m_Object->AddScript((char*)LPCSTR(m_ScriptNames[i]));
	}
	
	
	CDialog::OnOK();
}


//////////////////////////////////////////////////////////////////////////
void CScriptDlg::OnAddScript() 
{
	int sel = m_Scripts.GetCurSel();

	CString NewFile = GetRelativeFilename(m_Object->Game, "", "Script Files (*.dcs; *.script)|*.dcs;*.script", ((CSpriteEditApp*)AfxGetApp())->m_LastDirScript, this, true);
	if(NewFile!=""){
		for(int i=0; i<m_ScriptNames.GetSize(); i++){
			if(!m_ScriptNames[i].CompareNoCase(NewFile)){
				MessageBox("This script is already assigned.", NULL, MB_OK|MB_ICONINFORMATION);
				return;
			}
		}
		if(sel==m_Scripts.GetCount()-1) m_ScriptNames.Add(NewFile);
		else m_ScriptNames.InsertAt(sel+1, NewFile);
		Redraw(sel+1);
	}
}


//////////////////////////////////////////////////////////////////////////
void CScriptDlg::OnRemoveScript() 
{
	int sel = m_Scripts.GetCurSel();
	if(sel!=LB_ERR){
		if(MessageBox("Do you really want to remove this script?", NULL, MB_YESNO|MB_ICONQUESTION)!=IDYES) return;
		m_ScriptNames.RemoveAt(sel);
		Redraw(sel);
	}
	else MessageBeep(MB_ICONERROR);
	
}


//////////////////////////////////////////////////////////////////////////
void CScriptDlg::OnScriptUp() 
{
	int sel = m_Scripts.GetCurSel();
	if(sel!=LB_ERR && sel > 0){
		CString str = m_ScriptNames[sel];
		m_ScriptNames[sel] = m_ScriptNames[sel-1];
		m_ScriptNames[sel-1] = str;
		
		Redraw(sel-1);
	}
	else MessageBeep(MB_ICONERROR);
}


//////////////////////////////////////////////////////////////////////////
void CScriptDlg::OnScriptDown() 
{
	int sel = m_Scripts.GetCurSel();
	if(sel!=LB_ERR && sel < m_Scripts.GetCount()-1){
		CString str = m_ScriptNames[sel];
		m_ScriptNames[sel] = m_ScriptNames[sel+1];
		m_ScriptNames[sel+1] = str;
		
		Redraw(sel+1);
	}
	else MessageBeep(MB_ICONERROR);	
}


//////////////////////////////////////////////////////////////////////////
void CScriptDlg::OnEditScript() 
{
	int sel = m_Scripts.GetCurSel();
	if(sel==LB_ERR) return;

	((CSpriteEditApp*)AfxGetApp())->EditFile(m_ScriptNames[sel], m_Object->Game, this);
}


//////////////////////////////////////////////////////////////////////////
void CScriptDlg::OnDblclkScripts() 
{
	OnEditScript();
}
