// pagegeneral.cpp : implementation file
//

#include "stdafx.h"
#include "pagegeneral.h"
#include "BrowseForFolder.h"
#include "DCGFStringTable.h"
#include "utils_mfc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPageGeneral property page

IMPLEMENT_DYNCREATE(CPageGeneral, CPropertyPage)

CPageGeneral::CPageGeneral() : CPropertyPage(CPageGeneral::IDD)
{
	//{{AFX_DATA_INIT(CPageGeneral)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

//////////////////////////////////////////////////////////////////////////
CPageGeneral::~CPageGeneral()
{
}


//////////////////////////////////////////////////////////////////////////
void CPageGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageGeneral)
	DDX_Control(pDX, IDC_CHECK_UPDATES, m_CheckUpdates);
	DDX_Control(pDX, IDC_SHOW_HIDDEN, m_ShowHiddenFiles);
	DDX_Control(pDX, IDC_SETTINGS_DLG, m_SettingsDlg);
	DDX_Control(pDX, IDC_FORCE_TEXTURE_SUBDIVISION, m_TextureSubdivision);
	DDX_Control(pDX, IDC_FORCE_16BIT_TEXTURES, m_16bitTextures);
	DDX_Control(pDX, IDC_WME_PATH, m_WMEPath);
	DDX_Control(pDX, IDC_REPOSITORIES, m_Repositories);
	DDX_Control(pDX, IDC_ACCLERATION, m_Accleration);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(CPageGeneral)
	ON_BN_CLICKED(IDC_ADD_REPOSITORY, OnAddRepository)
	ON_BN_CLICKED(IDC_REMOVE_REPOSITORY, OnRemoveRepository)
	ON_BN_CLICKED(IDC_BROWSE_TOOLS, OnBrowseTools)
	ON_BN_CLICKED(IDC_ACCLERATION, OnAccleration)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageGeneral message handlers

BOOL CPageGeneral::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	LOCWIN(this);
	
	for(int i=0; i<10; i++){
		CString RepKey;
		RepKey.Format("RepositoryPath%d", i+1);
		
		CString Path = GetRegString(HKEY_CURRENT_USER, DCGF_TOOLS_REG_PATH, RepKey, "");
		if(Path!="") AddRepositoryPath(Path);
	}

	if(GetRegInt(HKEY_CURRENT_USER, DCGF_TOOLS_REG_PATH, "UseAcceleration", 1)) m_Accleration.SetCheck(TRUE);

	if(GetRegInt(HKEY_CURRENT_USER, DCGF_TOOLS_REG_PATH, "Force16bitTextures", 0)) m_16bitTextures.SetCheck(TRUE);
	if(GetRegInt(HKEY_CURRENT_USER, DCGF_TOOLS_REG_PATH, "ForceTextureSubdivision", 0)) m_TextureSubdivision.SetCheck(TRUE);
	
	if(GetRegInt(HKEY_CURRENT_USER, DCGF_TOOLS_REG_PATH, "AutoCheckUpdates", 1)) m_CheckUpdates.SetCheck(TRUE);

	if(GetRegInt(HKEY_CURRENT_USER, DCGF_TOOLS_REG_PATH, "ShowSettingsDlg", 1)) m_SettingsDlg.SetCheck(TRUE);
	if(GetRegInt(HKEY_CURRENT_USER, DCGF_TOOLS_REG_PATH, "ShowHiddenFiles", 0)) m_ShowHiddenFiles.SetCheck(TRUE);


	CString Str = GetRegString(HKEY_CURRENT_USER, DCGF_TOOLS_REG_PATH, "ToolsPath");
	m_WMEPath.SetWindowText(Str);

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
void CPageGeneral::OnOK() 
{
	int i;

	for(i=0; i<10; i++){
		CString RepKey;
		RepKey.Format("RepositoryPath%d", i+1);
		DelRegKey(HKEY_CURRENT_USER, DCGF_TOOLS_REG_PATH, RepKey);
	}

	CString Str;

	for(i=0; i<m_Repositories.GetCount(); i++){
		CString RepKey;
		RepKey.Format("RepositoryPath%d", i+1);
		m_Repositories.GetText(i, Str);
		SetRegString(HKEY_CURRENT_USER, DCGF_TOOLS_REG_PATH, RepKey, Str);
	}

	SetRegInt(HKEY_CURRENT_USER, DCGF_TOOLS_REG_PATH, "UseAcceleration",         m_Accleration.GetCheck());
	SetRegInt(HKEY_CURRENT_USER, DCGF_TOOLS_REG_PATH, "Force16bitTextures",      m_16bitTextures.GetCheck());
	SetRegInt(HKEY_CURRENT_USER, DCGF_TOOLS_REG_PATH, "ForceTextureSubdivision", m_TextureSubdivision.GetCheck());
	SetRegInt(HKEY_CURRENT_USER, DCGF_TOOLS_REG_PATH, "AutoCheckUpdates",        m_CheckUpdates.GetCheck());
	SetRegInt(HKEY_CURRENT_USER, DCGF_TOOLS_REG_PATH, "ShowSettingsDlg",         m_SettingsDlg.GetCheck());
	SetRegInt(HKEY_CURRENT_USER, DCGF_TOOLS_REG_PATH, "ShowHiddenFiles",         m_ShowHiddenFiles.GetCheck());

	m_WMEPath.GetWindowText(Str);
	SetRegString(HKEY_CURRENT_USER, DCGF_TOOLS_REG_PATH, "ToolsPath", Str);

	
	CPropertyPage::OnOK();
}


