// ScenePropPage.cpp : implementation file
//

#include "stdafx.h"
#include "SceneEdit.h"
#include "ScenePropPage.h"
#include "../mfc_shared/utils_mfc.h"
#include "EventsDlg.h"
#include "ScriptDlg.h"
#include "PropsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScenePropPage dialog


//////////////////////////////////////////////////////////////////////////
CScenePropPage::CScenePropPage(CWnd* pParent /*=NULL*/)
	: CDialog(CScenePropPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScenePropPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_View = NULL;
}


//////////////////////////////////////////////////////////////////////////
void CScenePropPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScenePropPage)
	DDX_Control(pDX, IDC_PERSISTENT, m_Persistent);
	DDX_Control(pDX, IDC_PERSISTENT_SPRITES, m_PersistentSprites);
	DDX_Control(pDX, IDC_SHOW_DECOR, m_ShowDecor);
	DDX_Control(pDX, IDC_COL_DECOR_SEL, m_DecorSel);
	DDX_Control(pDX, IDC_COL_DECOR, m_Decor);
	DDX_Control(pDX, IDC_CUSTOM, m_Custom);
	DDX_Control(pDX, IDC_SHOW_SCALE, m_ShowScale);
	DDX_Control(pDX, IDC_SHOW_REGIONS, m_ShowRegions);
	DDX_Control(pDX, IDC_SHOW_ENTITIES, m_ShowEntities);
	DDX_Control(pDX, IDC_SHOW_BLOCKED, m_ShowBlocked);
	DDX_Control(pDX, IDC_COL_BLOCKED_SEL, m_BlockedSel);
	DDX_Control(pDX, IDC_COL_BLOCKED, m_Blocked);
	DDX_Control(pDX, IDC_MARGIN_V, m_MarginV);
	DDX_Control(pDX, IDC_MARGIN_H, m_MarginH);
	DDX_Control(pDX, IDC_NAME, m_Name);
	DDX_Control(pDX, IDC_EVENTS, m_Events);
	DDX_Control(pDX, IDC_COL_WAYPOINTS_SEL, m_WaypointsSel);
	DDX_Control(pDX, IDC_COL_WAYPOINTS, m_Waypoints);
	DDX_Control(pDX, IDC_COL_REGION_SEL, m_RegionSel);
	DDX_Control(pDX, IDC_COL_REGION, m_Region);
	DDX_Control(pDX, IDC_COL_ENTITY_SEL, m_EntitySel);
	DDX_Control(pDX, IDC_COL_ENTITY, m_Entity);
	DDX_Control(pDX, IDC_COL_FRAME, m_Frame);
	DDX_Control(pDX, IDC_COL_SCALE, m_Scale);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScenePropPage, CDialog)
	//{{AFX_MSG_MAP(CScenePropPage)
	ON_EN_CHANGE(IDC_NAME, OnChangeName)
	ON_EN_KILLFOCUS(IDC_MARGIN_H, OnKillfocusMarginH)
	ON_EN_KILLFOCUS(IDC_MARGIN_V, OnKillfocusMarginV)
	ON_BN_CLICKED(IDC_COL_BLOCKED, OnColBlocked)
	ON_BN_CLICKED(IDC_COL_BLOCKED_SEL, OnColBlockedSel)
	ON_BN_CLICKED(IDC_COL_ENTITY, OnColEntity)
	ON_BN_CLICKED(IDC_COL_ENTITY_SEL, OnColEntitySel)
	ON_BN_CLICKED(IDC_COL_FRAME, OnColFrame)
	ON_BN_CLICKED(IDC_COL_REGION, OnColRegion)
	ON_BN_CLICKED(IDC_COL_REGION_SEL, OnColRegionSel)
	ON_BN_CLICKED(IDC_COL_WAYPOINTS, OnColWaypoints)
	ON_BN_CLICKED(IDC_COL_WAYPOINTS_SEL, OnColWaypointsSel)
	ON_BN_CLICKED(IDC_EVENTS, OnEvents)
	ON_BN_CLICKED(IDC_COL_SCALE, OnColScale)
	ON_BN_CLICKED(IDC_SHOW_REGIONS, OnShowRegions)
	ON_BN_CLICKED(IDC_SHOW_BLOCKED, OnShowBlocked)
	ON_BN_CLICKED(IDC_SHOW_ENTITIES, OnShowEntities)
	ON_BN_CLICKED(IDC_SHOW_SCALE, OnShowScale)
	ON_BN_CLICKED(IDC_CUSTOM, OnCustom)
	ON_BN_CLICKED(IDC_SHOW_DECOR, OnShowDecor)
	ON_BN_CLICKED(IDC_COL_DECOR, OnColDecor)
	ON_BN_CLICKED(IDC_COL_DECOR_SEL, OnColDecorSel)
	ON_BN_CLICKED(IDC_PERSISTENT, OnPersistent)
	ON_BN_CLICKED(IDC_PERSISTENT_SPRITES, OnPersistentSprites)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScenePropPage message handlers


