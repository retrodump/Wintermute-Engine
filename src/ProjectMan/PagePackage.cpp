// PagePackage.cpp : implementation file
//

#include "stdafx.h"
#include "ProjectMan.h"
#include "PagePackage.h"
#include "../MFCExt/BrowseForFolder.h"
#include <Shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPagePackage property page

IMPLEMENT_DYNCREATE(CPagePackage, CPropertyPage)

CPagePackage::CPagePackage() : CPropertyPage(CPagePackage::IDD)
, m_AddCrashLib(FALSE)
, m_AddFpsDisplay(FALSE)
, m_AddDebugMode(FALSE)
{
	//{{AFX_DATA_INIT(CPagePackage)
	m_OutputDir = _T("");
	m_CopyExe = FALSE;
	m_CopySet = FALSE;
	m_CopyD3DX = FALSE;
	m_ChangeIcon = FALSE;
	m_ChangeIconSet = FALSE;
	m_ExeName = _T("");
	m_SetName = _T("");
	m_IconName = _T("");
	m_IconNameSet = _T("");
	m_BindPackage = FALSE;
	//}}AFX_DATA_INIT

	m_ProjectRoot = "";
}

CPagePackage::~CPagePackage()
{
}

void CPagePackage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPagePackage)
	DDX_Control(pDX, IDC_PACKAGES, m_Packages);
	DDX_Control(pDX, IDC_BIND_PACKAGE, m_ButtonBindPackage);
	DDX_Control(pDX, IDC_BROWSE_ICON, m_ButtonBrowseIcon);
	DDX_Control(pDX, IDC_BROWSE_ICON_SET, m_ButtonBrowseIconSet);
	DDX_Control(pDX, IDC_ICON_NAME, m_EditIconName);
	DDX_Control(pDX, IDC_ICON_NAME_SET, m_EditIconNameSet);
	DDX_Control(pDX, IDC_EXE_NAME, m_EditExeName);
	DDX_Control(pDX, IDC_SET_NAME, m_EditSetName);
	DDX_Control(pDX, IDC_CHANGE_ICON, m_CheckChangeIcon);
	DDX_Control(pDX, IDC_CHANGE_ICON_SET, m_CheckChangeIconSet);
	DDX_Control(pDX, IDC_COPY_EXE, m_CheckCopyExe);
	DDX_Control(pDX, IDC_COPY_SET, m_CheckCopySet);
	DDX_Control(pDX, IDC_COPY_D3DX, m_CheckCopyD3DX);
	DDX_Control(pDX, IDC_OUTPUT_DIR, m_EdOutput);
	DDX_Text(pDX, IDC_OUTPUT_DIR, m_OutputDir);
	DDX_Check(pDX, IDC_COPY_EXE, m_CopyExe);
	DDX_Check(pDX, IDC_COPY_SET, m_CopySet);
	DDX_Check(pDX, IDC_COPY_D3DX, m_CopyD3DX);
	DDX_Check(pDX, IDC_CHANGE_ICON, m_ChangeIcon);
	DDX_Check(pDX, IDC_CHANGE_ICON_SET, m_ChangeIconSet);
	DDX_Text(pDX, IDC_EXE_NAME, m_ExeName);
	DDX_Text(pDX, IDC_SET_NAME, m_SetName);
	DDX_Text(pDX, IDC_ICON_NAME, m_IconName);
	DDX_Text(pDX, IDC_ICON_NAME_SET, m_IconNameSet);
	DDX_Check(pDX, IDC_BIND_PACKAGE, m_BindPackage);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_ADD_CRASH_LIB, m_AddCrashLib);
	DDX_Check(pDX, IDC_ADD_FPS, m_AddFpsDisplay);
	DDX_Check(pDX, IDC_ADD_DEBUG_MODE, m_AddDebugMode);
}


