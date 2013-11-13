// FolderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProjectMan.h"
#include "FolderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFolderDlg dialog


CFolderDlg::CFolderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFolderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFolderDlg)
	m_FolderName = _T("");
	//}}AFX_DATA_INIT
}


void CFolderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFolderDlg)
	DDX_Control(pDX, IDC_FOLDER_NAME, m_Name);
	DDX_Text(pDX, IDC_FOLDER_NAME, m_FolderName);
	DDV_MaxChars(pDX, m_FolderName, 256);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFolderDlg, CDialog)
	//{{AFX_MSG_MAP(CFolderDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFolderDlg message handlers


//////////////////////////////////////////////////////////////////////////
void CFolderDlg::OnOK() 
{
	m_Name.GetWindowText(m_FolderName);
	
	if(m_FolderName.FindOneOf("\\|*?:\"<>/")!=-1){
		MessageBox(LOC("/str0044/This name contains forbidden characters."), NULL, MB_OK|MB_ICONERROR);
		return;
	}

	CFileFind finder;
	if(finder.FindFile(m_BasePath + m_FolderName)){
		MessageBox(LOC("/str0045/A folder or file with this name already exists."), NULL, MB_OK|MB_ICONERROR);
		return;
	}

	CDialog::OnOK();
}


//////////////////////////////////////////////////////////////////////////
BOOL CFolderDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	LOCWIN(this);
	return TRUE;
}
