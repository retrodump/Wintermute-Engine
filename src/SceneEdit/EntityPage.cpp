// EntityPage.cpp : implementation file
//

#include "stdafx.h"
#include "SceneEdit.h"
#include "SceneView.h"
#include "EntityPage.h"
#include "MainFrm.h"
#include "../mfc_shared/utils_mfc.h"
#include "EventsDlg.h"
#include "ScriptDlg.h"
#include "PropsDlg.h"
#include ".\entitypage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEntityPage dialog


CEntityPage::CEntityPage(CWnd* pParent /*=NULL*/)
	: CDialog(CEntityPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEntityPage)
	//}}AFX_DATA_INIT

	m_Entity = NULL;
	m_View = NULL;
}


void CEntityPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEntityPage)
	DDX_Control(pDX, IDC_ITEM_NAME, m_Item);
	DDX_Control(pDX, IDC_IGNORE_ITEMS, m_IgnoreItems);
	DDX_Control(pDX, IDC_SOUND_PANNING, m_SoundPanning);
	DDX_Control(pDX, IDC_CUSTOM, m_Custom);
	DDX_Control(pDX, IDC_BROWSE_CAPTION, m_BrowseCaption);
	DDX_Control(pDX, IDC_CAPTION, m_Caption);
	DDX_Control(pDX, IDC_EVENTS, m_Events);
	DDX_Control(pDX, IDC_SPRITE, m_Sprite);
	DDX_Control(pDX, IDC_SHADOWABLE, m_Shadowable);
	DDX_Control(pDX, IDC_SCALABLE, m_Scalable);
	DDX_Control(pDX, IDC_REGISTRABLE, m_Registrable);
	DDX_Control(pDX, IDC_POS_Y, m_PosY);
	DDX_Control(pDX, IDC_POS_X, m_PosX);
	DDX_Control(pDX, IDC_WALK_X, m_WalkToX);
	DDX_Control(pDX, IDC_WALK_Y, m_WalkToY);
	DDX_Control(pDX, IDC_FONT, m_Font);
	DDX_Control(pDX, IDC_CURSOR, m_Cursor);
	DDX_Control(pDX, IDC_NAME, m_Name);
	DDX_Control(pDX, IDC_WALK_GET, m_WalkToGet);
	DDX_Control(pDX, IDC_WALK_SET, m_WalkToSet);
	DDX_Control(pDX, IDC_BROWSE_SPRITE, m_BrowseSprite);
	DDX_Control(pDX, IDC_BROWSE_FONT, m_BrowseFont);
	DDX_Control(pDX, IDC_BROWSE_CURSOR, m_BrowseCursor);
	DDX_Control(pDX, IDC_WALK_DIR, m_WalkToDir);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEntityPage, CDialog)
	//{{AFX_MSG_MAP(CEntityPage)
	ON_WM_SIZE()
	ON_EN_CHANGE(IDC_NAME, OnChangeName)
	ON_BN_CLICKED(IDC_REGISTRABLE, OnRegistrable)
	ON_BN_CLICKED(IDC_SCALABLE, OnScalable)
	ON_BN_CLICKED(IDC_SHADOWABLE, OnShadowable)
	ON_BN_CLICKED(IDC_BROWSE_SPRITE, OnBrowseSprite)
	ON_BN_CLICKED(IDC_BROWSE_FONT, OnBrowseFont)
	ON_BN_CLICKED(IDC_BROWSE_CURSOR, OnBrowseCursor)
	ON_EN_KILLFOCUS(IDC_POS_X, OnKillfocusPosX)
	ON_EN_KILLFOCUS(IDC_POS_Y, OnKillfocusPosY)
	ON_EN_KILLFOCUS(IDC_WALK_X, OnKillfocusWalkX)
	ON_EN_KILLFOCUS(IDC_WALK_Y, OnKillfocusWalkY)
	ON_BN_CLICKED(IDC_EVENTS, OnEvents)
	ON_EN_CHANGE(IDC_CAPTION, OnChangeCaption)
	ON_BN_CLICKED(IDC_CUSTOM, OnCustom)
	ON_BN_CLICKED(IDC_SOUND_PANNING, OnSoundPanning)
	ON_BN_CLICKED(IDC_IGNORE_ITEMS, OnIgnoreItems)
	ON_EN_CHANGE(IDC_ITEM_NAME, OnChangeItemName)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_WALK_DIR, OnWalkDir)
	ON_BN_CLICKED(IDC_WALK_GET, OnWalkGet)
	ON_BN_CLICKED(IDC_WALK_SET, OnWalkSet)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEntityPage message handlers


