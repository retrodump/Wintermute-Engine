// SoundPage.cpp : implementation file
//

#include "stdafx.h"
#include "SceneEdit.h"
#include "SceneView.h"
#include "SoundPage.h"
#include "MainFrm.h"
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
// CSoundPage dialog


CSoundPage::CSoundPage(CWnd* pParent /*=NULL*/)
	: CDialog(CSoundPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSoundPage)
	//}}AFX_DATA_INIT

	m_Entity = NULL;
	m_View = NULL;

}


void CSoundPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSoundPage)
	DDX_Control(pDX, IDC_VOLUME_SPIN, m_VolumeSpin);
	DDX_Control(pDX, IDC_VOLUME, m_Volume);
	DDX_Control(pDX, IDC_START_TIME, m_StartTime);
	DDX_Control(pDX, IDC_SOUND_PANNING, m_SoundPanning);
	DDX_Control(pDX, IDC_CUSTOM, m_Custom);
	DDX_Control(pDX, IDC_PLAY, m_Play);
	DDX_Control(pDX, IDC_BROWSE_SOUND, m_BrowseSound);
	DDX_Control(pDX, IDC_EVENTS, m_Events);
	DDX_Control(pDX, IDC_SOUND, m_Sound);
	DDX_Control(pDX, IDC_POS_Y, m_PosY);
	DDX_Control(pDX, IDC_POS_X, m_PosX);
	DDX_Control(pDX, IDC_NAME, m_Name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSoundPage, CDialog)
	//{{AFX_MSG_MAP(CSoundPage)
	ON_WM_SIZE()
	ON_EN_CHANGE(IDC_NAME, OnChangeName)
	ON_BN_CLICKED(IDC_BROWSE_SOUND, OnBrowseSound)
	ON_BN_CLICKED(IDC_EVENTS, OnEvents)
	ON_EN_KILLFOCUS(IDC_POS_X, OnKillfocusPosX)
	ON_EN_KILLFOCUS(IDC_POS_Y, OnKillfocusPosY)
	ON_BN_CLICKED(IDC_PLAY, OnPlay)
	ON_BN_CLICKED(IDC_CUSTOM, OnCustom)
	ON_BN_CLICKED(IDC_SOUND_PANNING, OnSoundPanning)
	ON_EN_KILLFOCUS(IDC_START_TIME, OnKillfocusStartTime)
	ON_EN_KILLFOCUS(IDC_VOLUME, OnKillfocusVolume)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSoundPage message handlers


//////////////////////////////////////////////////////////////////////////
void CSoundPage::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if(::IsWindow(m_BrowseSound)){
		ScaleFilePair(NULL, &m_Name);
		ScaleFilePair(&m_BrowseSound, &m_Sound);
	}	
}


//////////////////////////////////////////////////////////////////////////
BOOL CSoundPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetupButton(&m_Play, "Play/stop sound", IDB_PLAY);

	m_Events.m_nFlatStyle = CBCGButton::BUTTONSTYLE_SEMIFLAT;
	m_BrowseSound.m_nFlatStyle = CBCGButton::BUTTONSTYLE_SEMIFLAT;	
	m_Custom.m_nFlatStyle = CBCGButton::BUTTONSTYLE_SEMIFLAT;	
	m_Play.m_nFlatStyle = CBCGButton::BUTTONSTYLE_SEMIFLAT;	

	LOGFONT logFont;
	CFont* pFont = GetFont();
	pFont->GetLogFont(&logFont);
	logFont.lfWeight = FW_BOLD;
	m_BoldFont.CreateFontIndirect(&logFont);

	m_VolumeSpin.SetRange(0, 100);
	
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
void CSoundPage::ScaleFilePair(CButton *Button, CEdit *Edit)
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
void CSoundPage::OnCancel() 
{
	// do nothing
}


//////////////////////////////////////////////////////////////////////////
void CSoundPage::OnOK() 
{
	SetPos();	
}


//////////////////////////////////////////////////////////////////////////
void CSoundPage::ShowEntity(CAdEntity *Entity, CSceneView* View, bool Force)
{
	if(Entity==m_Entity && !Force) return;
	else{
		m_Entity = Entity;
		m_View = View;
		if(m_Entity){
			m_Name.SetWindowText(m_Entity->m_Name);			

			if(m_Entity->m_SFX && m_Entity->m_SFX->m_SoundFilename)
				m_Sound.SetWindowText(m_Entity->m_SFX->m_SoundFilename);
			else
				m_Sound.SetWindowText("");

			CString str;
			str.Format("%d", (int)m_Entity->m_PosX);
			m_PosX.SetWindowText(str);
			str.Format("%d", (int)m_Entity->m_PosY);
			m_PosY.SetWindowText(str);

			str.Format("%d", m_Entity->m_SFXVolume);
			m_Volume.SetWindowText(str);

			str.Format("%d", m_Entity->m_SFXStart);
			m_StartTime.SetWindowText(str);

			m_SoundPanning.SetCheck(m_Entity->m_AutoSoundPanning);
		}
		SetState();
	}
}


//////////////////////////////////////////////////////////////////////////
void CSoundPage::OnChangeName() 
{
	if(m_Entity){
		CString str;
		m_Name.GetWindowText(str);
		if(str.Compare(m_Entity->m_Name)!=0){
			m_View->SetUndoState("Change name");
			m_Entity->SetName((char*)LPCSTR(str));
			((CMainFrame*)AfxGetMainWnd())->m_wndWorkSpace.Refresh(false);
		}
	}	
}


