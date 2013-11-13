// SaveLoadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProjectMan.h"
#include "SaveLoadDlg.h"
#include ".\saveloaddlg.h"
#include "../MFCExt/utils_mfc.h"


// CSaveLoadDlg dialog

IMPLEMENT_DYNAMIC(CSaveLoadDlg, CDialog)
CSaveLoadDlg::CSaveLoadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveLoadDlg::IDD, pParent)
	, m_LoadX(0)
	, m_LoadY(0)
	, m_SaveX(0)
	, m_SaveY(0)
	, m_LoadFile(_T(""))
	, m_SaveFile(_T(""))
{
}

CSaveLoadDlg::~CSaveLoadDlg()
{
}

void CSaveLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LOAD_X, m_LoadX);
	DDX_Text(pDX, IDC_LOAD_Y, m_LoadY);
	DDX_Text(pDX, IDC_SAVE_X, m_SaveX);
	DDX_Text(pDX, IDC_SAVE_Y, m_SaveY);
	DDX_Text(pDX, IDC_LOAD_FILE, m_LoadFile);
	DDX_Text(pDX, IDC_SAVE_FILE, m_SaveFile);
	DDX_Control(pDX, IDC_LOAD_FILE, m_LoadFileControl);
	DDX_Control(pDX, IDC_SAVE_FILE, m_SaveFileControl);
}


BEGIN_MESSAGE_MAP(CSaveLoadDlg, CDialog)
	ON_BN_CLICKED(IDC_LOAD_BROWSE, OnBnClickedLoadBrowse)
	ON_BN_CLICKED(IDC_SAVE_BROWSE, OnBnClickedSaveBrowse)
END_MESSAGE_MAP()


// CSaveLoadDlg message handlers

//////////////////////////////////////////////////////////////////////////
BOOL CSaveLoadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	LOCWIN(this);

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////
void CSaveLoadDlg::OnBnClickedLoadBrowse()
{
	CString OrigFile;
	m_LoadFileControl.GetWindowText(OrigFile);
	CString NewFile = GetRelativeFilename(Game, OrigFile, CString(LOC("/str1112/Image files")) + " (*.bmp; *.tga; *.png; *.jpg)|*.bmp;*.tga;*.png;*.jpg", CString(""), this);
	if(NewFile!=""){
		m_LoadFileControl.SetWindowText(NewFile);
	}
}

//////////////////////////////////////////////////////////////////////////
void CSaveLoadDlg::OnBnClickedSaveBrowse()
{
	CString OrigFile;
	m_SaveFileControl.GetWindowText(OrigFile);
	CString NewFile = GetRelativeFilename(Game, OrigFile, CString(LOC("/str1112/Image files")) + " (*.bmp; *.tga; *.png; *.jpg)|*.bmp;*.tga;*.png;*.jpg", CString(""), this);
	if(NewFile!=""){
		m_SaveFileControl.SetWindowText(NewFile);
	}
}
