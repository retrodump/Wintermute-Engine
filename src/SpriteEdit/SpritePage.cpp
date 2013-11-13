// SpritePage.cpp : implementation file
//

#include "stdafx.h"
#include "SpriteEdit.h"
#include "SpritePage.h"
#include "MainFrm.h"
#include "../mfc_shared/utils_mfc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpritePage dialog


CSpritePage::CSpritePage(CWnd* pParent /*=NULL*/)
	: CDialog(CSpritePage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpritePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_View = NULL;
	m_IsRedrawing = false;
}


void CSpritePage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpritePage)
	DDX_Control(pDX, IDC_LOOPING, m_Looping);
	DDX_Control(pDX, IDC_PREV_FRAME, m_PrevFrame);
	DDX_Control(pDX, IDC_PLAY_STOP, m_PlayStop);
	DDX_Control(pDX, IDC_NEXT_FRAME, m_NextFrame);
	DDX_Control(pDX, IDC_LAST_FRAME, m_LastFrame);
	DDX_Control(pDX, IDC_FIRST_FRAME, m_FirstFrame);
	DDX_Control(pDX, IDC_ADD_SUBFRAME, m_AddSubframe);
	DDX_Control(pDX, IDC_SUBFRAME_DOWN, m_SubframeDown);
	DDX_Control(pDX, IDC_SUBFRAME_UP, m_SubframeUp);
	DDX_Control(pDX, IDC_FRAME_DOWN, m_FrameDown);
	DDX_Control(pDX, IDC_FRAME_UP, m_FrameUp);
	DDX_Control(pDX, IDC_REMOVE_FRAME, m_RemoveFrame);
	DDX_Control(pDX, IDC_ADD_FRAME, m_AddFrame);
	DDX_Control(pDX, IDC_SPRITE, m_SpriteTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSpritePage, CDialog)
	//{{AFX_MSG_MAP(CSpritePage)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_SPRITE, OnSelchangedSprite)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_SPRITE, OnItemexpandedSprite)
	ON_BN_CLICKED(IDC_FRAME_UP, OnFrameUp)
	ON_BN_CLICKED(IDC_FRAME_DOWN, OnFrameDown)
	ON_BN_CLICKED(IDC_SUBFRAME_UP, OnSubframeUp)
	ON_BN_CLICKED(IDC_SUBFRAME_DOWN, OnSubframeDown)
	ON_BN_CLICKED(IDC_REMOVE_FRAME, OnRemoveFrame)
	ON_BN_CLICKED(IDC_ADD_FRAME, OnAddFrame)
	ON_BN_CLICKED(IDC_ADD_SUBFRAME, OnAddSubframe)
	ON_BN_CLICKED(IDC_FIRST_FRAME, OnFirstFrame)
	ON_BN_CLICKED(IDC_LAST_FRAME, OnLastFrame)
	ON_BN_CLICKED(IDC_NEXT_FRAME, OnNextFrame)
	ON_BN_CLICKED(IDC_PREV_FRAME, OnPrevFrame)
	ON_BN_CLICKED(IDC_PLAY_STOP, OnPlayStop)
	ON_BN_CLICKED(IDC_LOOPING, OnLooping)
	ON_BN_DOUBLECLICKED(IDC_FRAME_UP, OnFrameUp)
	ON_BN_DOUBLECLICKED(IDC_FRAME_DOWN, OnFrameDown)
	ON_BN_DOUBLECLICKED(IDC_SUBFRAME_UP, OnSubframeUp)
	ON_BN_DOUBLECLICKED(IDC_SUBFRAME_DOWN, OnSubframeDown)
	ON_BN_DOUBLECLICKED(IDC_NEXT_FRAME, OnNextFrame)
	ON_BN_DOUBLECLICKED(IDC_PREV_FRAME, OnPrevFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpritePage message handlers


//////////////////////////////////////////////////////////////////////////
void CSpritePage::OnOK() 
{
	// do nothing
}


//////////////////////////////////////////////////////////////////////////
void CSpritePage::OnCancel() 
{
	// do nothing
}


//////////////////////////////////////////////////////////////////////////
BOOL CSpritePage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	SetupButton(&m_FrameUp,       "Frame up",         IDB_MOVE_UP);
	SetupButton(&m_FrameDown,     "Frame down",       IDB_MOVE_DOWN);
	SetupButton(&m_SubframeUp,    "Subframe up",      IDB_SUB_UP);
	SetupButton(&m_SubframeDown,  "Subframe down",    IDB_SUB_DOWN);
	SetupButton(&m_AddFrame,      "Add frame(s)",     IDB_NEW_FRAME);
	SetupButton(&m_AddSubframe,   "Add subframe",     IDB_NEW_SUBFRAME);
	SetupButton(&m_RemoveFrame,   "Remove frame",     IDB_BCGBARRES_DELETE);

	SetupButton(&m_FirstFrame,    "First frame",      IDB_FIRST_FRAME);
	SetupButton(&m_LastFrame,     "Last frame",       IDB_LAST_FRAME);
	SetupButton(&m_PrevFrame,     "Previous frame",   IDB_PREV_FRAME);
	SetupButton(&m_NextFrame,     "Next frame",       IDB_NEXT_FRAME);
	SetupButton(&m_PlayStop,      "Play",             IDB_PLAY);

	SetState();

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
void CSpritePage::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CRect rectClient;
	GetClientRect(&rectClient);

	CRect rectTree;

	if(m_SpriteTree.GetSafeHwnd()){
		m_SpriteTree.GetWindowRect(&rectTree);
		ScreenToClient(&rectTree);
		rectTree.right = rectTree.left + (rectClient.right - rectClient.left);
		rectTree.bottom = /*rectTree.top +*/ (rectClient.bottom - rectClient.top);
		m_SpriteTree.MoveWindow(&rectTree);
	}
	
}


