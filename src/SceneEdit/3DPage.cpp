// 3DPage.cpp : implementation file
//

#include "stdafx.h"
#include "SceneEdit.h"
#include "3DPage.h"
#include "../mfc_shared/utils_mfc.h"
#include ".\3dpage.h"

// C3DPage dialog

#define FOV_SLIDER_MOD 100.0f

IMPLEMENT_DYNAMIC(C3DPage, CDialog)
C3DPage::C3DPage(CWnd* pParent /*=NULL*/)
	: CDialog(C3DPage::IDD, pParent)
{
	m_View = NULL;
}

//////////////////////////////////////////////////////////////////////////
C3DPage::~C3DPage()
{
}

//////////////////////////////////////////////////////////////////////////
void C3DPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BROWSE_GEOMETRY, m_BrowseGeom);
	DDX_Control(pDX, IDC_SHOW_GEOMETRY, m_ShowGeom);
	DDX_Control(pDX, IDC_GEOMETRY, m_GeomName);
	DDX_Control(pDX, IDC_CAMERA, m_Camera);
	DDX_Control(pDX, IDC_SHADOWS, m_Shadows);
	DDX_Control(pDX, IDC_RES_WIDTH, m_ResWidth);
	DDX_Control(pDX, IDC_RES_HEIGHT, m_ResHeight);
	DDX_Control(pDX, IDC_OVERRIDE_FOV, m_OverrideFOV);
	DDX_Control(pDX, IDC_FOV, m_FOV);
	DDX_Control(pDX, IDC_FOV_VAL, m_FOVVal);
	DDX_Control(pDX, IDC_WAYPOINT_HEIGHT, m_WaypointHeight);	
	DDX_Control(pDX, IDC_NEAR_PLANE, m_NearPlane);
	DDX_Control(pDX, IDC_FAR_PLANE, m_FarPlane);
	DDX_Control(pDX, IDC_COL_AMBIENT, m_AmbientColor);

	m_WaypointHeight.SetMask("#######.##");
	m_WaypointHeight.AllowNegative(false);

	m_FOVVal.SetMask("###.##");
	m_FOVVal.AllowNegative(false);

	m_NearPlane.SetMask("#######.##");
	m_NearPlane.AllowNegative();
	m_FarPlane.SetMask("#######.##");
	m_FarPlane.AllowNegative();
	DDX_Control(pDX, IDC_2D_PATHFINDING, m_2DPathfinding);
	DDX_Control(pDX, IDC_SCROLL3D_COMPATIBILITY, m_Scroll3DCompatibility);
}


BEGIN_MESSAGE_MAP(C3DPage, CDialog)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_CAMERA, OnCameraSel)
	ON_CBN_SELCHANGE(IDC_SHADOWS, OnShadowsSel)
	ON_BN_CLICKED(IDC_BROWSE_GEOMETRY, OnBrowseGeom)
	ON_BN_CLICKED(IDC_SHOW_GEOMETRY, OnShowGeom)
	ON_EN_KILLFOCUS(IDC_RES_WIDTH, OnEnKillfocusResWidth)
	ON_EN_KILLFOCUS(IDC_RES_HEIGHT, OnEnKillfocusResHeight)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_OVERRIDE_FOV, OnBnClickedOverrideFov)
	ON_EN_KILLFOCUS(IDC_WAYPOINT_HEIGHT, OnEnKillfocusWaypointHeight)
	ON_EN_KILLFOCUS(IDC_NEAR_PLANE, OnEnKillfocusNearPlane)
	ON_EN_KILLFOCUS(IDC_FAR_PLANE, OnEnKillfocusFarPlane)
	ON_EN_KILLFOCUS(IDC_FOV_VAL, OnEnKillfocusFovVal)
	ON_BN_CLICKED(IDC_2D_PATHFINDING, On2DPathfinding)
	ON_BN_CLICKED(IDC_SCROLL3D_COMPATIBILITY, OnScroll3DCompatibility)
	ON_BN_CLICKED(IDC_COL_AMBIENT, OnColAmbient)