//////////////////////////////////////////////////////////////////////////
void CScenePropPage::OnCancel() 
{
	// do nothing
}


//////////////////////////////////////////////////////////////////////////
void CScenePropPage::OnOK() 
{
	SetMargins();
}



//////////////////////////////////////////////////////////////////////////
void CScenePropPage::SetCurrentView(CSceneView *View)
{
	if(View==m_View) return;
	
	m_View = View;
	Redraw();
}


//////////////////////////////////////////////////////////////////////////
void CScenePropPage::Redraw()
{
	SetState();

	if(m_View){
		m_Name.SetWindowText(m_View->Game->m_Scene->m_Name);

		m_WaypointsSel.SetColor(CBUtils::D3D2COLORREF(m_View->Game->m_Scene->m_EditorColWaypointsSel));
		m_Waypoints.SetColor(CBUtils::D3D2COLORREF(m_View->Game->m_Scene->m_EditorColWaypoints));
		m_BlockedSel.SetColor(CBUtils::D3D2COLORREF(m_View->Game->m_Scene->m_EditorColBlockedSel));
		m_Blocked.SetColor(CBUtils::D3D2COLORREF(m_View->Game->m_Scene->m_EditorColBlocked));
		m_RegionSel.SetColor(CBUtils::D3D2COLORREF(m_View->Game->m_Scene->m_EditorColRegionSel));
		m_Region.SetColor(CBUtils::D3D2COLORREF(m_View->Game->m_Scene->m_EditorColRegion));
		m_DecorSel.SetColor(CBUtils::D3D2COLORREF(m_View->Game->m_Scene->m_EditorColDecorSel));
		m_Decor.SetColor(CBUtils::D3D2COLORREF(m_View->Game->m_Scene->m_EditorColDecor));
		m_EntitySel.SetColor(CBUtils::D3D2COLORREF(m_View->Game->m_Scene->m_EditorColEntitySel));
		m_Entity.SetColor(CBUtils::D3D2COLORREF(m_View->Game->m_Scene->m_EditorColEntity));
		m_Frame.SetColor(CBUtils::D3D2COLORREF(m_View->Game->m_Scene->m_EditorColFrame));
		m_Scale.SetColor(CBUtils::D3D2COLORREF(m_View->Game->m_Scene->m_EditorColScale));

		m_ShowRegions.SetCheck(m_View->Game->m_Scene->m_EditorShowRegions);
		m_ShowBlocked.SetCheck(m_View->Game->m_Scene->m_EditorShowBlocked);
		m_ShowDecor.SetCheck(m_View->Game->m_Scene->m_EditorShowDecor);
		m_ShowEntities.SetCheck(m_View->Game->m_Scene->m_EditorShowEntities);
		m_ShowScale.SetCheck(m_View->Game->m_Scene->m_EditorShowScale);
		m_Persistent.SetCheck(m_View->Game->m_Scene->m_PersistentState);
		m_PersistentSprites.SetCheck(m_View->Game->m_Scene->m_PersistentStateSprites);

		CString str;
		str.Format("%d", (int)m_View->Game->m_Scene->m_EditorMarginH);
		m_MarginH.SetWindowText(str);
		str.Format("%d", (int)m_View->Game->m_Scene->m_EditorMarginV);
		m_MarginV.SetWindowText(str);
	}
}