//////////////////////////////////////////////////////////////////////////
void CEntityPage::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if(::IsWindow(m_BrowseSprite)){
		ScaleFilePair(NULL, &m_Name);
		ScaleFilePair(NULL, &m_Item);
		ScaleFilePair(&m_BrowseSprite, &m_Sprite);
		ScaleFilePair(&m_BrowseFont, &m_Font);
		ScaleFilePair(&m_BrowseCursor, &m_Cursor);
		ScaleFilePair(&m_BrowseCaption, &m_Caption);
	}
	
	// TODO: Add your message handler code here
	
}


//////////////////////////////////////////////////////////////////////////
BOOL CEntityPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Events.m_nFlatStyle = CBCGButton::BUTTONSTYLE_SEMIFLAT;
	m_Custom.m_nFlatStyle = CBCGButton::BUTTONSTYLE_SEMIFLAT;
	m_BrowseSprite.m_nFlatStyle = CBCGButton::BUTTONSTYLE_SEMIFLAT;
	m_BrowseFont.m_nFlatStyle = CBCGButton::BUTTONSTYLE_SEMIFLAT;
	m_BrowseCursor.m_nFlatStyle = CBCGButton::BUTTONSTYLE_SEMIFLAT;
	m_BrowseCaption.m_nFlatStyle = CBCGButton::BUTTONSTYLE_SEMIFLAT;

	m_WalkToGet.m_nFlatStyle = CBCGButton::BUTTONSTYLE_SEMIFLAT;
	m_WalkToSet.m_nFlatStyle = CBCGButton::BUTTONSTYLE_SEMIFLAT;


	LOGFONT logFont;
	CFont* pFont = GetFont();
	pFont->GetLogFont(&logFont);
	logFont.lfWeight = FW_BOLD;
	m_BoldFont.CreateFontIndirect(&logFont);

	m_WalkToDir.Clear();
	m_WalkToDir.AddString("Up"); 
	m_WalkToDir.AddString("Up Right"); 
	m_WalkToDir.AddString("Right"); 
	m_WalkToDir.AddString("Down Right"); 
	m_WalkToDir.AddString("Down"); 
	m_WalkToDir.AddString("Down Left"); 
	m_WalkToDir.AddString("Left"); 
	m_WalkToDir.AddString("Up Left"); 
	m_WalkToDir.AddString("<none>"); 
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//////////////////////////////////////////////////////////////////////////
void CEntityPage::ScaleFilePair(CButton *Button, CEdit *Edit)
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
void CEntityPage::OnCancel() 
{
	// do nothing
}


//////////////////////////////////////////////////////////////////////////
void CEntityPage::OnOK() 
{
	SetPos();
}


//////////////////////////////////////////////////////////////////////////
void CEntityPage::ShowEntity(CAdEntity *Entity, CSceneView* View, bool Force)
{
	if(Entity==m_Entity && !Force) return;
	else{
		m_Entity = Entity;
		m_View = View;
		if(m_Entity){
			m_Name.SetWindowText(m_Entity->m_Name);
			
			m_Caption.SetWindowText(m_Entity->GetCaption());

			if(m_Entity->m_Item) m_Item.SetWindowText(m_Entity->m_Item);
			else m_Item.SetWindowText("");
			

			if(m_Entity->m_Sprite && m_Entity->m_Sprite->m_Filename)
				m_Sprite.SetWindowText(m_Entity->m_Sprite->m_Filename);
			else
				m_Sprite.SetWindowText("");

			if(m_Entity->m_Font && m_Entity->m_Font->m_Filename)
				m_Font.SetWindowText(m_Entity->m_Font->m_Filename);
			else
				m_Font.SetWindowText("");

			if(m_Entity->m_Cursor && m_Entity->m_Cursor->m_Filename)
				m_Cursor.SetWindowText(m_Entity->m_Cursor->m_Filename);
			else
				m_Cursor.SetWindowText("");

			CString str;
			str.Format("%d", (int)m_Entity->m_PosX);
			m_PosX.SetWindowText(str);
			str.Format("%d", (int)m_Entity->m_PosY);
			m_PosY.SetWindowText(str);

			str.Format("%d", (int)m_Entity->m_WalkToX);
			m_WalkToX.SetWindowText(str);
			str.Format("%d", (int)m_Entity->m_WalkToY);
			m_WalkToY.SetWindowText(str);

			if(m_Entity->m_WalkToDir<NUM_DIRECTIONS)
				m_WalkToDir.SetCurSel((int)m_Entity->m_WalkToDir);
			else
				m_WalkToDir.SetCurSel((int)NUM_DIRECTIONS);


			m_Scalable.SetCheck(m_Entity->m_Zoomable);
			m_Registrable.SetCheck(m_Entity->m_Registrable);
			m_Shadowable.SetCheck(m_Entity->m_Shadowable);
			m_SoundPanning.SetCheck(m_Entity->m_AutoSoundPanning);
			m_IgnoreItems.SetCheck(m_Entity->m_IgnoreItems);
		}
		SetState();
	}
}