BEGIN_MESSAGE_MAP(CPagePackage, CPropertyPage)
	//{{AFX_MSG_MAP(CPagePackage)
	ON_BN_CLICKED(IDC_BROWSE_OUTPUT, OnBrowseOutput)
	ON_BN_CLICKED(IDC_BROWSE_ICON, OnBrowseIcon)
	ON_BN_CLICKED(IDC_BROWSE_ICON_SET, OnBrowseIconSet)
	ON_BN_CLICKED(IDC_BIND_PACKAGE, OnBindPackage)
	ON_BN_CLICKED(IDC_COPY_EXE, OnStateChange)
	ON_BN_CLICKED(IDC_COPY_SET, OnStateChange)
	ON_BN_CLICKED(IDC_CHANGE_ICON, OnStateChange)
	ON_BN_CLICKED(IDC_CHANGE_ICON_SET, OnStateChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPagePackage message handlers



//////////////////////////////////////////////////////////////////////////
BOOL CPagePackage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	LOCWIN(this);

	int i;

	m_Packages.Clear();
	for(i=0; i<m_PackageNames.GetSize(); i++){
		m_Packages.AddString(m_PackageNames[i]);
		if(m_PackageNames[i]==m_SelectedPackage) m_Packages.SetCurSel(i);
	}
	if(m_Packages.GetCurSel()==CB_ERR) m_Packages.SetCurSel(0);


	SetState();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
void CPagePackage::OnStateChange() 
{
	SetState();
}


//////////////////////////////////////////////////////////////////////////
void CPagePackage::SetState()
{
	m_EditExeName.EnableWindow(m_CheckCopyExe.GetCheck());
	m_CheckChangeIcon.EnableWindow(m_CheckCopyExe.GetCheck());
	m_EditIconName.EnableWindow(m_CheckCopyExe.GetCheck() && m_CheckChangeIcon.GetCheck());
	m_ButtonBrowseIcon.EnableWindow(m_CheckCopyExe.GetCheck() && m_CheckChangeIcon.GetCheck());

	m_ButtonBindPackage.EnableWindow(m_CheckCopyExe.GetCheck());
	m_Packages.EnableWindow(m_ButtonBindPackage.GetCheck() && m_CheckCopyExe.GetCheck());

	m_EditSetName.EnableWindow(m_CheckCopySet.GetCheck());
	m_CheckChangeIconSet.EnableWindow(m_CheckCopySet.GetCheck());
	m_EditIconNameSet.EnableWindow(m_CheckCopySet.GetCheck() && m_CheckChangeIconSet.GetCheck());
	m_ButtonBrowseIconSet.EnableWindow(m_CheckCopySet.GetCheck() && m_CheckChangeIconSet.GetCheck());

}

//////////////////////////////////////////////////////////////////////////
void CPagePackage::OnBrowseOutput() 
{
	CBrowseForFolder bf;	
	bf.hWndOwner = this->GetSafeHwnd();
	bf.strTitle = LOC("/str0103/Select a folder to put the compiled packages to...");

	CString Path;
	m_EdOutput.GetWindowText(Path);

	if(PathIsRelative(Path)){
		char Temp[MAX_PATH];
		if(m_ProjectRoot!="" && PathCanonicalize(Temp, m_ProjectRoot + Path)) Path = CString(Temp);
	}

	bf.strStartupDir = Path;

	if (bf.GetFolder(Path)){
		
		char RelPath[MAX_PATH];
		if(m_ProjectRoot=="" || !PathRelativePathTo(RelPath, m_ProjectRoot, FILE_ATTRIBUTE_NORMAL, Path, FILE_ATTRIBUTE_NORMAL)){
			strcpy(RelPath, Path);
		}		
		m_EdOutput.SetWindowText(RelPath);
	}
}

//////////////////////////////////////////////////////////////////////////
void CPagePackage::OnBrowseIcon() 
{
	OnBrowseIcon(m_EditIconName);
}

//////////////////////////////////////////////////////////////////////////
void CPagePackage::OnBrowseIconSet() 
{
	OnBrowseIcon(m_EditIconNameSet);
}

//////////////////////////////////////////////////////////////////////////
void CPagePackage::OnBrowseIcon(CEdit& EditName)
{
	CString Path;
	EditName.GetWindowText(Path);

	if(PathIsRelative(Path)){
		char Temp[MAX_PATH];
		if(m_ProjectRoot!="" && PathCanonicalize(Temp, m_ProjectRoot + Path)) Path = CString(Temp);
	}

	CFileFind finder;
	if(!finder.FindFile(Path)) Path = "";


	DWORD Flags = OFN_HIDEREADONLY|OFN_ENABLESIZING|OFN_NOCHANGEDIR|OFN_FILEMUSTEXIST;
	CString Filter = LOC("/str0179/Icon files") +  CString(" (*.ico)|*.ico|") + LOC("/str1003/All Files") + " (*.*)|*.*||";

	CFileDialog dlg(TRUE, NULL, Path, Flags, Filter, this);
	
	if(dlg.DoModal()==IDOK){
		char RelPath[MAX_PATH];
		if(m_ProjectRoot=="" || !PathRelativePathTo(RelPath, m_ProjectRoot, FILE_ATTRIBUTE_NORMAL, dlg.GetPathName(), FILE_ATTRIBUTE_NORMAL)){
			strcpy(RelPath, dlg.GetPathName());
		}		
		EditName.SetWindowText(RelPath);
	}
}


//////////////////////////////////////////////////////////////////////////
void CPagePackage::OnBindPackage() 
{
	SetState();
}


//////////////////////////////////////////////////////////////////////////
void CPagePackage::AddPackage(CString Name)
{
	m_PackageNames.Add(Name);
}


//////////////////////////////////////////////////////////////////////////
void CPagePackage::OnOK() 
{
	if(m_ButtonBindPackage.GetCheck() && m_Packages.GetCurSel()!=CB_ERR) m_SelectedPackage = m_PackageNames[m_Packages.GetCurSel()];
	else m_SelectedPackage = "";
	
	CPropertyPage::OnOK();
}
