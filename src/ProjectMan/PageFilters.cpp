// PageFilters.cpp : implementation file
//

#include "stdafx.h"
#include "ProjectMan.h"
#include "PageFilters.h"


// CPageFilters dialog

IMPLEMENT_DYNAMIC(CPageFilters, CPropertyPage)

CPageFilters::CPageFilters()
	: CPropertyPage(CPageFilters::IDD)
{
	m_ActiveExclude = FALSE;
	m_ActiveScript = FALSE;
	m_ActiveUncompress = FALSE;
	m_ActiveCopy = FALSE;
	m_MasksExclude = _T("");
	m_MasksScript = _T("");
	m_MasksUncompress = _T("");
	m_MasksCopy = _T("");
}

CPageFilters::~CPageFilters()
{
}

void CPageFilters::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageFilters)
	DDX_Control(pDX, IDC_MASKS_UNCOMPRESS, m_EdMasksUncompress);
	DDX_Control(pDX, IDC_MASKS_SCRIPT, m_EdMasksScript);
	DDX_Control(pDX, IDC_MASKS_EXCLUDE, m_EdMasksExclude);
	DDX_Control(pDX, IDC_MASKS_COPY, m_EdMasksCopy);
	DDX_Control(pDX, IDC_FILTER_UNCOMPRESS, m_CheckUncompress);
	DDX_Control(pDX, IDC_FILTER_SCRIPT, m_CheckScript);
	DDX_Control(pDX, IDC_FILTER_EXCLUDE, m_CheckExclude);
	DDX_Control(pDX, IDC_FILTER_COPY, m_CheckCopy);
	DDX_Check(pDX, IDC_FILTER_EXCLUDE, m_ActiveExclude);
	DDX_Check(pDX, IDC_FILTER_SCRIPT, m_ActiveScript);
	DDX_Check(pDX, IDC_FILTER_UNCOMPRESS, m_ActiveUncompress);
	DDX_Check(pDX, IDC_FILTER_COPY, m_ActiveCopy);
	DDX_Text(pDX, IDC_MASKS_EXCLUDE, m_MasksExclude);
	DDX_Text(pDX, IDC_MASKS_SCRIPT, m_MasksScript);
	DDX_Text(pDX, IDC_MASKS_UNCOMPRESS, m_MasksUncompress);
	DDX_Text(pDX, IDC_MASKS_COPY, m_MasksCopy);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageFilters, CPropertyPage)
	//{{AFX_MSG_MAP(CPagePackage)
	ON_BN_CLICKED(IDC_FILTER_EXCLUDE, OnFilterActivate)
	ON_BN_CLICKED(IDC_FILTER_SCRIPT, OnFilterActivate)
	ON_BN_CLICKED(IDC_FILTER_UNCOMPRESS, OnFilterActivate)
	ON_BN_CLICKED(IDC_FILTER_COPY, OnFilterActivate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CPageFilters message handlers

//////////////////////////////////////////////////////////////////////////
BOOL CPageFilters::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	LOCWIN(this);

	SetState();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
void CPageFilters::SetState()
{
	m_EdMasksExclude.EnableWindow(m_CheckExclude.GetCheck());
	m_EdMasksScript.EnableWindow(m_CheckScript.GetCheck());
	m_EdMasksUncompress.EnableWindow(m_CheckUncompress.GetCheck());
	m_EdMasksCopy.EnableWindow(m_CheckCopy.GetCheck());
}

//////////////////////////////////////////////////////////////////////////
void CPageFilters::OnFilterActivate() 
{
	SetState();
}