//////////////////////////////////////////////////////////////////////////
void CScenePropPage::SetState()
{
	m_WaypointsSel.EnableWindow(m_View!=NULL);
	m_Waypoints.EnableWindow(m_View!=NULL);
	m_BlockedSel.EnableWindow(m_View!=NULL);
	m_Blocked.EnableWindow(m_View!=NULL);
	m_DecorSel.EnableWindow(m_View!=NULL);
	m_Decor.EnableWindow(m_View!=NULL);
	m_RegionSel.EnableWindow(m_View!=NULL);
	m_Region.EnableWindow(m_View!=NULL);
	m_EntitySel.EnableWindow(m_View!=NULL);
	m_Entity.EnableWindow(m_View!=NULL);
	m_Frame.EnableWindow(m_View!=NULL);
	m_Scale.EnableWindow(m_View!=NULL);

	m_ShowRegions.EnableWindow(m_View!=NULL);
	m_ShowBlocked.EnableWindow(m_View!=NULL);
	m_ShowDecor.EnableWindow(m_View!=NULL);
	m_ShowEntities.EnableWindow(m_View!=NULL);
	m_ShowScale.EnableWindow(m_View!=NULL);
	
	m_Persistent.EnableWindow(m_View!=NULL);
	m_PersistentSprites.EnableWindow(m_View!=NULL);

	m_Events.EnableWindow(m_View!=NULL);
	m_Custom.EnableWindow(m_View!=NULL);

	m_MarginH.EnableWindow(m_View!=NULL);
	m_MarginV.EnableWindow(m_View!=NULL);

	m_Name.EnableWindow(m_View!=NULL);


	if(m_View && m_View->Game->m_Scene && m_View->Game->m_Scene->m_Scripts.GetSize()>0)
		m_Events.SetFont(&m_BoldFont);
	else
		m_Events.SetFont(GetFont());


	if(m_View && m_View->Game->m_Scene && m_View->Game->m_Scene->m_ScProp && m_View->Game->m_Scene->m_ScProp->m_ValObject.size()>0)
		m_Custom.SetFont(&m_BoldFont);
	else
		m_Custom.SetFont(GetFont());

}


//////////////////////////////////////////////////////////////////////////
BOOL CScenePropPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_WaypointsSel.EnableOtherButton("Other");
	m_Waypoints.EnableOtherButton("Other");
	m_BlockedSel.EnableOtherButton("Other");
	m_Blocked.EnableOtherButton("Other");
	m_RegionSel.EnableOtherButton("Other");
	m_Region.EnableOtherButton("Other");
	m_EntitySel.EnableOtherButton("Other");
	m_Entity.EnableOtherButton("Other");
	m_Frame.EnableOtherButton("Other");
	m_Scale.EnableOtherButton("Other");

	m_Events.m_nFlatStyle = CBCGButton::BUTTONSTYLE_SEMIFLAT;
	m_Custom.m_nFlatStyle = CBCGButton::BUTTONSTYLE_SEMIFLAT;

	LOGFONT logFont;
	CFont* pFont = GetFont();
	pFont->GetLogFont(&logFont);
	logFont.lfWeight = FW_BOLD;
	m_BoldFont.CreateFontIndirect(&logFont);


	SetState();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//////////////////////////////////////////////////////////////////////////
