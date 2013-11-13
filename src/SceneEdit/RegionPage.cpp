// RegionPage.cpp : implementation file
//

#include "stdafx.h"
#include "SceneEdit.h"
#include "SceneView.h"
#include "RegionPage.h"
#include "MainFrm.h"
#include "ScriptDlg.h"
#include "PropsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegionPage dialog


//////////////////////////////////////////////////////////////////////////
CRegionPage::CRegionPage(CWnd* pParent /*=NULL*/)
	: CDialog(CRegionPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegionPage)
	//}}AFX_DATA_INIT

	m_Region = NULL;
	m_View = NULL;
}


//////////////////////////////////////////////////////////////////////////
void CRegionPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegionPage)
	DDX_Control(pDX, IDC_CUSTOM, m_Custom);
	DDX_Control(pDX, IDC_EVENTS, m_Events);
	DDX_Control(pDX, IDC_SCALE_SPIN, m_ScaleSpin);
	DDX_Control(pDX, IDC_SCALE, m_Scale);
	DDX_Control(pDX, IDC_NAME, m_Name);
	DDX_Control(pDX, IDC_TYPE_BLOCKED, m_TypeBlocked);
	DDX_Control(pDX, IDC_TYPE_LAYOUT, m_TypeLayout);
	DDX_Control(pDX, IDC_TYPE_DECORATION, m_TypeDecoration);
	DDX_Control(pDX, IDC_ALPHA, m_Alpha);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegionPage, CDialog)
	//{{AFX_MSG_MAP(CRegionPage)
	ON_WM_CREATE()
	ON_EN_CHANGE(IDC_NAME, OnChangeName)
	ON_EN_CHANGE(IDC_SCALE, OnChangeScale)
	ON_BN_CLICKED(IDC_ALPHA, OnAlpha)
	ON_BN_CLICKED(IDC_EVENTS, OnEvents)
	ON_BN_CLICKED(IDC_CUSTOM, OnCustom)
	ON_BN_CLICKED(IDC_TYPE_BLOCKED, OnTypeBlocked)
	ON_BN_CLICKED(IDC_TYPE_DECORATION, OnTypeDecoration)
	ON_BN_CLICKED(IDC_TYPE_LAYOUT, OnTypeLayout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegionPage message handlers


//////////////////////////////////////////////////////////////////////////
int CRegionPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
		
	return 0;
}

//////////////////////////////////////////////////////////////////////////
BOOL CRegionPage::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_Events.m_nFlatStyle = CBCGButton::BUTTONSTYLE_SEMIFLAT;
	m_Custom.m_nFlatStyle = CBCGButton::BUTTONSTYLE_SEMIFLAT;

	m_Alpha.EnableOtherButton("Other");
	m_ScaleSpin.SetRange(0, 1000);
	
	LOGFONT logFont;
	CFont* pFont = GetFont();
	pFont->GetLogFont(&logFont);
	logFont.lfWeight = FW_BOLD;
	m_BoldFont.CreateFontIndirect(&logFont);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//////////////////////////////////////////////////////////////////////////