//////////////////////////////////////////////////////////////////////////
void CSpritePage::Redraw()
{
	m_IsRedrawing = true;
	m_SpriteTree.SetRedraw(FALSE);

	m_SpriteTree.DeleteAllItems();

	if(m_View && m_View->m_Sprite){
		CBSprite* spr = m_View->m_Sprite;

		m_Looping.SetCheck(spr->m_Looping);

		for(int i=0; i<spr->m_Frames.GetSize(); i++){			
			
			for(int j=0; j<spr->m_Frames[i]->m_Subframes.GetSize(); j++){
				CBSubFrame* sub = spr->m_Frames[i]->m_Subframes[j];

				if(!sub->m_Surface) continue;

				HTREEITEM MainFrame;
				if(j==0){
					MainFrame = m_SpriteTree.InsertItem(sub->m_Surface->m_Filename);					
					m_SpriteTree.SetItemData(MainFrame, (DWORD)sub);

					if(sub==m_View->m_SelectedSubframe){
						m_SpriteTree.Select(MainFrame, TVGN_CARET);
						m_SpriteTree.EnsureVisible(MainFrame);
					}
				}
				else{
					HTREEITEM SubFrame = m_SpriteTree.InsertItem (sub->m_Surface->m_Filename, MainFrame);
					m_SpriteTree.SetItemData(SubFrame, (DWORD)sub);
					
					if(spr->m_Frames[i]->m_EditorExpanded) m_SpriteTree.Expand(MainFrame, TVE_EXPAND);

					if(sub==m_View->m_SelectedSubframe){
						m_SpriteTree.Select(SubFrame, TVGN_CARET);
						m_SpriteTree.EnsureVisible(SubFrame);
					}
				}
			}	
		}		
	}

	if(m_View && m_View->m_Sprite && m_View->m_SelectedSubframe){
		((CMainFrame*)AfxGetMainWnd())->m_wndProperty.ShowFrame(m_View, m_View->m_Sprite->m_Frames[m_View->m_Sprite->m_CurrentFrame], m_View->m_SelectedSubframe, true);
	}
	else {
		((CMainFrame*)AfxGetMainWnd())->m_wndProperty.ShowNone();
	}

	SetState();

	m_SpriteTree.SetRedraw(TRUE);
	m_IsRedrawing = false;
}


