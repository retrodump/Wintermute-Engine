// GuidDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProjectMan.h"
#include "GuidDlg.h"


// CGuidDlg dialog

IMPLEMENT_DYNAMIC(CGuidDlg, CDialog)

CGuidDlg::CGuidDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGuidDlg::IDD, pParent)
{
	m_Guid = GUID_NULL;
}

CGuidDlg::~CGuidDlg()
{
}

void CGuidDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GUID, m_EditGuid);
	DDX_Control(pDX, IDC_GENERATE, m_BtnGenerate);
	DDX_Control(pDX, IDOK, m_BtnOK);
}


BEGIN_MESSAGE_MAP(CGuidDlg, CDialog)
	ON_BN_CLICKED(IDC_GENERATE, &CGuidDlg::OnGenerate)
	ON_EN_CHANGE(IDC_GUID, &CGuidDlg::OnGuidChange)
END_MESSAGE_MAP()


// CGuidDlg message handlers

//////////////////////////////////////////////////////////////////////////
BOOL CGuidDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	LOCWIN(this);
	DisplayGuid();
	SetState();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
void CGuidDlg::DisplayGuid()
{
	if(m_Guid!=GUID_NULL)
	{
		LPOLESTR  olestr;
		StringFromIID(m_Guid, &olestr);
		m_EditGuid.SetWindowText(CString(olestr));
		CoTaskMemFree(olestr);
	}
	else m_EditGuid.SetWindowText("");
}

//////////////////////////////////////////////////////////////////////////
GUID CGuidDlg::GetGuid()
{
	GUID Ret = GUID_NULL;

	CString GuidStr;
	m_EditGuid.GetWindowText(GuidStr);

	OLECHAR  olestr[50];

	mbstowcs(olestr, LPCSTR(GuidStr), 50);
	if(FAILED(IIDFromString(olestr, &Ret)))
	{
		Ret = GUID_NULL;
	}
	return Ret;
}

//////////////////////////////////////////////////////////////////////////
void CGuidDlg::OnGenerate()
{
	CoCreateGuid(&m_Guid);
	DisplayGuid();
	SetState();
}

//////////////////////////////////////////////////////////////////////////
void CGuidDlg::SetState()
{
	m_BtnOK.EnableWindow(GetGuid() != GUID_NULL);
}


//////////////////////////////////////////////////////////////////////////
void CGuidDlg::OnGuidChange()
{
	SetState();
}

//////////////////////////////////////////////////////////////////////////
void CGuidDlg::OnOK()
{
	if(IDYES==MessageBox(LOC("/str1156/Are you sure you want to change the GUID of your game?"), NULL, MB_YESNO|MB_ICONWARNING))
	{
		m_Guid = GetGuid();
		CDialog::OnOK();
	}
}
