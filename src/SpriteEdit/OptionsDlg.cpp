// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpriteEdit.h"
#include "OptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog


//////////////////////////////////////////////////////////////////////////
COptionsDlg::COptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionsDlg)
	//}}AFX_DATA_INIT

	Game = ::new CBGame;
	Game->m_EditorMode = true;
	Game->m_DoNotExpandStrings = true;

	Game->m_Registry->SetBasePath(SPRITEEDIT_REG_PATH);	
}


//////////////////////////////////////////////////////////////////////////
COptionsDlg::~COptionsDlg()
{
	delete Game;
}


//////////////////////////////////////////////////////////////////////////
void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDlg)
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_EDIT_INI, m_EditIni);
	DDX_Control(pDX, IDC_ACCELERATION, m_Accelerated);
	DDX_Control(pDX, IDC_FORCE_16BIT_TEXTURES, m_Force16bitTextures);
	DDX_Control(pDX, IDC_FORCE_TEXTURE_SUBDIVISION, m_ForceTextureSubdivision);
	DDX_Control(pDX, IDC_CUSTOM_PATHS, m_CustomPaths);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
	//{{AFX_MSG_MAP(COptionsDlg)
	ON_BN_CLICKED(IDC_EDIT_INI, OnEditIni)
	ON_BN_CLICKED(IDC_ACCELERATION, OnAcceleration)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg message handlers

BOOL COptionsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_CustomPaths.SetStandardButtons();

	m_Accelerated.SetCheck(Game->m_Registry->ReadBool("Options", "Accelerated", true));
	m_Force16bitTextures.SetCheck(Game->m_Registry->ReadBool("Options", "Force16bitTextures", false));
	m_ForceTextureSubdivision.SetCheck(Game->m_Registry->ReadBool("Options", "ForceTextureSubdivision", false));


	char* temp = new char[32768];
	DWORD size, len;
	int i;
	char* str_start;

	// single files paths
	size = 32768;
	Game->m_Registry->ReadString("Resource", "CustomPaths", temp, &size);
	str_start = temp;
	len = strlen(temp);
	for(i=0; i<=len; i++){
		if(temp[i]==';') temp[i]='\0';
		if(temp[i]=='\0'){
			m_CustomPaths.AddItem(str_start);
			str_start = temp + i + 1;
		}
	}


	m_OK.m_nFlatStyle = CBCGButton::BUTTONSTYLE_SEMIFLAT;
	m_Cancel.m_nFlatStyle = CBCGButton::BUTTONSTYLE_SEMIFLAT;
	m_EditIni.m_nFlatStyle = CBCGButton::BUTTONSTYLE_SEMIFLAT;

	m_CustomPaths.EnableWindow(FALSE);
	m_EditIni.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//////////////////////////////////////////////////////////////////////////
void COptionsDlg::OnOK() 
{
	Game->m_Registry->WriteBool("Options", "Accelerated", m_Accelerated.GetCheck()!=FALSE);
	Game->m_Registry->WriteBool("Options", "Force16bitTextures", m_Force16bitTextures.GetCheck()!=FALSE);
	Game->m_Registry->WriteBool("Options", "ForceTextureSubdivision", m_ForceTextureSubdivision.GetCheck()!=FALSE);

	CString CustomPaths;
	for(int i=0; i<m_CustomPaths.GetCount(); i++) CustomPaths = CustomPaths + (i==0?"":";") + m_CustomPaths.GetItemText(i);
	Game->m_Registry->WriteString("Resource", "CustomPaths", (char*)LPCSTR(CustomPaths));
	
	MessageBox("This change will not affect the currently open documents.", "Options", MB_ICONINFORMATION);

	CDialog::OnOK();
}


//////////////////////////////////////////////////////////////////////////
void COptionsDlg::OnEditIni() 
{
	WinExec("notepad.exe \".\\dcgf.ini\"", SW_NORMAL);
}


//////////////////////////////////////////////////////////////////////////
void COptionsDlg::OnAcceleration() 
{
	bool Accel = m_Accelerated.GetCheck()!=0;
	m_Force16bitTextures.EnableWindow(Accel);
	m_ForceTextureSubdivision.EnableWindow(Accel);
}