//////////////////////////////////////////////////////////////////////////
void CEntityPage::OnChangeName() 
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
void CEntityPage::OnChangeCaption() 
{
	if(m_Entity){
		CString str;
		m_Caption.GetWindowText(str);
		if(str.Compare(m_Entity->GetCaption())!=0){
			m_View->SetUndoState("Change caption");
			m_Entity->SetCaption((char*)LPCSTR(str));
			((CMainFrame*)AfxGetMainWnd())->m_wndWorkSpace.Refresh(false);
		}
	}	
}


//////////////////////////////////////////////////////////////////////////
void CEntityPage::OnRegistrable() 
{
	if(m_Entity){
		m_View->SetUndoState("Change registrable");
		m_Entity->m_Registrable = m_Registrable.GetCheck() != FALSE;
	}
}


//////////////////////////////////////////////////////////////////////////
void CEntityPage::OnScalable() 
{
	if(m_Entity){
		m_View->SetUndoState("Change scalable");
		m_Entity->m_Zoomable = m_Scalable.GetCheck() != FALSE;
	}
}


//////////////////////////////////////////////////////////////////////////
void CEntityPage::OnShadowable() 
{
	if(m_Entity){
		m_View->SetUndoState("Change shadowable");
		m_Entity->m_Shadowable = m_Shadowable.GetCheck() != FALSE;
	}
}


//////////////////////////////////////////////////////////////////////////
void CEntityPage::OnSoundPanning() 
{
	if(m_Entity){
		m_View->SetUndoState("Change sound panning");
		m_Entity->m_AutoSoundPanning = m_SoundPanning.GetCheck() != FALSE;
	}
}