void CRegionPage::ShowRegion(CAdRegion *Region, CSceneView* View, bool Force)
{
	if(Region==m_Region && !Force) return;
	else{
		m_Region = Region;
		m_View = View;
		if(m_Region){
			m_Name.SetWindowText(m_Region->m_Name);
			CString str;
			str.Format("%d", (int)m_Region->m_Zoom);
			m_Scale.SetWindowText(str);
			m_Alpha.SetColor(CBUtils::D3D2COLORREF(m_Region->m_Alpha));
			
			//m_Blocked.SetCheck(m_Region->m_Blocked);
			if(m_Region->m_Blocked){
				m_TypeBlocked.SetCheck(true);
				m_TypeDecoration.SetCheck(false);
				m_TypeLayout.SetCheck(false);
			}
			else if(m_Region->m_Decoration){
				m_TypeBlocked.SetCheck(false);
				m_TypeDecoration.SetCheck(true);
				m_TypeLayout.SetCheck(false);
			}
			else{
				m_TypeBlocked.SetCheck(false);
				m_TypeDecoration.SetCheck(false);
				m_TypeLayout.SetCheck(true);
			}
			
			SetState();
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CRegionPage::OnChangeName() 
{
	if(m_Region){
		CString str;
		m_Name.GetWindowText(str);
		if(str.Compare(m_Region->m_Name)!=0){
			m_View->SetUndoState("Change name");
			m_Region->SetName((char*)LPCSTR(str));
			((CMainFrame*)AfxGetMainWnd())->m_wndWorkSpace.Refresh(false);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CRegionPage::OnCancel() 
{
	// do nothing
}


//////////////////////////////////////////////////////////////////////////
void CRegionPage::OnOK() 
{
	// do nothing
}


//////////////////////////////////////////////////////////////////////////
void CRegionPage::OnChangeScale() 
{
	if(m_Region){
		float NewScale;
		CString str;
		m_Scale.GetWindowText(str);
		NewScale = atof(LPCSTR(str));
		NewScale = min(NewScale, 1000);
		NewScale = max(NewScale, 0);

		if(NewScale != m_Region->m_Zoom){
			m_View->SetUndoState("Change scale");
			m_Region->m_Zoom = NewScale;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CRegionPage::OnAlpha() 
{
	if(m_Region){
		m_View->SetUndoState("Change color");
		m_Region->m_Alpha = CBUtils::COLORREF2D3D(m_Alpha.GetColor());
	}
}


//////////////////////////////////////////////////////////////////////////
void CRegionPage::OnEvents() 
{
	if(m_Region && m_View){
		CScriptDlg dlg;
		dlg.m_Object = m_Region;
		dlg.m_View = m_View;
		if(dlg.DoModal()==IDOK) SetState();
	}
}


//////////////////////////////////////////////////////////////////////////
void CRegionPage::OnCustom() 
{
	if(m_View && m_Region){
		CPropsDlg dlg;
		dlg.m_Object = m_Region;
		dlg.m_View = m_View;
		if(dlg.DoModal()==IDOK) SetState();
	}
}


//////////////////////////////////////////////////////////////////////////
void CRegionPage::SetState()
{
	if(!m_Region) return;

	m_Scale.EnableWindow(m_Region && !m_Region->m_Blocked);
	m_Alpha.EnableWindow(m_Region && !m_Region->m_Blocked);
	m_ScaleSpin.EnableWindow(m_Region && !m_Region->m_Blocked);


	if(m_Region->m_Scripts.GetSize()>0)
		m_Events.SetFont(&m_BoldFont);
	else
		m_Events.SetFont(GetFont());


	if(m_Region->m_ScProp && m_Region->m_ScProp->m_ValObject.size()>0)
		m_Custom.SetFont(&m_BoldFont);
	else
		m_Custom.SetFont(GetFont());
}


//////////////////////////////////////////////////////////////////////////
void CRegionPage::OnTypeBlocked() 
{
	if(m_Region){
		m_View->SetUndoState("Change blocked");
		m_Region->m_Blocked = true;
		m_Region->m_Decoration = false;
		SetState();
		((CMainFrame*)AfxGetMainWnd())->m_wndWorkSpace.Refresh(false);
	}
}


//////////////////////////////////////////////////////////////////////////
void CRegionPage::OnTypeDecoration() 
{
	if(m_Region){
		m_View->SetUndoState("Change decoration");
		m_Region->m_Blocked = false;
		m_Region->m_Decoration = true;
		SetState();
		((CMainFrame*)AfxGetMainWnd())->m_wndWorkSpace.Refresh(false);
	}
}


//////////////////////////////////////////////////////////////////////////
void CRegionPage::OnTypeLayout() 
{
	if(m_Region){
		m_View->SetUndoState("Change layout");
		m_Region->m_Blocked = false;
		m_Region->m_Decoration = false;
		SetState();
		((CMainFrame*)AfxGetMainWnd())->m_wndWorkSpace.Refresh(false);
	}
}
