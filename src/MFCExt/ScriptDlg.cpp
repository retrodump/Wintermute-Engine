// ScriptDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ScriptDlg.h"
#include "DCGFStringTable.h"

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
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_Object = NULL;
	m_View = NULL;
}


//////////////////////////////////////////////////////////////////////////
void CScriptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScriptDlg)
	DDX_Control(pDX, IDC_UP, m_BtnUp);
	DDX_Control(pDX, IDC_SCRIPTS, m_Scripts);
	DDX_Control(pDX, IDC_NEW, m_BtnNew);
	DDX_Control(pDX, IDC_EDIT, m_BtnEdit);
	DDX_Control(pDX, IDC_DOWN, m_BtnDown);
	DDX_Control(pDX, IDC_DELETE, m_BtnDelete);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScriptDlg, CDialog)
	//{{AFX_MSG_MAP(CScriptDlg)
	ON_BN_CLICKED(IDC_DELETE, OnRemoveScript)
	ON_BN_CLICKED(IDC_NEW, OnAddScript)
	ON_BN_CLICKED(IDC_DOWN, OnScriptDown)
	ON_BN_CLICKED(IDC_UP, OnScriptUp)
	ON_BN_CLICKED(IDC_EDIT, OnEditScript)
	ON_LBN_SELCHANGE(IDC_SCRIPTS, OnSelchangeScripts)
	ON_LBN_DBLCLK(IDC_SCRIPTS, OnDblclkScripts)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScriptDlg message handlers


//////////////////////////////////////////////////////////////////////////
BOOL CScriptDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	LOCWIN(this);

	m_BtnUp.SetBitmaps(IDB_UP, RGB(255, 0, 255));
	m_BtnUp.SetTooltipText(LOC("/str1023/Script up"));

	m_BtnDown.SetBitmaps(IDB_DOWN, RGB(255, 0, 255));
	m_BtnDown.SetTooltipText(LOC("/str1024/Script down"));

	m_BtnDelete.SetBitmaps(IDB_DELETE, RGB(255, 0, 255));
	m_BtnDelete.SetTooltipText(LOC("/str1025/Remove script"));

	m_BtnNew.SetBitmaps(IDB_NEW, RGB(255, 0, 255));
	m_BtnNew.SetTooltipText(LOC("/str1026/Add script"));

	m_BtnEdit.SetBitmaps(IDB_PROPS, RGB(255, 0, 255));
	m_BtnEdit.SetTooltipText(LOC("/str1027/Edit script"));



	if(m_Object){
		for(int i=0; i<m_Object->m_Scripts.GetSize(); i++){
			m_ScriptNames.Add(CString(m_Object->m_Scripts[i]->m_Filename));
		}

		if(m_Object->m_Name) SetWindowText(CString(LOC("/str1028/Scripts for the") + CString(" \"") + CString(m_Object->m_Name) + "\" " + LOC("/str1029/object")));
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

	for(int i=0; i<m_ScriptNames.GetSize(); i++)
	{
		m_Scripts.AddString(LPCSTR(m_ScriptNames[i]));
	}
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

	m_BtnUp.EnableWindow(sel != LB_ERR && sel > 0);
	m_BtnDown.EnableWindow(sel != LB_ERR && sel < m_Scripts.GetCount()-1);
	m_Scripts.EnableWindow(TRUE);
	m_BtnDelete.EnableWindow(sel != LB_ERR);
	m_BtnEdit.EnableWindow(sel != LB_ERR);
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

	for(i=0; i<m_ScriptNames.GetSize(); i++) m_Object->AddScript((char*)LPCSTR(m_ScriptNames[i]));
	
	
	CDialog::OnOK();
}


//////////////////////////////////////////////////////////////////////////
void CScriptDlg::OnAddScript() 
{
	int sel = m_Scripts.GetCurSel();

	CString NewFile = GetRelativeFilename(m_Object->Game, "", CString(LOC("/str1030/Script Files")) + CString(" (*.dcs; *.script)|*.dcs;*.script"), ((CDCGFApp*)AfxGetApp())->m_LastDirScript, this, true);
	if(NewFile!=""){
		for(int i=0; i<m_ScriptNames.GetSize(); i++){
			if(!m_ScriptNames[i].CompareNoCase(NewFile)){
				MessageBox(LOC("/str1031/This script is already assigned."), NULL, MB_OK|MB_ICONINFORMATION);
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
		if(MessageBox(LOC("/str1032/Do you really want to remove this script?"), NULL, MB_YESNO|MB_ICONQUESTION)!=IDYES) return;
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

	EditFile(m_ScriptNames[sel], m_View->Game, this);
}


//////////////////////////////////////////////////////////////////////////
void CScriptDlg::OnDblclkScripts() 
{
	OnEditScript();
}
