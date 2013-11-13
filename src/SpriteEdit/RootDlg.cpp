// RootDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RootDlg.h"
#include "../mfc_shared/BrowseForFolder.h"
#include "../mfc_shared/utils_mfc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRootDlg dialog


CRootDlg::CRootDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRootDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRootDlg)
	//}}AFX_DATA_INIT
}


void CRootDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRootDlg)
	DDX_Control(pDX, IDOK, m_BtnOK);
	DDX_Control(pDX, IDC_BROWSE, m_BtnBrowse);
	DDX_Control(pDX, IDC_PREVIOUS, m_BtnPrev);
	DDX_Control(pDX, IDC_CUSTOM, m_BtnCustom);
	DDX_Control(pDX, IDC_PREV_LIST, m_PrevList);
	DDX_Control(pDX, IDC_CUSTOM_NAME, m_CustomName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRootDlg, CDialog)
	//{{AFX_MSG_MAP(CRootDlg)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_CUSTOM, OnCustom)
	ON_BN_CLICKED(IDC_PREVIOUS, OnPrevious)
	ON_EN_CHANGE(IDC_CUSTOM_NAME, OnChangeCustomName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRootDlg message handlers


//////////////////////////////////////////////////////////////////////////
void CRootDlg::OnBrowse() 
{
	CBrowseForFolder bf;	
	bf.hWndOwner = this->m_hWnd;
	bf.strTitle = "Select a folder...";
	CString init;
	m_CustomName.GetWindowText(init);
	if(init=="" &&m_Filename!=""){
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		_splitpath(LPCSTR(m_Filename), drive, dir, NULL, NULL);
		init.Format("%s%s", drive, dir);
	}
	bf.strStartupDir = init;

	if (bf.GetFolder(init)){
		m_CustomName.SetWindowText(init);
	}

	SetState();
}


//////////////////////////////////////////////////////////////////////////
void CRootDlg::OnCustom() 
{
	SetState();
	m_CustomName.SetFocus();
}


//////////////////////////////////////////////////////////////////////////
void CRootDlg::OnPrevious() 
{
	SetState();
	m_PrevList.SetFocus();
}


//////////////////////////////////////////////////////////////////////////
void CRootDlg::OnOK() 
{
	CString result;

	if(m_BtnPrev.GetCheck()){
		m_PrevList.GetText(m_PrevList.GetCurSel(), result);
	}
	else{
		m_CustomName.GetWindowText(result);
		if(result==""){
			MessageBeep(MB_ICONERROR);
			return;
		}

		char full[MAX_PATH];
		_fullpath(full, LPCSTR(result), MAX_PATH);
		result = CString(full);

		result.Replace('/', '\\');
		if(result[result.GetLength()-1]!='\\') result += '\\';
		m_CustomName.SetWindowText(result);
	}

	// check whether the filename lies within this folder
	if(m_Filename!=""){
		CString filename = m_Filename;
		filename.Replace('/', '\\');
		if(result!=filename.Left(result.GetLength())){
			MessageBox("Folder '" + result + "' is not a parent folder of file '" + m_Filename + "'.", "Error", MB_ICONERROR);
			return;
		}
	}


	DWORD attr = GetFileAttributes(LPCSTR(result));
	if(attr == 0xFFFFFFFF || (attr & FILE_ATTRIBUTE_DIRECTORY) == 0){
		if(MessageBox("Folder '" + result + "' doesn't exist.\nDo you want to create it?", NULL, MB_ICONQUESTION|MB_OKCANCEL)!=IDOK) return;

		result = MakePath(result);
		if(result==""){
			MessageBox("The specified folder cannot be created.", "Error", MB_ICONERROR);
			return;
		}
	}
/*
	CFileFind finder;
	if(!finder.FindFile(result + "startup.settings")){
		if(MessageBox("This doesn't appear to be a valid root folder (the 'startup.settings' file was not found).\n\nDo you want to continue anyway?", "Warning", MB_ICONWARNING|MB_YESNO)!=IDYES) return;
	}
*/
	CFileFind finder;
	if(!finder.FindFile(result + "*.wpr")){
		if(MessageBox("This doesn't appear to be a valid root folder (no project file was found).\n\nDo you want to continue anyway?", "Warning", MB_ICONWARNING|MB_YESNO)!=IDYES) return;
	}
	
	m_SelectedFolder = result;

	
	CDialog::OnOK();
}


//////////////////////////////////////////////////////////////////////////
void CRootDlg::SetState()
{
	m_CustomName.EnableWindow(m_BtnCustom.GetCheck());
	m_BtnBrowse.EnableWindow(m_BtnCustom.GetCheck());

	m_PrevList.EnableWindow(!m_BtnCustom.GetCheck());

	CString str;
	m_CustomName.GetWindowText(str);
	m_BtnOK.EnableWindow(m_BtnPrev.GetCheck() || str != "");
}


//////////////////////////////////////////////////////////////////////////
BOOL CRootDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(m_PrevFolders.GetSize()<1){
		m_BtnPrev.EnableWindow(FALSE);
		m_BtnCustom.SetCheck(TRUE);
		m_BtnPrev.SetCheck(FALSE);
	}
	else{
		for(int i=0; i<m_PrevFolders.GetSize(); i++) m_PrevList.AddString(m_PrevFolders[i]);
		m_PrevList.SetCurSel(0);
		m_BtnPrev.SetCheck(TRUE);
		m_BtnCustom.SetCheck(FALSE);
	}

	if(m_SelectedFolder!=""){
		m_CustomName.SetWindowText(m_SelectedFolder);
		m_BtnCustom.SetCheck(TRUE);
		m_BtnPrev.SetCheck(FALSE);
	}

	SetState();

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
void CRootDlg::AddPrevFolder(CString Folder)
{
	if(Folder=="") return;

	Folder.Replace('/', '\\');

	if(Folder[Folder.GetLength()-1]!='\\') Folder+='\\';

	for(int i=0; i<m_PrevFolders.GetSize(); i++){
		if(m_PrevFolders[i]==Folder) return;
	}
	m_PrevFolders.Add(Folder);
}


//////////////////////////////////////////////////////////////////////////
void CRootDlg::OnChangeCustomName() 
{
	SetState();
}