//////////////////////////////////////////////////////////////////////////
void CSpritePage::SetState()
{
	m_SpriteTree.EnableWindow(m_View && !m_View->m_Playing);

	CBFrame* frame = NULL;
	if(m_View && m_View->m_Sprite->m_CurrentFrame >= 0 && m_View->m_Sprite->m_CurrentFrame < m_View->m_Sprite->m_Frames.GetSize())
		frame = m_View->m_Sprite->m_Frames[m_View->m_Sprite->m_CurrentFrame];

	m_FrameUp.EnableWindow(m_View && !m_View->m_Playing && m_View->m_Sprite->m_CurrentFrame > 0);
	m_FrameDown.EnableWindow(m_View && !m_View->m_Playing && m_View->m_Sprite->m_CurrentFrame < m_View->m_Sprite->m_Frames.GetSize()-1);

	m_SubframeUp.EnableWindow(frame && !m_View->m_Playing && frame->m_EditorExpanded && m_View->m_SelectedSubframe && m_View->m_SelectedSubframe!=frame->m_Subframes[0]);
	m_SubframeDown.EnableWindow(frame && !m_View->m_Playing && frame->m_EditorExpanded && m_View->m_SelectedSubframe && m_View->m_SelectedSubframe!=frame->m_Subframes[frame->m_Subframes.GetSize()-1]);

	m_AddFrame.EnableWindow(m_View &&!m_View->m_Playing);
	m_AddSubframe.EnableWindow(frame && !m_View->m_Playing);
	m_Looping.EnableWindow(frame && !m_View->m_Playing);


	if(m_View && m_View->m_Playing)
		SetupButton(&m_PlayStop, "Stop", IDB_STOP);
	else
		SetupButton(&m_PlayStop, "Play", IDB_PLAY);

	m_RemoveFrame.EnableWindow(m_View && !m_View->m_Playing && m_View->m_SelectedSubframe);

	m_PlayStop.EnableWindow(m_View && m_View->m_Sprite->m_Frames.GetSize()>0);
	m_FirstFrame.EnableWindow(m_View && !m_View->m_Playing && m_View->m_Sprite->m_CurrentFrame>0);
	m_LastFrame.EnableWindow(m_View && !m_View->m_Playing && m_View->m_Sprite->m_CurrentFrame>=0 && m_View->m_Sprite->m_CurrentFrame<m_View->m_Sprite->m_Frames.GetSize()-1);

	m_PrevFrame.EnableWindow(m_View && !m_View->m_Playing && m_View->m_Sprite->m_Frames.GetSize()>1);
	m_NextFrame.EnableWindow(m_View && !m_View->m_Playing && m_View->m_Sprite->m_Frames.GetSize()>1);
}


//////////////////////////////////////////////////////////////////////////
void CSpritePage::SetCurrentView(CSpriteView *View)
{
	if(View==m_View) return;

	m_View = View;
	Redraw();
}


//////////////////////////////////////////////////////////////////////////
void CSpritePage::OnSelchangedSprite(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	if(m_View && !m_IsRedrawing){
		CBSubFrame* sub=NULL;

		HTREEITEM item = m_SpriteTree.GetSelectedItem();
		if(item) sub = (CBSubFrame*)m_SpriteTree.GetItemData(item);

		m_View->SelectSubframe(sub);

		if(m_View && m_View->m_Sprite && m_View->m_SelectedSubframe){
			((CMainFrame*)AfxGetMainWnd())->m_wndProperty.ShowFrame(m_View, m_View->m_Sprite->m_Frames[m_View->m_Sprite->m_CurrentFrame], m_View->m_SelectedSubframe, true);
		}
		else {
			((CMainFrame*)AfxGetMainWnd())->m_wndProperty.ShowNone();
		}
	}
	SetState();

	*pResult = 0;
}


//////////////////////////////////////////////////////////////////////////
void CSpritePage::OnItemexpandedSprite(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	CBSubFrame* sub = (CBSubFrame*)m_SpriteTree.GetItemData(pNMTreeView->itemNew.hItem);
	if(sub && m_View) m_View->SetExpand(sub, pNMTreeView->action==TVE_EXPAND);
	SetState();
	*pResult = 0;
}


