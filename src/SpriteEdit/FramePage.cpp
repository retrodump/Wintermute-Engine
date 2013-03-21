// FramePage.cpp : implementation file
//

#include "stdafx.h"
#include "SpriteEdit.h"
#include "FramePage.h"
#include "MainFrm.h"
#include "../mfc_shared/utils_mfc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFramePage dialog


CFramePage::CFramePage(CWnd* pParent /*=NULL*/)
	: CDialog(CFramePage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFramePage)
	//}}AFX_DATA_INIT

	m_View = NULL;
	m_Frame = NULL;
	m_Subframe = NULL;
}


void CFramePage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFramePage)
	DDX_Control(pDX, IDC_TOTAL_TIME, m_TotalTime);
	DDX_Control(pDX, IDC_COPY_ALPHA, m_CopyAlpha);
	DDX_Control(pDX, IDC_COPY_MIRROR, m_CopyMirror);
	DDX_Control(pDX, IDC_COPY_TRANS, m_CopyTrans);
	DDX_Control(pDX, IDC_KILL_SOUND, m_KillSound);
	DDX_Control(pDX, IDC_KEYFRAME, m_Keyframe);
	DDX_Control(pDX, IDC_DECORATION, m_Decoration);
	DDX_Control(pDX, IDC_EVENT, m_Event);
	DDX_Control(pDX, IDC_SOUND, m_Sound);
	DDX_Control(pDX, IDC_BROWSE_SOUND, m_BrowseSound);
	DDX_Control(pDX, IDC_ALPHA, m_Alpha);
	DDX_Control(pDX, IDC_TRANSPARENT, m_Transparent);
	DDX_Control(pDX, IDC_TRANS_RGB, m_TransRGB);
	DDX_Control(pDX, IDC_MOVE_Y, m_MoveY);
	DDX_Control(pDX, IDC_MOVE_X, m_MoveX);
	DDX_Control(pDX, IDC_IMAGE, m_Image);
	DDX_Control(pDX, IDC_HOTSPOT_Y, m_HotspotY);
	DDX_Control(pDX, IDC_HOTSPOT_X, m_HotspotX);
	DDX_Control(pDX, IDC_DELAY, m_Delay);
	DDX_Control(pDX, IDC_COPY_MOVE, m_CopyMove);
	DDX_Control(pDX, IDC_COPY_HOTSPOT, m_CopyHotspot);
	DDX_Control(pDX, IDC_COPY_DELAY, m_CopyDelay);
	DDX_Control(pDX, IDC_BROWSE_IMAGE, m_BrowseImage);
	DDX_Control(pDX, IDC_ALPHA_SPIN, m_AlphaSpin);
	DDX_Control(pDX, IDC_ALPHA_RGB, m_AlphaRGB);
	DDX_Control(pDX, IDC_ALPHA_COLOR, m_AlphaColor);
	DDX_Control(pDX, IDC_3D_ONLY, m_3DOnly);
	DDX_Control(pDX, IDC_2D_ONLY, m_2DOnly);
	DDX_Control(pDX, IDC_MIRROR_X, m_MirrorX);
	DDX_Control(pDX, IDC_MIRROR_Y, m_MirrorY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFramePage, CDialog)
	//{{AFX_MSG_MAP(CFramePage)
	ON_WM_SIZE()
	ON_EN_KILLFOCUS(IDC_DELAY, OnLeaveNum)
	ON_EN_CHANGE(IDC_ALPHA, OnChangeAlpha)
	ON_BN_CLICKED(IDC_TRANSPARENT, OnTransparent)
	ON_BN_CLICKED(IDC_ALPHA_COLOR, OnAlphaColor)
	ON_BN_CLICKED(IDC_2D_ONLY, On2dOnly)
	ON_BN_CLICKED(IDC_3D_ONLY, On3dOnly)
	ON_BN_CLICKED(IDC_MIRROR_X, OnMirrorX)
	ON_BN_CLICKED(IDC_MIRROR_Y, OnMirrorY)
	ON_BN_CLICKED(IDC_BROWSE_IMAGE, OnBrowseImage)
	ON_BN_CLICKED(IDC_COPY_DELAY, OnCopyDelay)
	ON_BN_CLICKED(IDC_COPY_HOTSPOT, OnCopyHotspot)
	ON_BN_CLICKED(IDC_COPY_MOVE, OnCopyMove)
	ON_BN_CLICKED(IDC_BROWSE_SOUND, OnBrowseSound)
	ON_BN_CLICKED(IDC_KEYFRAME, OnKeyframe)
	ON_BN_CLICKED(IDC_DECORATION, OnDecoration)
	ON_BN_CLICKED(IDC_KILL_SOUND, OnKillSound)
	ON_BN_CLICKED(IDC_COPY_TRANS, OnCopyTrans)
	ON_EN_KILLFOCUS(IDC_HOTSPOT_X, OnLeaveNum)
	ON_EN_KILLFOCUS(IDC_HOTSPOT_Y, OnLeaveNum)
	ON_EN_KILLFOCUS(IDC_MOVE_X, OnLeaveNum)
	ON_EN_KILLFOCUS(IDC_MOVE_Y, OnLeaveNum)
	ON_EN_KILLFOCUS(IDC_EVENT, OnLeaveNum)
	ON_BN_CLICKED(IDC_COPY_ALPHA, OnCopyAlpha)
	ON_BN_CLICKED(IDC_COPY_MIRROR, OnCopyMirror)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFramePage message handlers