//////////////////////////////////////////////////////////////////////////
void CEntityPage::OnBrowseSprite() 
{
	if(m_Entity){
		CString OrigFile;
		if(m_Entity->m_Sprite && m_Entity->m_Sprite->m_Filename) OrigFile = CString(m_Entity->m_Sprite->m_Filename);
		CString NewFile = GetRelativeFilename(m_Entity->Game, OrigFile, "Sprite Files (*.sprite; *.bmp; *.tga; *.png; *.jpg)|*.sprite;*.bmp;*.tga;*.png;*.jpg", ((CSceneEditApp*)AfxGetApp())->m_LastDirSprite, this);
		if(NewFile==""){
			if(OrigFile!=""){
				if(MessageBox("Do you want to remove current sprite?", "Sprite", MB_ICONQUESTION|MB_YESNO)==IDYES){
					m_View->SetUndoState("Remove sprite");
					SAFE_DELETE(m_Entity->m_Sprite);
					m_Sprite.SetWindowText("");
				}
			}
		}
		else{
			m_View->SetUndoState("Change sprite");
			SAFE_DELETE(m_Entity->m_Sprite);
			m_Entity->m_Sprite = ::new CBSprite(m_Entity->Game, m_Entity);
			if(!m_Entity->m_Sprite || FAILED(m_Entity->m_Sprite->LoadFile((char*)LPCSTR(NewFile)))){
				SAFE_DELETE(m_Entity->m_Sprite);
				m_Sprite.SetWindowText("");
				MessageBox("Error loading sprite '" + NewFile + "'", "Error", MB_ICONERROR);
			}
			else m_Sprite.SetWindowText(NewFile);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CEntityPage::OnBrowseFont() 
{
	if(m_Entity){
		CString OrigFile;
		if(m_Entity->m_Font && m_Entity->m_Font->m_Filename) OrigFile = CString(m_Entity->m_Font->m_Filename);
		CString NewFile = GetRelativeFilename(m_Entity->Game, OrigFile, "Font Files (*.font; *.fnt)|*.font;*.fnt", ((CSceneEditApp*)AfxGetApp())->m_LastDirFont, this);
		if(NewFile==""){
			if(OrigFile!=""){
				if(MessageBox("Do you want to remove current font?", "Font", MB_ICONQUESTION|MB_YESNO)==IDYES){
					m_View->SetUndoState("Remove font");
					m_Entity->Game->m_FontStorage->RemoveFont(m_Entity->m_Font);
					m_Entity->m_Font = NULL;
					m_Font.SetWindowText("");
				}
			}
		}
		else{
			m_View->SetUndoState("Change font");
			m_Entity->Game->m_FontStorage->RemoveFont(m_Entity->m_Font);
			m_Entity->m_Font = m_Entity->Game->m_FontStorage->AddFont((char*)LPCSTR(NewFile));
			if(!m_Entity->m_Font){
				m_Font.SetWindowText("");
				MessageBox("Error loading font '" + NewFile + "'", "Error", MB_ICONERROR);
			}
			else m_Font.SetWindowText(NewFile);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CEntityPage::OnBrowseCursor() 
{
	if(m_Entity){
		CString OrigFile;
		if(m_Entity->m_Cursor && m_Entity->m_Cursor->m_Filename) OrigFile = CString(m_Entity->m_Cursor->m_Filename);
		CString NewFile = GetRelativeFilename(m_Entity->Game, OrigFile, "Sprite Files (*.sprite; *.bmp; *.tga; *.png; *.jpg)|*.sprite;*.bmp;*.tga;*.png;*.jpg", ((CSceneEditApp*)AfxGetApp())->m_LastDirCursor, this);
		if(NewFile==""){
			if(OrigFile!=""){
				if(MessageBox("Do you want to remove current cursor?", "Cursor", MB_ICONQUESTION|MB_YESNO)==IDYES){
					m_View->SetUndoState("Remove cursor");
					SAFE_DELETE(m_Entity->m_Cursor);
					m_Cursor.SetWindowText("");
				}
			}
		}
		else{
			m_View->SetUndoState("Change cursor");
			SAFE_DELETE(m_Entity->m_Cursor);
			m_Entity->m_Cursor = ::new CBSprite(m_Entity->Game, m_Entity);
			if(!m_Entity->m_Cursor || FAILED(m_Entity->m_Cursor->LoadFile((char*)LPCSTR(NewFile)))){
				SAFE_DELETE(m_Entity->m_Cursor);
				m_Cursor.SetWindowText("");
				MessageBox("Error loading cursor '" + NewFile + "'", "Error", MB_ICONERROR);
			}
			else m_Cursor.SetWindowText(NewFile);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CEntityPage::SetPos()
{
	if(m_Entity){
		int i1 = m_Entity->m_PosX;
		int i2 = m_Entity->m_PosY;
		int i3 = m_Entity->m_WalkToX;
		int i4 = m_Entity->m_WalkToY;

		UpdateIntEdit(this, &m_PosX, &i1);
		UpdateIntEdit(this, &m_PosY, &i2);
		UpdateIntEdit(this, &m_WalkToX, &i3);
		UpdateIntEdit(this, &m_WalkToY, &i4);

		if(i1 != m_Entity->m_PosX || i2 != m_Entity->m_PosY || i3 != m_Entity->m_WalkToX || i4 != m_Entity->m_WalkToY) m_View->SetUndoState("Change position/walk to point");

		m_Entity->m_PosX = i1;
		m_Entity->m_PosY = i2;
		m_Entity->m_WalkToX = i3;
		m_Entity->m_WalkToY = i4;
	}
}


//////////////////////////////////////////////////////////////////////////
void CEntityPage::OnKillfocusPosX() 
{
	SetPos();	
}


//////////////////////////////////////////////////////////////////////////
void CEntityPage::OnKillfocusPosY() 
{
	SetPos();
}

//////////////////////////////////////////////////////////////////////////
void CEntityPage::OnKillfocusWalkX() 
{
	SetPos();
}

//////////////////////////////////////////////////////////////////////////
void CEntityPage::OnKillfocusWalkY() 
{
	SetPos();
}


//////////////////////////////////////////////////////////////////////////
void CEntityPage::OnEvents() 
{
	if(m_Entity && m_View){
		CScriptDlg dlg;
		dlg.m_Object = m_Entity;
		dlg.m_View = m_View;
		if(dlg.DoModal()==IDOK) SetState();
	}	
}


//////////////////////////////////////////////////////////////////////////
void CEntityPage::SetState()
{
	if(!m_Entity) return;

	bool region = m_Entity->m_Region != NULL;

	m_BrowseSprite.EnableWindow(!region);
	m_Sprite.EnableWindow(!region);
	m_Scalable.EnableWindow(!region);
	m_Shadowable.EnableWindow(!region);	
	m_PosX.EnableWindow(!region);
	m_PosY.EnableWindow(!region);
	//m_BrowseCaption.EnableWindow(!region);
	m_BrowseCaption.EnableWindow(FALSE);
	m_Item.EnableWindow(!region);


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
void CEntityPage::OnCustom() 
{
	if(m_View && m_Entity){
		CPropsDlg dlg;
		dlg.m_Object = m_Entity;
		dlg.m_View = m_View;
		if(dlg.DoModal()==IDOK) SetState();
	}
}


//////////////////////////////////////////////////////////////////////////
void CEntityPage::OnIgnoreItems() 
{
	if(m_Entity){
		m_View->SetUndoState("Change ignore items");
		m_Entity->m_IgnoreItems = m_IgnoreItems.GetCheck() != FALSE;
	}
}


//////////////////////////////////////////////////////////////////////////
void CEntityPage::OnChangeItemName() 
{
	if(m_Entity){
		CString str;
		m_Item.GetWindowText(str);
		if(!m_Entity->m_Item || str.Compare(m_Entity->m_Item)!=0){
			//m_View->SetUndoState("Change item");
			m_Entity->SetItem((char*)LPCSTR(str));
			//((CMainFrame*)AfxGetMainWnd())->m_wndWorkSpace.Refresh(false);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void CEntityPage::OnWalkDir()
{
	if(m_Entity){
		int Sel = m_WalkToDir.GetCurSel();		
		if(Sel!=CB_ERR){
			if((TDirection)Sel>=NUM_DIRECTIONS) m_Entity->m_WalkToDir = DI_NONE;
			else m_Entity->m_WalkToDir = (TDirection)Sel;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void CEntityPage::OnWalkGet()
{
	if(m_View && (m_View->m_Actor || m_View->m_Actor3D || m_View->m_Actor3DX) && m_Entity){

		m_View->SetUndoState("Set walk-to point");

		if(m_View->m_Actor)
		{
			m_Entity->m_WalkToX = m_View->m_Actor->m_PosX;
			m_Entity->m_WalkToY = m_View->m_Actor->m_PosY;
			m_Entity->m_WalkToDir = m_View->m_Actor->m_Dir;
		}
		else if(m_View->m_Actor3D)
		{
			m_Entity->m_WalkToX = m_View->m_Actor3D->m_PosX;
			m_Entity->m_WalkToY = m_View->m_Actor3D->m_PosY;
			m_Entity->m_WalkToDir = m_View->m_Actor3D->AngleToDir(m_View->m_Actor3D->m_Angle);
		}
		else if(m_View->m_Actor3DX)
		{
			m_Entity->m_WalkToX = m_View->m_Actor3DX->m_PosX;
			m_Entity->m_WalkToY = m_View->m_Actor3DX->m_PosY;
			m_Entity->m_WalkToDir = m_View->m_Actor3DX->AngleToDir(m_View->m_Actor3DX->m_Angle);
		}
		
		CString str;
		str.Format("%d", (int)m_Entity->m_WalkToX);
		m_WalkToX.SetWindowText(str);
		str.Format("%d", (int)m_Entity->m_WalkToY);
		m_WalkToY.SetWindowText(str);

		m_WalkToDir.SetCurSel((int)m_Entity->m_WalkToDir);

	}
	else MessageBox("You have to place an actor into the scene first. Use the 'Actor->Place actor' menu command or click the actor button on toolbar.", NULL, MB_OK|MB_ICONINFORMATION);
}


//////////////////////////////////////////////////////////////////////////
void CEntityPage::OnWalkSet()
{
	if(m_View && (m_View->m_Actor || m_View->m_Actor3D || m_View->m_Actor3DX) && m_Entity)
	{
		if(m_View->m_Actor)
		{
			m_View->m_Actor->m_PosX = m_Entity->m_WalkToX;
			m_View->m_Actor->m_PosY = m_Entity->m_WalkToY;
			m_View->m_Actor->m_Dir  = m_Entity->m_WalkToDir;
		}
		else if(m_View->m_Actor3D)
		{
			m_View->m_Actor3D->SkipTo(m_Entity->m_WalkToX, m_Entity->m_WalkToY);
			m_View->m_Actor3D->m_Angle = m_View->m_Actor3D->DirToAngle(m_Entity->m_WalkToDir);
		}
		else if(m_View->m_Actor3DX)
		{
			m_View->m_Actor3DX->SkipTo(m_Entity->m_WalkToX, m_Entity->m_WalkToY);
			m_View->m_Actor3DX->m_Angle = m_View->m_Actor3DX->DirToAngle(m_Entity->m_WalkToDir);
		}
	}
	else MessageBeep(MB_OK);
}