//////////////////////////////////////////////////////////////////////////
void CSpritePage::OnFrameUp() 
{
	m_View->SetUndoState("Frame up");
	CBFrame* frame = m_View->m_Sprite->m_Frames[m_View->m_Sprite->m_CurrentFrame-1];

	m_View->m_Sprite->m_Frames[m_View->m_Sprite->m_CurrentFrame-1] = m_View->m_Sprite->m_Frames[m_View->m_Sprite->m_CurrentFrame];
	m_View->m_Sprite->m_Frames[m_View->m_Sprite->m_CurrentFrame] = frame;

	m_View->m_Sprite->m_CurrentFrame--;
	Redraw();
}


//////////////////////////////////////////////////////////////////////////
void CSpritePage::OnFrameDown() 
{
	m_View->SetUndoState("Frame down");
	CBFrame* frame = m_View->m_Sprite->m_Frames[m_View->m_Sprite->m_CurrentFrame+1];

	m_View->m_Sprite->m_Frames[m_View->m_Sprite->m_CurrentFrame+1] = m_View->m_Sprite->m_Frames[m_View->m_Sprite->m_CurrentFrame];
	m_View->m_Sprite->m_Frames[m_View->m_Sprite->m_CurrentFrame] = frame;

	m_View->m_Sprite->m_CurrentFrame++;
	Redraw();
	
}