//////////////////////////////////////////////////////////////////////////
void CFramePage::OnOK() 
{
	SetPos();
}


//////////////////////////////////////////////////////////////////////////
void CFramePage::OnCancel() 
{
	// do nothing
}


//////////////////////////////////////////////////////////////////////////
void CFramePage::ShowFrame(CSpriteView* View, CBFrame* Frame, CBSubFrame* Subframe, bool Force){
	if(Frame==m_Frame && Subframe==m_Subframe && !Force) return;
	else{
		m_Frame = Frame;
		m_Subframe = Subframe;
		m_View = View;
		if(m_Frame && m_Subframe){
			CString str;

			str.Format("%d", Frame->m_Delay);
			m_Delay.SetWindowText(str);


			if(Frame->m_ApplyEvent.GetSize()>0) m_Event.SetWindowText(Frame->m_ApplyEvent[0]);
			else m_Event.SetWindowText("");

			str.Format("%d", Frame->m_MoveX);
			m_MoveX.SetWindowText(str);

			str.Format("%d", Frame->m_MoveY);
			m_MoveY.SetWindowText(str);

			m_Keyframe.SetCheck(Frame->m_Keyframe);

			m_KillSound.SetCheck(Frame->m_KillSound);

			m_Decoration.SetCheck(Subframe->m_Decoration);

			m_3DOnly.SetCheck(Subframe->m_3DOnly);
			m_2DOnly.SetCheck(Subframe->m_2DOnly);

			m_MirrorX.SetCheck(Subframe->m_MirrorX);
			m_MirrorY.SetCheck(Subframe->m_MirrorY);

			str.Format("%d", Subframe->m_HotspotX);
			m_HotspotX.SetWindowText(str);

			str.Format("%d", Subframe->m_HotspotY);
			m_HotspotY.SetWindowText(str);

			if(CBUtils::D3D2COLORREF(Subframe->m_Alpha) != m_AlphaColor.GetColor())
				m_AlphaColor.SetColor(CBUtils::D3D2COLORREF(Subframe->m_Alpha));

			if(CBUtils::D3D2COLORREF(Subframe->m_Transparent) != m_Transparent.GetColor())
				m_Transparent.SetColor(CBUtils::D3D2COLORREF(Subframe->m_Transparent));
			
			str.Format("%d", D3DCOLGetA(Subframe->m_Alpha));
			m_Alpha.SetWindowText(str);

			if(Subframe->m_Surface && Subframe->m_Surface->m_Filename)
				m_Image.SetWindowText(Subframe->m_Surface->m_Filename);
			else
				m_Image.SetWindowText("");

			if(Frame->m_Sound && Frame->m_Sound->m_SoundFilename)
				m_Sound.SetWindowText(Frame->m_Sound->m_SoundFilename);
			else
				m_Sound.SetWindowText("");
		}
		ShowRGB();

		SetState();
		ShowTotalTime();
	}
}


//////////////////////////////////////////////////////////////////////////
void CFramePage::SetState()
{
	if(!m_Frame || !m_Subframe) return;

	EnableWindow(m_View && !m_View->m_Playing);
}