void CScenePropPage::OnChangeName() 
{
	if(m_View){
		CString str;
		m_Name.GetWindowText(str);
		if(str.Compare(m_View->Game->m_Scene->m_Name)!=0){
			m_View->SetUndoState("Change scene name");
			m_View->Game->m_Scene->SetName((char*)LPCSTR(str));
			//((CMainFrame*)AfxGetMainWnd())->m_wndWorkSpace.Refresh(false);
		}
	}	
}


//////////////////////////////////////////////////////////////////////////
void CScenePropPage::SetMargins()
{
	if(m_View){
		int i1 = m_View->Game->m_Scene->m_EditorMarginH;
		int i2 = m_View->Game->m_Scene->m_EditorMarginV;

		UpdateIntEdit(this, &m_MarginH, &i1);
		UpdateIntEdit(this, &m_MarginV, &i2);

		if(i1 != m_View->Game->m_Scene->m_EditorMarginH || i2 != m_View->Game->m_Scene->m_EditorMarginV) m_View->SetUndoState("Change margins");

		m_View->Game->m_Scene->m_EditorMarginH = i1;
		m_View->Game->m_Scene->m_EditorMarginV = i2;
	}
}


//////////////////////////////////////////////////////////////////////////
void CScenePropPage::OnKillfocusMarginH() 
{
	SetMargins();
}


//////////////////////////////////////////////////////////////////////////
void CScenePropPage::OnKillfocusMarginV() 
{
	SetMargins();
}


//////////////////////////////////////////////////////////////////////////
void CScenePropPage::OnColBlocked() 
{
	if(m_View){
		m_View->SetUndoState("Change color");
		m_View->Game->m_Scene->m_EditorColBlocked = CBUtils::COLORREF2D3D(m_Blocked.GetColor());
	}
}


//////////////////////////////////////////////////////////////////////////
void CScenePropPage::OnColBlockedSel() 
{
	if(m_View){
		m_View->SetUndoState("Change color");
		m_View->Game->m_Scene->m_EditorColBlockedSel = CBUtils::COLORREF2D3D(m_BlockedSel.GetColor());
	}
}


//////////////////////////////////////////////////////////////////////////
void CScenePropPage::OnColEntity() 
{
	if(m_View){
		m_View->SetUndoState("Change color");
		m_View->Game->m_Scene->m_EditorColEntity = CBUtils::COLORREF2D3D(m_Entity.GetColor());
	}
}


//////////////////////////////////////////////////////////////////////////
void CScenePropPage::OnColEntitySel() 
{
	if(m_View){
		m_View->SetUndoState("Change color");
		m_View->Game->m_Scene->m_EditorColEntitySel = CBUtils::COLORREF2D3D(m_EntitySel.GetColor());
	}
}


//////////////////////////////////////////////////////////////////////////
void CScenePropPage::OnColFrame() 
{
	if(m_View){
		m_View->SetUndoState("Change color");
		m_View->Game->m_Scene->m_EditorColFrame = CBUtils::COLORREF2D3D(m_Frame.GetColor());
	}
}


//////////////////////////////////////////////////////////////////////////
void CScenePropPage::OnColScale() 
{
	if(m_View){
		m_View->SetUndoState("Change color");
		m_View->Game->m_Scene->m_EditorColScale = CBUtils::COLORREF2D3D(m_Scale.GetColor());
	}
}


//////////////////////////////////////////////////////////////////////////
void CScenePropPage::OnColRegion() 
{
	if(m_View){
		m_View->SetUndoState("Change color");
		m_View->Game->m_Scene->m_EditorColRegion = CBUtils::COLORREF2D3D(m_Region.GetColor());
	}
}


//////////////////////////////////////////////////////////////////////////
void CScenePropPage::OnColRegionSel() 
{
	if(m_View){
		m_View->SetUndoState("Change color");
		m_View->Game->m_Scene->m_EditorColRegionSel = CBUtils::COLORREF2D3D(m_RegionSel.GetColor());
	}
}