//////////////////////////////////////////////////////////////////////////
void CSpritePage::OnSubframeUp() 
{	
	CBFrame* frame = m_View->m_Sprite->m_Frames[m_View->m_Sprite->m_CurrentFrame];
	
	for(int i=0; i<frame->m_Subframes.GetSize(); i++){
		if(frame->m_Subframes[i]==m_View->m_SelectedSubframe){
			m_View->SetUndoState("Subframe up");
			frame->m_Subframes[i] = frame->m_Subframes[i-1];
			frame->m_Subframes[i-1] = m_View->m_SelectedSubframe;
			Redraw();
			break;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CSpritePage::OnSubframeDown() 
{
	CBFrame* frame = m_View->m_Sprite->m_Frames[m_View->m_Sprite->m_CurrentFrame];
	
	for(int i=0; i<frame->m_Subframes.GetSize(); i++){
		if(frame->m_Subframes[i]==m_View->m_SelectedSubframe){
			m_View->SetUndoState("Subframe down");
			frame->m_Subframes[i] = frame->m_Subframes[i+1];
			frame->m_Subframes[i+1] = m_View->m_SelectedSubframe;
			Redraw();
			break;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CSpritePage::OnRemoveFrame() 
{
	CBFrame* frame = m_View->m_Sprite->m_Frames[m_View->m_Sprite->m_CurrentFrame];
	
	for(int i=0; i<frame->m_Subframes.GetSize(); i++){
		if(frame->m_Subframes[i]==m_View->m_SelectedSubframe){
			// whole frame
			if(i==0){
				if(frame->m_Subframes.GetSize()==1){
					if(MessageBox("Do you really want to remove this frame?", NULL, MB_YESNO|MB_ICONQUESTION)==IDYES){
						m_View->SetUndoState("Remove frame");
						delete m_View->m_Sprite->m_Frames[m_View->m_Sprite->m_CurrentFrame];
						m_View->m_Sprite->m_Frames.RemoveAt(m_View->m_Sprite->m_CurrentFrame);
						m_View->m_SelectedSubframe = NULL;
					}
				}
				else{
					int ret = MessageBox("Do you want to remove this frame including subframes?\n\n  •  Press \"Yes\" to remove whole frame\n  •  Press \"No\" to remove only the selected subframe", NULL, MB_YESNOCANCEL|MB_ICONQUESTION);
					if(ret==IDYES){
						m_View->SetUndoState("Remove frame");
						delete m_View->m_Sprite->m_Frames[m_View->m_Sprite->m_CurrentFrame];
						m_View->m_Sprite->m_Frames.RemoveAt(m_View->m_Sprite->m_CurrentFrame);
						m_View->m_SelectedSubframe = NULL;
					}
					else if(ret==IDNO){
						m_View->SetUndoState("Remove subframe");
						delete frame->m_Subframes[i];
						frame->m_Subframes.RemoveAt(i);
						while(i>=frame->m_Subframes.GetSize()) i--;
						if(i<0) m_View->m_SelectedSubframe = NULL;
						else m_View->m_SelectedSubframe = frame->m_Subframes[i];
					}
				}
			}
			// subframe
			else{
				if(MessageBox("Do you really want to remove this subframe?", NULL, MB_YESNO|MB_ICONQUESTION)==IDYES){
					m_View->SetUndoState("Remove subframe");
					delete frame->m_Subframes[i];
					frame->m_Subframes.RemoveAt(i);
					while(i>=frame->m_Subframes.GetSize()) i--;
					if(i<0) m_View->m_SelectedSubframe = NULL;
					else m_View->m_SelectedSubframe = frame->m_Subframes[i];
				}
			}
			break;
		}
	}	

	while(m_View->m_Sprite->m_CurrentFrame >= m_View->m_Sprite->m_Frames.GetSize()) m_View->m_Sprite->m_CurrentFrame--;
	if(m_View->m_Sprite->m_CurrentFrame >=0 && !m_View->m_SelectedSubframe) m_View->m_SelectedSubframe = m_View->m_Sprite->m_Frames[m_View->m_Sprite->m_CurrentFrame]->m_Subframes[0];
	Redraw();
}


//////////////////////////////////////////////////////////////////////////
void CSpritePage::OnAddFrame() 
{
	CString ret = "";
	CBGame* Game = m_View->m_Sprite->Game;

	CString InitFile="";
	if(m_View->m_SelectedSubframe) InitFile = m_View->m_SelectedSubframe->m_Surface->m_Filename;

	// make absolute path
	Game->m_FileManager->RestoreCurrentDir();
	for(int i=0; i<Game->m_FileManager->m_SinglePaths.GetSize(); i++){
		CString NewPath = CString(Game->m_FileManager->m_SinglePaths[i]) + InitFile;
		CFileFind finder;
		if(finder.FindFile(NewPath)){
			InitFile = NewPath;
			break;
		}
	}

	CString Filter = "Image files (*.bmp; *.tga; *.png; *.jpg)|*.bmp;*.tga;*.png;*.jpg|All Files (*.*)|*.*||";
	CFileDialog dlg(TRUE, NULL, InitFile, OFN_ALLOWMULTISELECT|OFN_HIDEREADONLY|OFN_ENABLESIZING|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_NOCHANGEDIR, Filter, this);
	char NameBuffer[32768+1] = "";
	dlg.m_ofn.lpstrFile = NameBuffer;
	dlg.m_ofn.nMaxFile = 32768;
	if(InitFile=="") InitFile = CString(Game->m_FileManager->m_BasePath);
	dlg.m_ofn.lpstrInitialDir = LPCSTR(InitFile);
	
	if(dlg.DoModal()==IDOK){
		m_View->SetUndoState("Add frame(s)");
		POSITION pos = dlg.GetStartPosition();
		while(pos){
			CString filename = dlg.GetNextPathName(pos);
			char* str = new char[filename.GetLength()+1];
			strcpy(str, LPCSTR(filename));
			Game->m_FileManager->MakeRelativePath(str);

			CBSurface* surf = Game->m_SurfaceStorage->AddSurface(str);
			if(surf){
				CBFrame* frame = ::new CBFrame(Game);
				CBSubFrame* subframe = ::new CBSubFrame(Game);
				subframe->m_Surface = surf;
				subframe->SetDefaultRect();
				frame->m_Subframes.Add(subframe);

				int i = m_View->m_Sprite->m_CurrentFrame+1;
				if(i<m_View->m_Sprite->m_Frames.GetSize())
					m_View->m_Sprite->m_Frames.InsertAt(i, frame);
				else{
					m_View->m_Sprite->m_Frames.Add(frame);					
				}
				m_View->m_Sprite->m_CurrentFrame=i;
				m_View->m_SelectedSubframe = subframe;
			}
			else MessageBox("Error loading image \"" + filename + "\".", NULL, MB_OK|MB_ICONERROR);
			delete [] str;
		}
		Redraw();
	}
}


//////////////////////////////////////////////////////////////////////////
void CSpritePage::OnAddSubframe() 
{
	CBFrame* frame = m_View->m_Sprite->m_Frames[m_View->m_Sprite->m_CurrentFrame];

	CString OrigFile;
	if(m_View->m_SelectedSubframe->m_Surface && m_View->m_SelectedSubframe->m_Surface->m_Filename) OrigFile = CString(m_View->m_SelectedSubframe->m_Surface->m_Filename);
	CString NewFile = GetRelativeFilename(frame->Game, OrigFile, "Image Files (*.bmp; *.tga; *.png; *.jpg)|*.bmp;*.tga;*.png;*.jpg", ((CSpriteEditApp*)AfxGetApp())->m_LastDirImage, this);
	if(NewFile!=""){
		m_View->SetUndoState("Add subframe");
		CBSurface* surf = frame->Game->m_SurfaceStorage->AddSurface((char*)LPCSTR(NewFile));
		if(surf){
			int i;
			for(i=0; i<frame->m_Subframes.GetSize(); i++){
				if(frame->m_Subframes[i]==m_View->m_SelectedSubframe) break;
			}
			i++;

			CBSubFrame* subframe = ::new CBSubFrame(frame->Game);
			subframe->m_Surface = surf;
			subframe->SetDefaultRect();

			if(i<frame->m_Subframes.GetSize())
				frame->m_Subframes.InsertAt(i, subframe);
			else
				frame->m_Subframes.Add(subframe);

			frame->m_EditorExpanded = true;
			m_View->m_SelectedSubframe = subframe;

			Redraw();
		}
		else MessageBox("Error loading image \"" + NewFile + "\".", NULL, MB_OK|MB_ICONERROR);		
	}
}


//////////////////////////////////////////////////////////////////////////
void CSpritePage::OnFirstFrame() 
{
	m_View->m_Sprite->m_CurrentFrame = 0;
	m_View->m_SelectedSubframe = m_View->m_Sprite->m_Frames[m_View->m_Sprite->m_CurrentFrame]->m_Subframes[0];
	Redraw();
}


//////////////////////////////////////////////////////////////////////////
void CSpritePage::OnLastFrame() 
{
	m_View->m_Sprite->m_CurrentFrame = m_View->m_Sprite->m_Frames.GetSize()-1;
	m_View->m_SelectedSubframe = m_View->m_Sprite->m_Frames[m_View->m_Sprite->m_CurrentFrame]->m_Subframes[0];
	Redraw();
}


//////////////////////////////////////////////////////////////////////////
void CSpritePage::OnNextFrame() 
{
	m_View->m_Sprite->m_CurrentFrame++;
	if(m_View->m_Sprite->m_CurrentFrame>=m_View->m_Sprite->m_Frames.GetSize()) m_View->m_Sprite->m_CurrentFrame = 0;
	m_View->m_SelectedSubframe = m_View->m_Sprite->m_Frames[m_View->m_Sprite->m_CurrentFrame]->m_Subframes[0];
	Redraw();
}


//////////////////////////////////////////////////////////////////////////
void CSpritePage::OnPrevFrame() 
{
	m_View->m_Sprite->m_CurrentFrame--;
	if(m_View->m_Sprite->m_CurrentFrame<0) m_View->m_Sprite->m_CurrentFrame = m_View->m_Sprite->m_Frames.GetSize()-1;
	m_View->m_SelectedSubframe = m_View->m_Sprite->m_Frames[m_View->m_Sprite->m_CurrentFrame]->m_Subframes[0];
	Redraw();
}


//////////////////////////////////////////////////////////////////////////
void CSpritePage::OnPlayStop() 
{
	if(m_View){
		if(m_View->m_Playing){
			m_View->Stop();			
		}
		else{
			m_View->Play();
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CSpritePage::OnLooping() 
{
	if(m_View){
		m_View->SetUndoState("Change looping");
		m_View->m_Sprite->m_Looping = (m_Looping.GetCheck()!=FALSE);
		((CMainFrame*)AfxGetMainWnd())->m_wndWorkSpace.Refresh();
	}
}