END_MESSAGE_MAP()


// C3DPage message handlers

//////////////////////////////////////////////////////////////////////////
void C3DPage::SetCurrentView(CSceneView *View)
{
	if(View==m_View) return;

	m_View = View;
	Redraw();
}

//////////////////////////////////////////////////////////////////////////
void C3DPage::OnOK()
{
	SetFOVVal();
	SetResolution();
	SetWaypointHeight();
	SetClipping();
}

//////////////////////////////////////////////////////////////////////////
void C3DPage::OnCancel()
{
	// do nothing
}

//////////////////////////////////////////////////////////////////////////
BOOL C3DPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_FOV.SetRange(0, 180 * FOV_SLIDER_MOD);
	m_FOV.SetTicFreq(45 * FOV_SLIDER_MOD);
	m_FOV.SetBuddy(&m_FOVVal);

	m_AmbientColor.EnableOtherButton("Other");


	SetState();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



//////////////////////////////////////////////////////////////////////////
void C3DPage::Redraw()
{
	if(m_View && m_View->Game->m_Scene)
	{
		m_ShowGeom.SetCheck(m_View->Game->m_Scene->m_ShowGeometry);
		m_2DPathfinding.SetCheck(m_View->Game->m_Scene->m_2DPathfinding);
		m_Scroll3DCompatibility.SetCheck(m_View->Game->m_Scene->m_Scroll3DCompatibility);
		
		CString Str;
		Str.Format("%d", m_View->Game->m_Scene->m_EditorResolutionWidth);
		m_ResWidth.SetWindowText(Str);

		Str.Format("%d", m_View->Game->m_Scene->m_EditorResolutionHeight);
		m_ResHeight.SetWindowText(Str);

		m_AmbientColor.SetColor(CBUtils::D3D2COLORREF(m_View->Game->m_Scene->m_AmbientLightColor));
	}

	if(m_View && m_View->Game->m_Scene && m_View->Game->m_Scene->m_Geom)
	{
		CAdSceneGeometry* Geom = m_View->Game->m_Scene->m_Geom;
		m_GeomName.SetWindowText(Geom->m_Filename);

		m_Camera.ResetContent();
		for(int i=0; i<Geom->m_Cameras.GetSize(); i++)
		{
			m_Camera.AddString(Geom->m_Cameras[i]->m_Name);
		}
		if(Geom->m_ActiveCamera >= 0 && Geom->m_ActiveCamera < Geom->m_Cameras.GetSize())
			m_Camera.SetCurSel(Geom->m_ActiveCamera);

		m_Shadows.ResetContent();
		m_Shadows.AddString("none");
		m_Shadows.AddString("simple");
		m_Shadows.AddString("flat");
		m_Shadows.AddString("stencil");
		m_Shadows.SetCurSel((int)m_View->Game->m_Scene->m_MaxShadowType);


		if(m_View->Game->m_Scene->m_FOV>=0.0f)
		{
			int i = (int)(RadToDeg(m_View->Game->m_Scene->m_FOV) * FOV_SLIDER_MOD);
			m_OverrideFOV.SetCheck(TRUE);
			m_FOV.SetPos((int)(RadToDeg(m_View->Game->m_Scene->m_FOV) * FOV_SLIDER_MOD));
		}
		else
		{
			m_OverrideFOV.SetCheck(FALSE);
			if(Geom->m_ActiveCamera >= 0 && Geom->m_ActiveCamera < Geom->m_Cameras.GetSize())
			{
				m_FOV.SetPos((int)(RadToDeg(Geom->m_Cameras[Geom->m_ActiveCamera]->m_FOV) * FOV_SLIDER_MOD));
			}
			else m_FOV.SetPos(0);
		}
		
		CString Str;
		Str.Format("%f", Geom->m_WaypointHeight);
		m_WaypointHeight.SetWindowText(Str);

		Str.Format("%f", m_View->Game->m_Scene->m_NearClipPlane);
		m_NearPlane.SetWindowText(Str);

		Str.Format("%f", m_View->Game->m_Scene->m_FarClipPlane);
		m_FarPlane.SetWindowText(Str);
	}
	else
	{
		m_GeomName.SetWindowText("");
		m_Camera.ResetContent();
		m_Shadows.ResetContent();
		m_OverrideFOV.SetCheck(FALSE);
		m_FOV.SetPos(0);
		m_WaypointHeight.SetWindowText("");
		m_NearPlane.SetWindowText("");
		m_FarPlane.SetWindowText("");
	}
	SetState();
	SetFOV();
}