//////////////////////////////////////////////////////////////////////////
BOOL CFramePage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_AlphaColor.EnableOtherButton("Other");
	m_Transparent.EnableOtherButton("Other");
	m_AlphaSpin.SetRange(0, 255);

	m_BrowseImage.m_nFlatStyle = CBCGButton::BUTTONSTYLE_SEMIFLAT;
	m_BrowseImage.SetTooltip("Browse image");

	m_BrowseSound.m_nFlatStyle = CBCGButton::BUTTONSTYLE_SEMIFLAT;
	m_BrowseSound.SetTooltip("Browse sound");

	SetupButton(&m_CopyDelay, "Apply to all frames", IDB_COPY_ALL);
	SetupButton(&m_CopyHotspot, "Apply to all frames", IDB_COPY_ALL);
	SetupButton(&m_CopyMove, "Apply to all frames", IDB_COPY_ALL);
	SetupButton(&m_CopyTrans, "Apply to all frames", IDB_COPY_ALL);
	SetupButton(&m_CopyAlpha, "Apply to all frames", IDB_COPY_ALL);
	SetupButton(&m_CopyMirror, "Apply to all frames", IDB_COPY_ALL);
	
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
void CFramePage::ScalePair(CButton *Button, CEdit *Edit)
{
	CRect rcClient, rcButton, rcEdit;
	GetClientRect(&rcClient);
	Button->GetWindowRect(&rcButton);
	Edit->GetWindowRect(&rcEdit);

	ScreenToClient(&rcButton);
	ScreenToClient(&rcEdit);

	int ButtonX = rcClient.Width() - rcButton.Width() - 5;

	rcEdit.right = ButtonX;
	Edit->SetWindowPos(NULL, -1, -1, rcEdit.Width(), rcEdit.Height(), SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);

	Button->SetWindowPos(NULL, ButtonX, rcButton.top, -1, -1, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
}


//////////////////////////////////////////////////////////////////////////
void CFramePage::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if(::IsWindow(m_BrowseImage)){
		ScalePair(&m_BrowseImage, &m_Image);
		ScalePair(&m_BrowseSound, &m_Sound);
	}
}


//////////////////////////////////////////////////////////////////////////
void CFramePage::ShowRGB()
{
	CString str;
	str.Format("(%d,%d,%d)", GetRValue(m_AlphaColor.GetColor()), GetGValue(m_AlphaColor.GetColor()), GetBValue(m_AlphaColor.GetColor()));
	m_AlphaRGB.SetWindowText(str);

	str.Format("(%d,%d,%d)", GetRValue(m_Transparent.GetColor()), GetGValue(m_Transparent.GetColor()), GetBValue(m_Transparent.GetColor()));
	m_TransRGB.SetWindowText(str);
}



//////////////////////////////////////////////////////////////////////////
void CFramePage::SetPos()
{
	int i1, i2;

	if(m_Frame){
		i1 = m_Frame->m_MoveX;
		i2 = m_Frame->m_MoveY;

		UpdateIntEdit(this, &m_MoveX, &i1);
		UpdateIntEdit(this, &m_MoveY, &i2);

		if(i1 != m_Frame->m_MoveX || i2 != m_Frame->m_MoveY) m_View->SetUndoState("Change move");

		m_Frame->m_MoveX = i1;
		m_Frame->m_MoveY = i2;

	
		i1 = m_Frame->m_Delay;
		UpdateIntEdit(this, &m_Delay, &i1);
		if(i1 != m_Frame->m_Delay) m_View->SetUndoState("Change delay");
		m_Frame->m_Delay = i1;


		// update event
		CString event;
		m_Event.GetWindowText(event);
		if((m_Frame->m_ApplyEvent.GetSize()==0 && event!="") || (m_Frame->m_ApplyEvent.GetSize()>0 && CString(m_Frame->m_ApplyEvent[0])!=event)){
			m_View->SetUndoState("Change event");
			if(event==""){
				SAFE_DELETE_ARRAY(m_Frame->m_ApplyEvent[0]);
				m_Frame->m_ApplyEvent.RemoveAt(0);
			}
			else{
				if(m_Frame->m_ApplyEvent.GetSize()>0){
					SAFE_DELETE_ARRAY(m_Frame->m_ApplyEvent[0]);
					m_Frame->m_ApplyEvent.RemoveAt(0);
				}
				char* str = new char[event.GetLength()+1];
				strcpy(str, LPCSTR(event));
				m_Frame->m_ApplyEvent.InsertAt(0, str);
			}
		}
	}

	if(m_Subframe){
		i1 = m_Subframe->m_HotspotX;
		i2 = m_Subframe->m_HotspotY;

		UpdateIntEdit(this, &m_HotspotX, &i1);
		UpdateIntEdit(this, &m_HotspotY, &i2);

		if(i1 != m_Subframe->m_HotspotX || i2 != m_Subframe->m_HotspotY) m_View->SetUndoState("Change hotspot");

		m_Subframe->m_HotspotX = i1;
		m_Subframe->m_HotspotY = i2;
	}
	ShowTotalTime();
	
}