//////////////////////////////////////////////////////////////////////////
void CPageGeneral::OnAddRepository() 
{
	if(m_Repositories.GetCount()>=10){
		MessageBox(LOC("/str1015/Sorry, you can't have more than 10 repository paths."), NULL, MB_OK|MB_ICONINFORMATION);
		return;
	}

	CBrowseForFolder bf;	
	bf.hWndOwner = this->GetSafeHwnd();
	bf.strTitle = LOC("/str1016/Select a folder...");

	CString Dir="";
	int i = m_Repositories.GetCurSel();
	if(i!=LB_ERR) m_Repositories.GetText(i, Dir);
	
	bf.strStartupDir = Dir;

	if(bf.GetFolder(Dir)) AddRepositoryPath(Dir, true);
}


//////////////////////////////////////////////////////////////////////////
void CPageGeneral::OnRemoveRepository() 
{
	int i = m_Repositories.GetCurSel();
	if(i==LB_ERR){
		MessageBeep(MB_OK);
		return;
	}

	m_Repositories.DeleteString(i);
	while(i>=m_Repositories.GetCount()) i--;

	if(i>=0) m_Repositories.SetCurSel(i);
}


//////////////////////////////////////////////////////////////////////////
void CPageGeneral::OnBrowseTools() 
{
	CString Filter = CString(LOC("/str1017/WME runtime")) + " (wme.exe)|wme.exe|" + LOC("/str1003/All Files") + " (*.*)|*.*||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_ENABLESIZING|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_NOCHANGEDIR, Filter, this);
	CString Dir;
	m_WMEPath.GetWindowText(Dir);
	dlg.m_ofn.lpstrInitialDir = Dir;
	
	if(dlg.DoModal()==IDOK) m_WMEPath.SetWindowText(GetPath(dlg.GetPathName()));
}


//////////////////////////////////////////////////////////////////////////
void CPageGeneral::AddRepositoryPath(CString Path, bool Select)
{
	if(Path=="") return;
	if(Path[Path.GetLength()-1]!='\\') Path+= "\\";

	for(int i=0; i<m_Repositories.GetCount(); i++){
		CString str;
		m_Repositories.GetText(i, str);
		if(Path.CompareNoCase(str)==0){
			if(Select) m_Repositories.SetCurSel(i);
			return;
		}
	}
	m_Repositories.AddString(Path);
	if(Select) m_Repositories.SetCurSel(m_Repositories.GetCount()-1);
}



//////////////////////////////////////////////////////////////////////////
void CPageGeneral::OnAccleration() 
{
	bool Accel = (m_Accleration.GetCheck()!=0);
	m_16bitTextures.EnableWindow(Accel);
	m_TextureSubdivision.EnableWindow(Accel);
}