//////////////////////////////////////////////////////////////////////////
void CSoundPage::OnBrowseSound() 
{
	if(m_Entity){
		CString OrigFile;
		if(m_Entity->m_SFX && m_Entity->m_SFX->m_SoundFilename) OrigFile = CString(m_Entity->m_SFX->m_SoundFilename);
		CString NewFile = GetRelativeFilename(m_Entity->Game, OrigFile, "Sound Files (*.ogg; *.wav)|*.ogg;*.wav", ((CSceneEditApp*)AfxGetApp())->m_LastDirSound, this);
		if(NewFile==""){
			if(OrigFile!=""){
				if(MessageBox("Do you want to remove current sound?", "Sound", MB_ICONQUESTION|MB_YESNO)==IDYES){
					m_View->SetUndoState("Remove sound");
					SAFE_DELETE(m_Entity->m_SFX);
					m_Sound.SetWindowText("");
				}
			}
		}
		else{
			m_View->SetUndoState("Change sound");
			SAFE_DELETE(m_Entity->m_SFX);
			if(FAILED(m_Entity->PlaySFX((char*)LPCSTR(NewFile), false, false))){
				SAFE_DELETE(m_Entity->m_SFX);
				m_Sound.SetWindowText("");
				MessageBox("Error loading sound '" + NewFile + "'", "Error", MB_ICONERROR);
			}
			else m_Sound.SetWindowText(NewFile);
		}
	}
	SetState();
}


//////////////////////////////////////////////////////////////////////////
void CSoundPage::SetPos()
{
	if(m_Entity){
		int i1 = m_Entity->m_PosX;
		int i2 = m_Entity->m_PosY;

		UpdateIntEdit(this, &m_PosX, &i1);
		UpdateIntEdit(this, &m_PosY, &i2);

		if(i1 != m_Entity->m_PosX || i2 != m_Entity->m_PosY) m_View->SetUndoState("Change position");

		m_Entity->m_PosX = i1;
		m_Entity->m_PosY = i2;


		int Volume = m_Entity->m_SFXVolume;
		UpdateIntEdit(this, &m_Volume, &Volume);
		Volume = min(100, max(0, Volume));
		if(Volume != m_Entity->m_SFXVolume){
			m_View->SetUndoState("Change volume");
			m_Entity->SetSFXVolume(Volume);
		}

		int StartTime = m_Entity->m_SFXStart;
		UpdateIntEdit(this, &m_StartTime, &StartTime);
		if(StartTime==0 || StartTime != m_Entity->m_SFXStart){
			m_View->SetUndoState("Change start time");
			m_Entity->SetSFXTime(StartTime);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CSoundPage::OnKillfocusPosX() 
{
	SetPos();
}


//////////////////////////////////////////////////////////////////////////
void CSoundPage::OnKillfocusPosY() 
{
	SetPos();
}


//////////////////////////////////////////////////////////////////////////
void CSoundPage::OnKillfocusStartTime() 
{
	SetPos();
}


//////////////////////////////////////////////////////////////////////////
void CSoundPage::OnKillfocusVolume() 
{
	SetPos();
}


//////////////////////////////////////////////////////////////////////////
void CSoundPage::OnEvents() 
{
	if(m_Entity && m_View){
		CScriptDlg dlg;
		dlg.m_Object = m_Entity;
		dlg.m_View = m_View;
		if(dlg.DoModal()==IDOK) SetState();
	}	
}


//////////////////////////////////////////////////////////////////////////
void CSoundPage::SetState()
{
	if(!m_Entity) return;

	bool region = m_Entity->m_Region != NULL;

	m_BrowseSound.EnableWindow(true);
	m_Sound.EnableWindow(true);
	m_PosX.EnableWindow(true);
	m_PosY.EnableWindow(true);

	m_Play.EnableWindow(m_Entity->m_SFX!=NULL);


	if(m_Entity->m_Scripts.GetSize()>0)
		m_Events.SetFont(&m_BoldFont);
	else
		m_Events.SetFont(GetFont());


	if(m_Entity->m_ScProp && m_Entity->m_ScProp->m_ValObject.size()>0)
		m_Custom.SetFont(&m_BoldFont);
	else
		m_Custom.SetFont(GetFont());

}


//////////////////////////////////////////////////////////////////////////
void CSoundPage::OnPlay() 
{
	if(m_Entity){
		if(m_Entity->m_SFX && m_Entity->m_SFX->IsPlaying()) m_Entity->StopSFX(false);
		else m_Entity->PlaySFX(NULL);
	}
}


//////////////////////////////////////////////////////////////////////////
void CSoundPage::OnCustom() 
{
	if(m_View && m_Entity){
		CPropsDlg dlg;
		dlg.m_Object = m_Entity;
		dlg.m_View = m_View;
		if(dlg.DoModal()==IDOK) SetState();
	}
}

//////////////////////////////////////////////////////////////////////////
void CSoundPage::OnSoundPanning() 
{
	if(m_Entity){
		m_View->SetUndoState("Change sound panning");
		m_Entity->m_AutoSoundPanning = m_SoundPanning.GetCheck() != FALSE;
	}
}