//////////////////////////////////////////////////////////////////////////
void CScenePropPage::OnColWaypoints() 
{
	if(m_View){
		m_View->SetUndoState("Change color");
		m_View->Game->m_Scene->m_EditorColWaypoints = CBUtils::COLORREF2D3D(m_Waypoints.GetColor());
	}
}


//////////////////////////////////////////////////////////////////////////
void CScenePropPage::OnColWaypointsSel() 
{
	if(m_View){
		m_View->SetUndoState("Change color");
		m_View->Game->m_Scene->m_EditorColWaypointsSel = CBUtils::COLORREF2D3D(m_WaypointsSel.GetColor());
	}
}


//////////////////////////////////////////////////////////////////////////
void CScenePropPage::OnEvents() 
{
	if(m_View){
		CScriptDlg dlg;
		dlg.m_Object = m_View->Game->m_Scene;
		dlg.m_View = m_View;
		if(dlg.DoModal()==IDOK) SetState();
	}
}


//////////////////////////////////////////////////////////////////////////
void CScenePropPage::OnShowRegions() 
{
	if(m_View){
		m_View->SetUndoState("Change show regions");
		m_View->Game->m_Scene->m_EditorShowRegions = (m_ShowRegions.GetCheck()!=0);
	}
}


//////////////////////////////////////////////////////////////////////////
void CScenePropPage::OnShowBlocked() 
{
	if(m_View){
		m_View->SetUndoState("Change show blocked");
		m_View->Game->m_Scene->m_EditorShowBlocked = (m_ShowBlocked.GetCheck()!=0);
	}
}


//////////////////////////////////////////////////////////////////////////
void CScenePropPage::OnShowEntities() 
{
	if(m_View){
		m_View->SetUndoState("Change show entities");
		m_View->Game->m_Scene->m_EditorShowEntities = (m_ShowEntities.GetCheck()!=0);
	}
}


//////////////////////////////////////////////////////////////////////////
void CScenePropPage::OnShowScale() 
{
	if(m_View){
		m_View->SetUndoState("Change show scale");
		m_View->Game->m_Scene->m_EditorShowScale = (m_ShowScale.GetCheck()!=0);
	}
}


//////////////////////////////////////////////////////////////////////////
void CScenePropPage::OnCustom() 
{
	if(m_View){
		CPropsDlg dlg;
		dlg.m_Object = m_View->Game->m_Scene;
		dlg.m_View = m_View;
		if(dlg.DoModal()==IDOK) SetState();
	}	
}


//////////////////////////////////////////////////////////////////////////
void CScenePropPage::OnShowDecor() 
{
	if(m_View){
		m_View->SetUndoState("Change show decorations");
		m_View->Game->m_Scene->m_EditorShowDecor = (m_ShowDecor.GetCheck()!=0);
	}
}


//////////////////////////////////////////////////////////////////////////
void CScenePropPage::OnColDecor() 
{
	if(m_View){
		m_View->SetUndoState("Change color");
		m_View->Game->m_Scene->m_EditorColDecor = CBUtils::COLORREF2D3D(m_Decor.GetColor());
	}
}

void CScenePropPage::OnColDecorSel() 
{
	if(m_View){
		m_View->SetUndoState("Change color");
		m_View->Game->m_Scene->m_EditorColDecorSel = CBUtils::COLORREF2D3D(m_DecorSel.GetColor());
	}
}

//////////////////////////////////////////////////////////////////////////
void CScenePropPage::OnPersistent() 
{
	if(m_View){
		m_View->SetUndoState("Change persistent");
		m_View->Game->m_Scene->m_PersistentState = (m_Persistent.GetCheck()!=0);
	}
}

//////////////////////////////////////////////////////////////////////////
void CScenePropPage::OnPersistentSprites() 
{
	if(m_View){
		m_View->SetUndoState("Change persistent sprites");
		m_View->Game->m_Scene->m_PersistentStateSprites = (m_PersistentSprites.GetCheck()!=0);
	}
}