//////////////////////////////////////////////////////////////////////////
void C3DPage::SetState()
{
	BOOL GeomSet = m_View && m_View->Game->m_Scene && m_View->Game->m_Scene->m_Geom;

	m_BrowseGeom.EnableWindow(m_View!=NULL);
	m_GeomName.EnableWindow(m_View!=NULL);
	m_Camera.EnableWindow(GeomSet);
	m_Shadows.EnableWindow(GeomSet);
	m_ShowGeom.EnableWindow(GeomSet);
	m_2DPathfinding.EnableWindow(GeomSet);
	m_Scroll3DCompatibility.EnableWindow(GeomSet);
	m_ResWidth.EnableWindow(GeomSet);
	m_ResHeight.EnableWindow(GeomSet);
	m_AmbientColor.EnableWindow(GeomSet);

	m_OverrideFOV.EnableWindow(GeomSet);
	m_FOV.EnableWindow(GeomSet && m_OverrideFOV.GetCheck());
	m_FOVVal.EnableWindow(GeomSet && m_OverrideFOV.GetCheck());
	m_WaypointHeight.EnableWindow(GeomSet);
	m_NearPlane.EnableWindow(GeomSet);
	m_FarPlane.EnableWindow(GeomSet);
}


//////////////////////////////////////////////////////////////////////////
void C3DPage::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(::IsWindow(m_BrowseGeom))
	{
		ScaleFilePair(&m_BrowseGeom, &m_GeomName);
	}
}

