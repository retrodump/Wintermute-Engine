// PageVista.cpp : implementation file
//

#include "stdafx.h"
#include "ProjectMan.h"
#include "PageVista.h"
#include <Shlwapi.h>

// CPageVista dialog

IMPLEMENT_DYNAMIC(CPageVista, CPropertyPage)

CPageVista::CPageVista()
	: CPropertyPage(CPageVista::IDD)
	, m_AddGeData(FALSE)
	, m_GdfFile("")
	, m_ThumbnailFile("")
{
	m_ProjectRoot = "";
}

CPageVista::~CPageVista()
{
}

void CPageVista::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GE_DATA, m_CheckAddGeData);
	DDX_Control(pDX, IDC_GDF_FILE, m_EdGdfFile);
	DDX_Control(pDX, IDC_THUMBNAIL_FILE, m_EdThumbnailFile);
	DDX_Control(pDX, IDC_BROWSE_GDF, m_BtnBrowseGdf);
	DDX_Control(pDX, IDC_BROWSE_THUMBNAIL, m_BtnBrowseThumbnail);
	DDX_Check(pDX, IDC_GE_DATA, m_AddGeData);
	DDX_Text(pDX, IDC_GDF_FILE, m_GdfFile);
	DDX_Text(pDX, IDC_THUMBNAIL_FILE, m_ThumbnailFile);

}


BEGIN_MESSAGE_MAP(CPageVista, CPropertyPage)
	ON_BN_CLICKED(IDC_BROWSE_GDF, &CPageVista::OnBnClickedBrowseGdf)
	ON_BN_CLICKED(IDC_BROWSE_THUMBNAIL, &CPageVista::OnBnClickedBrowseThumbnail)
	ON_BN_CLICKED(IDC_GE_DATA, &CPageVista::OnBnClickedGeData)
END_MESSAGE_MAP()


// CPageVista message handlers

//////////////////////////////////////////////////////////////////////////
BOOL CPageVista::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	LOCWIN(this);

	SetState();

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
void CPageVista::SetState()
{
	m_EdGdfFile.EnableWindow(m_CheckAddGeData.GetCheck());
	m_BtnBrowseGdf.EnableWindow(m_CheckAddGeData.GetCheck());
	m_EdThumbnailFile.EnableWindow(m_CheckAddGeData.GetCheck());
	m_BtnBrowseThumbnail.EnableWindow(m_CheckAddGeData.GetCheck());
}

//////////////////////////////////////////////////////////////////////////
void CPageVista::OnBnClickedBrowseGdf()
{
	CString Path;
	m_EdGdfFile.GetWindowText(Path);

	if(PathIsRelative(Path))
	{
		char Temp[MAX_PATH];
		if(m_ProjectRoot!="" && PathCanonicalize(Temp, m_ProjectRoot + Path)) Path = CString(Temp);
	}

	CFileFind finder;
	if(!finder.FindFile(Path)) Path = "";


	DWORD Flags = OFN_HIDEREADONLY|OFN_ENABLESIZING|OFN_NOCHANGEDIR|OFN_FILEMUSTEXIST;
	CString Filter = LOC("/str1167/Game definition files") +  CString(" (*.xml; *.gdf)|*.xml;*.gdf|") + LOC("/str1003/All Files") + " (*.*)|*.*||";

	CFileDialog dlg(TRUE, NULL, Path, Flags, Filter, this);

	if(dlg.DoModal()==IDOK)
	{
		char RelPath[MAX_PATH];
		if(m_ProjectRoot=="" || !PathRelativePathTo(RelPath, m_ProjectRoot, FILE_ATTRIBUTE_NORMAL, dlg.GetPathName(), FILE_ATTRIBUTE_NORMAL))
		{
			strcpy(RelPath, dlg.GetPathName());
		}		
		m_EdGdfFile.SetWindowText(RelPath);
	}
}

//////////////////////////////////////////////////////////////////////////
void CPageVista::OnBnClickedBrowseThumbnail()
{
	CString Path;
	m_EdThumbnailFile.GetWindowText(Path);

	if(PathIsRelative(Path))
	{
		char Temp[MAX_PATH];
		if(m_ProjectRoot!="" && PathCanonicalize(Temp, m_ProjectRoot + Path)) Path = CString(Temp);
	}

	CFileFind finder;
	if(!finder.FindFile(Path)) Path = "";


	DWORD Flags = OFN_HIDEREADONLY|OFN_ENABLESIZING|OFN_NOCHANGEDIR|OFN_FILEMUSTEXIST;
	CString Filter = LOC("/str1112/Image files") +  CString(" (*.png)|*.png|") + LOC("/str1003/All Files") + " (*.*)|*.*||";

	CFileDialog dlg(TRUE, NULL, Path, Flags, Filter, this);

	if(dlg.DoModal()==IDOK)
	{
		char RelPath[MAX_PATH];
		if(m_ProjectRoot=="" || !PathRelativePathTo(RelPath, m_ProjectRoot, FILE_ATTRIBUTE_NORMAL, dlg.GetPathName(), FILE_ATTRIBUTE_NORMAL))
		{
			strcpy(RelPath, dlg.GetPathName());
		}		
		m_EdThumbnailFile.SetWindowText(RelPath);
	}
}

//////////////////////////////////////////////////////////////////////////
void CPageVista::OnBnClickedGeData()
{
	SetState();
}