//////////////////////////////////////////////////////////////////////////
void CFramePage::OnLeaveNum() 
{
	SetPos();
}


//////////////////////////////////////////////////////////////////////////
void CFramePage::OnChangeAlpha() 
{
	if(m_Subframe){
		int NewVal;
		CString str;
		m_Alpha.GetWindowText(str);
		NewVal = atoi(LPCSTR(str));
		NewVal = min(NewVal, 255);
		NewVal = max(NewVal, 0);

		if(NewVal != D3DCOLGetA(m_Subframe->m_Alpha)){
			m_View->SetUndoState("Change opacity");
			m_Subframe->m_Alpha = DRGBA(D3DCOLGetR(m_Subframe->m_Alpha), D3DCOLGetG(m_Subframe->m_Alpha), D3DCOLGetB(m_Subframe->m_Alpha), NewVal);
		}
	}	
}


//////////////////////////////////////////////////////////////////////////
void CFramePage::OnTransparent() 
{
	if(m_Subframe){
		if(m_Subframe->m_Transparent!=CBUtils::COLORREF2D3D(m_Transparent.GetColor())){
			m_View->SetUndoState("Change transparent color");

			m_Subframe->m_Transparent = CBUtils::COLORREF2D3D(m_Transparent.GetColor());
			ShowRGB();

			// force graphics to reload
			m_View->SetUndoState("Temporary");
			m_View->Undo();
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CFramePage::OnAlphaColor() 
{
	if(m_Subframe){
		int r = GetRValue(m_AlphaColor.GetColor());
		int g = GetGValue(m_AlphaColor.GetColor());
		int b = GetBValue(m_AlphaColor.GetColor());
		int a = D3DCOLGetA(m_Subframe->m_Alpha);

		m_View->SetUndoState("Change alpha color");
		m_Subframe->m_Alpha = DRGBA(r, g, b, a);

		ShowRGB();
	}
}


//////////////////////////////////////////////////////////////////////////
void CFramePage::On2dOnly() 
{
	if(m_Subframe){
		m_View->SetUndoState("Change 2D only");
		m_Subframe->m_2DOnly = m_2DOnly.GetCheck() != FALSE;

		if(m_Subframe->m_2DOnly){
			m_Subframe->m_3DOnly = false;
			m_3DOnly.SetCheck(FALSE);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CFramePage::On3dOnly() 
{
	if(m_Subframe){
		m_View->SetUndoState("Change 3D only");
		m_Subframe->m_3DOnly = m_3DOnly.GetCheck() != FALSE;

		if(m_Subframe->m_3DOnly){
			m_Subframe->m_2DOnly = false;
			m_2DOnly.SetCheck(FALSE);
		}	
	}
}

//////////////////////////////////////////////////////////////////////////
void CFramePage::OnMirrorX() 
{
	if(m_Subframe){
		m_View->SetUndoState("Change mirror X");
		m_Subframe->m_MirrorX = m_MirrorX.GetCheck() != FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
void CFramePage::OnMirrorY() 
{
	if(m_Subframe){
		m_View->SetUndoState("Change mirror Y");
		m_Subframe->m_MirrorY = m_MirrorY.GetCheck() != FALSE;
	}
}


//////////////////////////////////////////////////////////////////////////
void CFramePage::OnBrowseImage() 
{
	if(m_Subframe){
		CString OrigFile;
		if(m_Subframe->m_Surface && m_Subframe->m_Surface->m_Filename) OrigFile = CString(m_Subframe->m_Surface->m_Filename);
		CString NewFile = GetRelativeFilename(m_Subframe->Game, OrigFile, "Image Files (*.bmp; *.tga; *.png; *.jpg)|*.bmp;*.tga;*.png;*.jpg", ((CSpriteEditApp*)AfxGetApp())->m_LastDirImage, this);
		if(NewFile!=""){
			m_View->SetUndoState("Change image");
			m_Subframe->Game->m_SurfaceStorage->RemoveSurface(m_Subframe->m_Surface);
			m_Subframe->m_Surface = m_Subframe->Game->m_SurfaceStorage->AddSurface((char*)LPCSTR(NewFile), false, D3DCOLGetR(m_Subframe->m_Transparent), D3DCOLGetG(m_Subframe->m_Transparent), D3DCOLGetB(m_Subframe->m_Transparent));
			m_Subframe->SetDefaultRect();

			if(!m_Subframe->m_Surface){				
				m_Image.SetWindowText("");
				MessageBox("Error loading image '" + NewFile + "'", "Error", MB_ICONERROR);
			}
			else{
				m_Image.SetWindowText(NewFile);
				((CMainFrame*)AfxGetMainWnd())->m_wndWorkSpace.Refresh();
			}
		}
	}	
}


//////////////////////////////////////////////////////////////////////////
void CFramePage::OnCopyDelay() 
{
	CBSprite* spr = m_View->m_Sprite;
	if(!spr || !m_Frame) return;

	if(MessageBox("Do you really want to apply this delay to all frames?", NULL, MB_YESNO|MB_ICONQUESTION)==IDYES){
		m_View->SetUndoState("Set delay to all frames");
		for(int i=0; i<spr->m_Frames.GetSize(); i++){
			spr->m_Frames[i]->m_Delay = m_Frame->m_Delay;
		}
		ShowTotalTime();
	}
}


//////////////////////////////////////////////////////////////////////////
void CFramePage::OnCopyTrans() 
{
	CBSprite* spr = m_View->m_Sprite;
	if(!spr || !m_Subframe) return;

	if(MessageBox("Do you really want to apply this transparent color to all frames?", NULL, MB_YESNO|MB_ICONQUESTION)==IDYES){
		m_View->SetUndoState("Set transparent to all frames");
		for(int i=0; i<spr->m_Frames.GetSize(); i++){
			for(int j=0; j<spr->m_Frames[i]->m_Subframes.GetSize(); j++){
				spr->m_Frames[i]->m_Subframes[j]->m_Transparent = m_Subframe->m_Transparent;
			}
		}
	}
	
	// force graphics to reload
	m_View->SetUndoState("Temporary");
	m_View->Undo();
}


//////////////////////////////////////////////////////////////////////////
void CFramePage::OnCopyAlpha() 
{
	CBSprite* spr = m_View->m_Sprite;
	if(!spr || !m_Subframe) return;

	if(MessageBox("Do you really want to apply this alpha color and opacity to all frames?", NULL, MB_YESNO|MB_ICONQUESTION)==IDYES){
		m_View->SetUndoState("Set alpha to all frames");
		for(int i=0; i<spr->m_Frames.GetSize(); i++){
			for(int j=0; j<spr->m_Frames[i]->m_Subframes.GetSize(); j++){
				spr->m_Frames[i]->m_Subframes[j]->m_Alpha = m_Subframe->m_Alpha;
			}
		}
	}
	
	// force graphics to reload
	m_View->SetUndoState("Temporary");
	m_View->Undo();
	
}


//////////////////////////////////////////////////////////////////////////
void CFramePage::OnCopyMirror() 
{
	CBSprite* spr = m_View->m_Sprite;
	if(!spr || !m_Subframe) return;

	if(MessageBox("Do you really want to apply this mirroring to all frames?", NULL, MB_YESNO|MB_ICONQUESTION)==IDYES){
		m_View->SetUndoState("Set mirror to all frames");
		for(int i=0; i<spr->m_Frames.GetSize(); i++){
			for(int j=0; j<spr->m_Frames[i]->m_Subframes.GetSize(); j++)
			{
				spr->m_Frames[i]->m_Subframes[j]->m_MirrorX = m_Subframe->m_MirrorX;
				spr->m_Frames[i]->m_Subframes[j]->m_MirrorY = m_Subframe->m_MirrorY;
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CFramePage::OnCopyHotspot() 
{
	CBSprite* spr = m_View->m_Sprite;
	if(!spr || !m_Subframe) return;

	if(MessageBox("Do you really want to apply this hotspot to all frames?", NULL, MB_YESNO|MB_ICONQUESTION)==IDYES){
		m_View->SetUndoState("Set hotspot to all frames");
		for(int i=0; i<spr->m_Frames.GetSize(); i++){
			for(int j=0; j<spr->m_Frames[i]->m_Subframes.GetSize(); j++){
				spr->m_Frames[i]->m_Subframes[j]->m_HotspotX = m_Subframe->m_HotspotX;
				spr->m_Frames[i]->m_Subframes[j]->m_HotspotY = m_Subframe->m_HotspotY;
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CFramePage::OnCopyMove() 
{
	CBSprite* spr = m_View->m_Sprite;
	if(!spr || !m_Frame) return;

	if(MessageBox("Do you really want to apply this move to all frames?", NULL, MB_YESNO|MB_ICONQUESTION)==IDYES){
		m_View->SetUndoState("Set move to all frames");
		for(int i=0; i<spr->m_Frames.GetSize(); i++){
			spr->m_Frames[i]->m_MoveX = m_Frame->m_MoveX;
			spr->m_Frames[i]->m_MoveY = m_Frame->m_MoveY;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CFramePage::OnBrowseSound() 
{
	if(m_Frame){
		CString OrigFile;
		if(m_Frame->m_Sound && m_Frame->m_Sound->m_SoundFilename) OrigFile = CString(m_Frame->m_Sound->m_SoundFilename);
		CString NewFile = GetRelativeFilename(m_Frame->Game, OrigFile, "Sound Files (*.ogg; *.wav)|*.ogg;*.wav", ((CSpriteEditApp*)AfxGetApp())->m_LastDirSound, this);
		if(NewFile==""){
			if(OrigFile!=""){
				if(MessageBox("Do you want to remove current sound?", NULL, MB_ICONQUESTION|MB_YESNO)==IDYES){
					m_View->SetUndoState("Remove sound");
					SAFE_DELETE(m_Frame->m_Sound);
					m_Sound.SetWindowText("");
				}
			}
		}
		else{
			m_View->SetUndoState("Change sound");
			SAFE_DELETE(m_Frame->m_Sound);
			m_Frame->m_Sound = ::new CBSound(m_Frame->Game);
			if(m_Frame->m_Sound){
				if(FAILED(m_Frame->m_Sound->SetSound((char*)LPCSTR(NewFile)))){
					SAFE_DELETE(m_Frame->m_Sound);
				}
			}

			if(!m_Frame->m_Sound){
				m_Sound.SetWindowText("");
				MessageBox("Error loading sound '" + NewFile + "'", "Error", MB_ICONERROR);
			}
			else m_Sound.SetWindowText(NewFile);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CFramePage::OnKeyframe() 
{
	if(m_Frame){
		m_View->SetUndoState("Change keyframe");
		m_Frame->m_Keyframe = m_Keyframe.GetCheck() != FALSE;
	}
}


//////////////////////////////////////////////////////////////////////////
void CFramePage::OnDecoration() 
{
	if(m_Subframe){
		m_View->SetUndoState("Change decoration");
		m_Subframe->m_Decoration = m_Decoration.GetCheck() != FALSE;
	}	
}


//////////////////////////////////////////////////////////////////////////
void CFramePage::OnKillSound() 
{
	if(m_Frame){
		m_View->SetUndoState("Change kill sound");
		m_Frame->m_KillSound = m_KillSound.GetCheck() != FALSE;
	}
}


//////////////////////////////////////////////////////////////////////////
void CFramePage::ShowTotalTime()
{
	DWORD TotalTime = 0;

	if(m_View && m_View->m_Sprite){
		for(int i=0; i<m_View->m_Sprite->m_Frames.GetSize(); i++) TotalTime+=m_View->m_Sprite->m_Frames[i]->m_Delay;
	}

	CString Str;
	Str.Format("%d", TotalTime);
	m_TotalTime.SetWindowText(Str);
}