//////////////////////////////////////////////////////////////////////////
void C3DPage::ScaleFilePair(CButton *Button, CEdit *Edit)
{
	CRect rcClient, rcButton, rcEdit;
	GetClientRect(&rcClient);
	if(Button) Button->GetWindowRect(&rcButton);
	else rcButton.SetRectEmpty();
	Edit->GetWindowRect(&rcEdit);

	ScreenToClient(&rcButton);
	ScreenToClient(&rcEdit);

	int ButtonX = rcClient.Width() - rcButton.Width() - 5;

	rcEdit.right = ButtonX;
	Edit->SetWindowPos(NULL, -1, -1, rcEdit.Width(), rcEdit.Height(), SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);

	if(Button)
		Button->SetWindowPos(NULL, ButtonX, rcButton.top, -1, -1, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
}


//////////////////////////////////////////////////////////////////////////
void C3DPage::OnCameraSel()
{
	m_View->SetUndoState("Camera change");
	m_View->Game->m_Scene->m_Geom->SetActiveCamera(m_Camera.GetCurSel(), m_OverrideFOV.GetCheck()?(float)m_FOV.GetPos()/FOV_SLIDER_MOD:-1.0f, m_View->Game->m_Scene->m_NearClipPlane, m_View->Game->m_Scene->m_FarClipPlane);
	SetFOV();
}

//////////////////////////////////////////////////////////////////////////
void C3DPage::OnShadowsSel()
{
	m_View->SetUndoState("Shadow type change");
	m_View->Game->m_Scene->SetMaxShadowType((TShadowType)m_Shadows.GetCurSel());
}


//////////////////////////////////////////////////////////////////////////
void C3DPage::OnShowGeom()
{
	m_View->SetUndoState("Geometry display change");
	m_View->Game->m_Scene->m_ShowGeometry = m_ShowGeom.GetCheck()!=FALSE;
}


//////////////////////////////////////////////////////////////////////////
void C3DPage::OnBrowseGeom()
{
	CAdScene* Scene = m_View->Game->m_Scene;

	if(Scene){
		CString OrigFile;
		if(Scene->m_Geom && Scene->m_Geom->m_Filename) OrigFile = CString(Scene->m_Geom->m_Filename);
		CString NewFile = GetRelativeFilename(Scene->Game, OrigFile, "Geometry files (*.3ds)|*.3ds", ((CSceneEditApp*)AfxGetApp())->m_LastDirGeom, this);
		if(NewFile==""){
			if(OrigFile!=""){
				if(MessageBox("Do you want to remove current geometry file?", "Geometry", MB_ICONQUESTION|MB_YESNO)==IDYES){
					m_View->SetUndoState("Remove geometry");
					SAFE_DELETE(Scene->m_Geom);
					Redraw();
				}
			}
		}
		else{
			
			m_View->SetUndoState("Change geometry");
			SAFE_DELETE(Scene->m_Geom);
			Scene->m_Geom = ::new CAdSceneGeometry(Scene->Game);
			if(!Scene->m_Geom || FAILED(Scene->m_Geom->LoadFile((char*)LPCSTR(NewFile)))){
				SAFE_DELETE(Scene->m_Geom);
				Redraw();
				MessageBox("Error loading geometry file '" + NewFile + "'", "Error", MB_ICONERROR);
			}
			else{
				Redraw();
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void C3DPage::SetResolution()
{
	if(m_View)
	{
		int i1 = m_View->Game->m_Scene->m_EditorResolutionWidth;
		int i2 = m_View->Game->m_Scene->m_EditorResolutionHeight;

		UpdateIntEdit(this, &m_ResWidth, &i1);
		UpdateIntEdit(this, &m_ResHeight, &i2);

		if(i1 != m_View->Game->m_Scene->m_EditorResolutionWidth || i2 != m_View->Game->m_Scene->m_EditorResolutionHeight) m_View->SetUndoState("Change game resolution");

		m_View->Game->m_Scene->m_EditorResolutionWidth = i1;
		m_View->Game->m_Scene->m_EditorResolutionHeight = i2;

		m_View->Game->m_EditorResolutionWidth = i1;
		m_View->Game->m_EditorResolutionHeight = i2;
	}
}

//////////////////////////////////////////////////////////////////////////
void C3DPage::OnEnKillfocusResWidth()
{
	SetResolution();
}

//////////////////////////////////////////////////////////////////////////
void C3DPage::OnEnKillfocusResHeight()
{
	SetResolution();
}

//////////////////////////////////////////////////////////////////////////
void C3DPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	if((CSliderCtrl*)pScrollBar==&m_FOV)
	{
		SetFOV();
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);	
}

//////////////////////////////////////////////////////////////////////////
void C3DPage::SetFOV()
{
	float FOV = DegToRad((float)m_FOV.GetPos()/FOV_SLIDER_MOD);
	if(m_View && m_View->Game && m_View->Game->m_Scene && m_View->Game->m_Scene->m_Geom)
	{
		m_View->Game->m_Scene->m_Geom->SetActiveCamera(m_Camera.GetCurSel(), m_OverrideFOV.GetCheck()?FOV:-1.0f, m_View->Game->m_Scene->m_NearClipPlane, m_View->Game->m_Scene->m_FarClipPlane);
		m_View->Game->m_Scene->m_FOV = m_OverrideFOV.GetCheck()?FOV:-1.0f;

		if(!m_OverrideFOV.GetCheck())
		{
			CAdSceneGeometry* Geom = m_View->Game->m_Scene->m_Geom;	
			if(Geom->m_ActiveCamera >= 0 && Geom->m_ActiveCamera < Geom->m_Cameras.GetSize())
			{
				m_FOV.SetPos((int)(RadToDeg(Geom->m_Cameras[Geom->m_ActiveCamera]->m_FOV) * FOV_SLIDER_MOD));
			}
		}
	}
	else m_FOV.SetPos(0);

	CString Str;
	Str.Format("%.02f", (float)m_FOV.GetPos()/FOV_SLIDER_MOD);
	m_FOVVal.SetWindowText(Str);

}


//////////////////////////////////////////////////////////////////////////
void C3DPage::OnBnClickedOverrideFov()
{
	m_View->SetUndoState("FOV override change");
	SetFOV();
	SetState();
}


//////////////////////////////////////////////////////////////////////////
void C3DPage::SetWaypointHeight()
{
	if(m_View && m_View->Game->m_Scene->m_Geom)
	{
		float f = m_View->Game->m_Scene->m_Geom->m_WaypointHeight;

		UpdateFloatEdit(this, &m_WaypointHeight, &f);

		if(f != m_View->Game->m_Scene->m_Geom->m_WaypointHeight) m_View->SetUndoState("Change waypoint height");

		m_View->Game->m_Scene->m_Geom->m_WaypointHeight = f;
		m_View->Game->m_Scene->m_Geom->DropWaypoints();
	}
}

//////////////////////////////////////////////////////////////////////////
void C3DPage::SetClipping()
{
	if(m_View && m_View->Game->m_Scene->m_Geom)
	{
		float f1 = m_View->Game->m_Scene->m_NearClipPlane;
		float f2 = m_View->Game->m_Scene->m_FarClipPlane;

		UpdateFloatEdit(this, &m_NearPlane, &f1);
		UpdateFloatEdit(this, &m_FarPlane, &f2);

		if(f2>0.0f && f1 > f2)
		{
			float Temp = f1;
			f1 = f2;
			f2 = Temp;
		}

		if(f1 != m_View->Game->m_Scene->m_NearClipPlane || f2 != m_View->Game->m_Scene->m_FarClipPlane) m_View->SetUndoState("Change clipping planes");

		m_View->Game->m_Scene->m_NearClipPlane = f1;
		m_View->Game->m_Scene->m_FarClipPlane = f2;
		
		SetFOV();
	}
}

//////////////////////////////////////////////////////////////////////////
void C3DPage::OnEnKillfocusWaypointHeight()
{
	SetWaypointHeight();
}

//////////////////////////////////////////////////////////////////////////
void C3DPage::OnEnKillfocusNearPlane()
{
	SetClipping();
}

//////////////////////////////////////////////////////////////////////////
void C3DPage::OnEnKillfocusFarPlane()
{
	SetClipping();
}


//////////////////////////////////////////////////////////////////////////
void C3DPage::OnEnKillfocusFovVal()
{
	SetFOVVal();
	SetFOV();
}

//////////////////////////////////////////////////////////////////////////
void C3DPage::SetFOVVal()
{
	if(m_View && m_View->Game->m_Scene->m_Geom)
	{
		float f = RadToDeg(m_View->Game->m_Scene->m_FOV);

		UpdateFloatEdit(this, &m_FOVVal, &f);
		m_View->SetUndoState("FOV change");

		m_View->Game->m_Scene->m_FOV = DegToRad(f);
		m_FOV.SetPos((int)(RadToDeg(m_View->Game->m_Scene->m_FOV) * FOV_SLIDER_MOD));
	}
}

//////////////////////////////////////////////////////////////////////////
void C3DPage::On2DPathfinding()
{
	m_View->SetUndoState("Pathfinding change");
	m_View->Game->m_Scene->m_2DPathfinding = m_2DPathfinding.GetCheck()!=FALSE;
}


//////////////////////////////////////////////////////////////////////////
void C3DPage::OnScroll3DCompatibility()
{
	m_View->SetUndoState("Scrolling compatibility changed");
	m_View->Game->m_Scene->m_Scroll3DCompatibility = m_Scroll3DCompatibility.GetCheck()!=FALSE;
}


//////////////////////////////////////////////////////////////////////////
void C3DPage::OnColAmbient() 
{
	if(m_View){
		m_View->SetUndoState("Change ambient color");
		m_View->Game->m_Scene->m_AmbientLightColor = CBUtils::COLORREF2D3D(m_AmbientColor.GetColor());
	}
}
