// SpriteView.cpp : implementation of the CSpriteView class
//

#include "stdafx.h"
#include "SpriteEdit.h"

#include "SpriteDoc.h"
#include "SpriteView.h"
#include "MainFrm.h"
#include "BgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpriteView

IMPLEMENT_DYNCREATE(CSpriteView, CScrollView)

BEGIN_MESSAGE_MAP(CSpriteView, CScrollView)
	//{{AFX_MSG_MAP(CSpriteView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_TOOLS_REFRESH, OnToolsRefresh)
	ON_COMMAND(ID_VIEW_BACKGROUND, OnViewBackground)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BACKGROUND, OnUpdateViewBackground)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_FPS, OnDisplayFPS)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_MODE, OnDisplayMode)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_CURSOR, OnDisplayCursor)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpriteView construction/destruction


//////////////////////////////////////////////////////////////////////////
CSpriteView::CSpriteView()
{
	Game = ::new CBGame;
	Game->m_EditorMode = true;
	Game->m_DoNotExpandStrings = true;
	Game->m_Registry->SetBasePath(SPRITEEDIT_REG_PATH);
	Game->m_EditorForceScripts = true;

	m_Object = ::new CBObject(Game);
	m_Object->m_Movable = true;
	m_Object->m_PosX = m_Object->m_PosY = 0;

	m_Sprite = ::new CBSprite(Game, m_Object);
	m_Sprite->m_EditorAllFrames = true;

	m_Background = NULL;

	
	m_UndoPointer = -1;

	m_ViewReady = false;

	m_SelectedSubframe = NULL;
	m_CumulateMove = true;

	m_HotRect.SetRectEmpty();

	m_Dragging = false;
	m_DragFrom = m_DragOrigin = CPoint(0, 0);
	m_DragMove = false;

	m_Playing = false;

	m_LastSelectedFrame = 0;
}


//////////////////////////////////////////////////////////////////////////
CSpriteView::~CSpriteView()
{
	for(int i=0; i<m_UndoStates.GetSize(); i++) delete m_UndoStates[i];
	m_UndoStates.RemoveAll();

	SAFE_DELETE(m_Background);
	SAFE_DELETE(m_Object);
	SAFE_DELETE(m_Sprite);
	SAFE_DELETE(Game);
}


//////////////////////////////////////////////////////////////////////////
BOOL CSpriteView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSpriteView drawing

//////////////////////////////////////////////////////////////////////////
void CSpriteView::OnDraw(CDC* pDC)
{
	CSpriteDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CSize size;
	GetScrollSize(size);

	int mode;
	CSize OldSize, PageSize, LineSize;
	GetDeviceScrollSizes(mode, OldSize, PageSize, LineSize);

	if(size!=OldSize) SetScrollSizes(MM_TEXT, size);

	if(Game && Game->m_Renderer) RenderFrame();
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// setup current directory
	CSpriteDoc* doc = NULL;
	if(GetDocument() && GetDocument()->IsKindOf(RUNTIME_CLASS(CSpriteDoc))){
		doc = (CSpriteDoc*)GetDocument();
		SetCurrentDirectory(doc->m_ProjectRoot);
	}


	CRect rc;
	GetClientRect(&rc);

	Game->DEBUG_DebugEnable(".\\SpriteEdit.log");

	Game->m_UseD3D = Game->m_Registry->ReadBool("Options", "Accelerated", true);
	Game->Initialize1();
	if(!Game->IsDirectXBound())
	{
		MessageBox("This program requires DirectX 8.0 or later.", NULL, MB_ICONERROR);
		return;
	}
	
	Game->Initialize2();

	Game->m_Renderer->m_Windowed = true;
	Game->m_Renderer->m_Instance = AfxGetInstanceHandle();


	// set game's base path
	if(doc){
		CFileFind finder;
		BOOL working = finder.FindFile(doc->m_ProjectRoot + "*.wpr");
		while(working){
			working = finder.FindNextFile();
			if(!finder.IsDots() && !finder.IsDirectory()){
				Game->m_Registry->SetIniName((char*)LPCSTR(finder.GetFilePath()));
				break;
			}
		}
		
		Game->m_FileManager->SetBasePath((char*)LPCSTR(doc->m_ProjectRoot));		
	}


	CHWManager* m = new CHWManager(Game);
	m->m_Force16bitTextures = Game->m_Registry->ReadBool("Options", "Force16bitTextures", false);
	m->m_ForceTextureSubdivision = Game->m_Registry->ReadBool("Options", "ForceTextureSubdivision", false);
	
	CWaitCursor wait;
	m->AutoselectWindowedDevice(Game->m_UseD3D, !Game->m_UseD3D, AfxGetMainWnd()->GetSafeHwnd());


	Game->InitInput(AfxGetInstanceHandle(), AfxGetMainWnd()->GetSafeHwnd());
	Game->m_Renderer->m_Window = GetSafeHwnd();
	Game->m_Renderer->m_ClipperWindow = GetSafeHwnd();
	Game->m_Renderer->m_Ready = (Game->m_Renderer->InitRenderer(m)==S_OK); 

	if(!Game->m_Renderer->m_Ready && Game->m_UseD3D){
		MessageBox("Error initializing accelerated renderer. Sticking back to software...", NULL, MB_OK|MB_ICONWARNING);
		SAFE_DELETE(Game->m_Renderer);
		Game->m_UseD3D = false;
		Game->Initialize2();

		//p->m_Accelerated = false;
		wait.Restore();
		m->AutoselectWindowedDevice(false, true, AfxGetMainWnd()->GetSafeHwnd());

		Game->m_Renderer->m_Window = GetSafeHwnd();
		Game->m_Renderer->m_ClipperWindow = GetSafeHwnd();
		Game->m_Renderer->m_Ready = (Game->m_Renderer->InitRenderer(m)==S_OK); 
	}

	// initialize sound manager
	HRESULT ret = Game->m_SoundMgr->Initialize(GetSafeHwnd(), m);
	if(FAILED(ret)){
		Game->LOG(ret, "Sound is NOT available.");
	}

	delete m;


	Game->m_Renderer->m_Ready = false;
	Game->m_SystemFont = Game->m_FontStorage->AddFont("system_font.fnt");
		
	CString Filename;
	Filename = GetDocument()->GetPathName();

	if(Filename!=""){
		Game->m_DEBUG_AbsolutePathWarning = false;
		Game->LOG(0, "----- Open file: '%s' -----", LPCSTR(Filename));
		if(FAILED(m_Sprite->LoadFile((char*)LPCSTR(Filename)))){
			MessageBox("There were errors loading the sprite. View log for details.", NULL, MB_OK|MB_ICONWARNING);
		}
		Game->m_DEBUG_AbsolutePathWarning = true;
		GetSelectedObjects();
		ReloadBackground();
	}
	else{
		// new
	}


	CSize sizeTotal;
	// TODO: calculate the total size of this view

	GetScrollSize(sizeTotal);	
	
	SetScrollSizes(MM_TEXT, sizeTotal);


	Game->m_Renderer->m_Ready = true;
}


/////////////////////////////////////////////////////////////////////////////
// CSpriteView diagnostics

#ifdef _DEBUG
void CSpriteView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CSpriteView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CSpriteDoc* CSpriteView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSpriteDoc)));
	return (CSpriteDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSpriteView message handlers

void CSpriteView::OnContextMenu(CWnd*, CPoint point)
{
	theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	if(m_nMapMode==0) return;
	CScrollView::OnPrepareDC(pDC, pInfo);
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);

	if(m_nMapMode==0) return;
	if(!Game || !Game->m_Renderer) return;
	

	if(Game->m_UseD3D){
		CBRenderD3D* rend = (CBRenderD3D*)Game->m_Renderer;
		if(rend->m_Ready){
			rend->m_Ready = false;
			rend->m_Width = rend->m_RealWidth = cx;
			rend->m_Height = rend->m_RealHeight = cy;
			rend->m_Ready = SUCCEEDED(rend->ResetDevice());
		}
	}
	else{
		CBRenderDD* rend = (CBRenderDD*)Game->m_Renderer;

		if(rend->m_DirectDraw!=NULL){	
			rend->ReleaseAll();
			rend->m_Ready = (rend->InitSurfaces()==DD_OK);
		}
	}

	if(!m_ViewReady){
		int mode;
		CSize ViewSize, PageSize, LineSize;
		GetDeviceScrollSizes(mode, ViewSize, PageSize, LineSize);


		CRect client;
		GetClientRect(&client);
		ScrollToPosition(CPoint((ViewSize.cx - client.Width()) / 2, (ViewSize.cy - client.Height()) / 2));
	}
}


//////////////////////////////////////////////////////////////////////////
BOOL CSpriteView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}




//////////////////////////////////////////////////////////////////////////
HRESULT CSpriteView::RenderFrame()
{
	m_HotRect.SetRectEmpty();
	
	HRESULT ret = S_OK;
	if (Game->m_Renderer->m_Windowed && Game->m_Renderer->m_Ready){

		m_ViewReady = true;

		// init loop
		Game->m_Renderer->OnWindowChange();
		Game->InitLoop();		

		if(FAILED(Game->m_Renderer->Fill(0, 0, 0))) return E_FAIL;

		CRect ClientRect;
		GetClientRect(&ClientRect);

		
		CPoint p = GetScrollPosition();
		int mode;
		CSize ViewSize, PageSize, LineSize;
		GetDeviceScrollSizes(mode, ViewSize, PageSize, LineSize);

		// get move
		int MoveX=0, MoveY=0;

		if(m_Sprite && m_Sprite->m_CurrentFrame >= 0 && m_Sprite->m_CurrentFrame < m_Sprite->m_Frames.GetSize()){
			if(m_CumulateMove){
				for(int i=0; i<=m_Sprite->m_CurrentFrame; i++){
					MoveX += m_Sprite->m_Frames[i]->m_MoveX;
					MoveY += m_Sprite->m_Frames[i]->m_MoveY;
				}
			}
			else{
				MoveX = m_Sprite->m_Frames[m_Sprite->m_CurrentFrame]->m_MoveX;
				MoveY = m_Sprite->m_Frames[m_Sprite->m_CurrentFrame]->m_MoveY;
			}
		}

		// get hotspot pos
		int hx = ViewSize.cx / 2 - p.x;
		int hy = ViewSize.cy / 2 - p.y;

		if(m_Sprite && m_Background) m_Background->Draw(-p.x + m_Sprite->m_EditorBgOffsetX, -p.y + m_Sprite->m_EditorBgOffsetY, NULL, 100, 100, DRGBA(0xFF, 0xFF, 0xFF, (BYTE)m_Sprite->m_EditorBgAlpha));


		if(m_Playing){
			if(m_Sprite){
				m_Sprite->GetCurrentFrame();
				if(m_Sprite->m_Changed){
					m_Object->m_PosX += m_Sprite->m_MoveX;
					m_Object->m_PosY += m_Sprite->m_MoveY;
				}
				if(m_Object->m_PosX < (0-ViewSize.cx/2) || m_Object->m_PosX>(0+ViewSize.cx/2) || m_Object->m_PosY < (0-ViewSize.cy/2) || m_Object->m_PosY > (0+ViewSize.cy/2)){
					m_Object->m_PosX = m_Object->m_PosY = 0;
				}

				m_Sprite->Display(hx+m_Object->m_PosX, hy+m_Object->m_PosY);

				if(m_Sprite->m_Finished) Stop();
			}
		}
		else{
			if(m_Sprite) m_Sprite->Display(hx + MoveX, hy + MoveY);


			Game->m_Renderer->SetupLines();

			// draw tracker
			if(m_SelectedSubframe){
				DrawTracker(hx + MoveX, hy + MoveY);
			}

			// draw move base
			if(MoveX!=0 || MoveY!=0){
				DrawPoint(hx+MoveX-m_Sprite->m_Frames[m_Sprite->m_CurrentFrame]->m_MoveX, hy+MoveY-m_Sprite->m_Frames[m_Sprite->m_CurrentFrame]->m_MoveY, 0x880000FF);
				DrawPoint(hx, hy, 0xFF0000FF);
			}


			// draw hotspot
			DrawPoint(hx + MoveX, hy + MoveY, 0xFFFF0000);
		}


		// display quick messages
		Game->m_Renderer->Setup2D();		
		Game->DisplayQuickMsg();

		// flip
		ret = Game->m_Renderer->Flip();	

		CString str;
		str.Format("FPS: %d", Game->m_Fps);
		((CMainFrame*)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(3, str);
	}

	return ret;
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::Undo()
{
	CWaitCursor wait;

	BYTE* buf = m_UndoStates[m_UndoPointer]->GetData(true);
	if(buf && SUCCEEDED(m_Sprite->LoadBuffer(buf, true))){
		GetSelectedObjects();
		ReloadBackground();
		PurgeLastUndo();

		((CMainFrame*)AfxGetMainWnd())->m_wndWorkSpace.Refresh();
	}
	else MessageBox("Cannot undo changes", "Error", MB_OK|MB_ICONERROR);
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::PurgeLastUndo()
{
	if(m_UndoPointer < m_UndoStates.GetSize()){
		delete m_UndoStates[m_UndoPointer];
		m_UndoStates.RemoveAt(m_UndoPointer);
		m_UndoPointer--;
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT CSpriteView::SetUndoState(CString Name)
{
	//MessageBox(Name);
	UpdateSelectionState();
	CBDynBuffer* buf = new CBDynBuffer(Game);
	m_Sprite->SaveAsText(buf, 0);
	CUndoState* state = new CUndoState;
	if(FAILED(state->SetData(buf->m_Buffer, buf->GetSize(), Name))){
		delete state;
		delete buf;
		return E_FAIL;
	}

	m_UndoPointer++;
	m_UndoStates.SetAtGrow(m_UndoPointer, state);

	SetDirty();

	delete buf;
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::OnEditUndo() 
{
	Undo();	
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_UndoPointer>=0);
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::SetDirty(bool IsDirty)
{
	if(IsDirty) GetDocument()->SetModifiedFlag(TRUE);
	else GetDocument()->SetModifiedFlag(FALSE);
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::OnDisplayFPS(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	if (!((CSpriteEditApp*)AfxGetApp())->m_Active) pCmdUI->SetText("inactive");
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::OnDisplayCursor(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	CString str;
	CPoint point;
	CRect rect;

	CPoint p = GetScrollPosition();
	int mode;
	CSize ViewSize, PageSize, LineSize;
	GetDeviceScrollSizes(mode, ViewSize, PageSize, LineSize);
	int hx = ViewSize.cx / 2 - p.x;
	int hy = ViewSize.cy / 2 - p.y;


	GetCursorPos(&point);
	ScreenToClient(&point);

	GetClientRect(&rect);

	if(rect.PtInRect(point)){		
		//point += GetDeviceScrollPosition();
		str.Format("%d, %d", point.x - hx, point.y - hy);
		pCmdUI->SetText(str);
	}
	else pCmdUI->SetText("");
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::OnDisplayMode(CCmdUI *pCmdUI)
{
	CString str;
	CRect rc;
	GetClientRect(&rc);
	str.Format("Window: %dx%d (%s)", rc.Width(), rc.Height(), Game->m_UseD3D?"accelerated":"software");
	pCmdUI->SetText(str);
	
	pCmdUI->Enable();
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	if(pActivateView==pDeactiveView && pActivateView==this){
		((CMainFrame*)AfxGetMainWnd())->m_wndWorkSpace.ActivateSpriteView(bActivate?this:NULL);
	}
	
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


//////////////////////////////////////////////////////////////////////////
int CSpriteView::SelectSubframe(CBSubFrame *Sub)
{
	if(!m_Sprite) return -1;

	for(int i=0; i<m_Sprite->m_Frames.GetSize(); i++){
		for(int j=0; j<m_Sprite->m_Frames[i]->m_Subframes.GetSize(); j++){
			if(m_Sprite->m_Frames[i]->m_Subframes[j] == Sub){
				m_Sprite->m_CurrentFrame = i;
				m_SelectedSubframe = Sub;
				return i;
			}
		}
	}

	m_SelectedSubframe = NULL;
	m_Sprite->m_CurrentFrame = -1;
	return -1;
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::UpdateSelectionState()
{
	for(int i=0; i<m_Sprite->m_Frames.GetSize(); i++){
		for(int j=0; j<m_Sprite->m_Frames[i]->m_Subframes.GetSize(); j++){
			m_Sprite->m_Frames[i]->m_Subframes[j]->m_EditorSelected = m_Sprite->m_Frames[i]->m_Subframes[j] == m_SelectedSubframe;
		}
	}

}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::GetSelectedObjects()
{
	m_SelectedSubframe = NULL;
	if(!m_Sprite) return;

	for(int i=0; i<m_Sprite->m_Frames.GetSize(); i++){
		for(int j=0; j<m_Sprite->m_Frames[i]->m_Subframes.GetSize(); j++){
			if(m_Sprite->m_Frames[i]->m_Subframes[j]->m_EditorSelected) SelectSubframe(m_Sprite->m_Frames[i]->m_Subframes[j]);
		}
	}

	if(!m_SelectedSubframe && m_Sprite->m_Frames.GetSize() > 0){
		if(m_Sprite->m_Frames[0]->m_Subframes.GetSize() > 0){
			SelectSubframe(m_Sprite->m_Frames[0]->m_Subframes[0]);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::SetExpand(CBSubFrame *Sub, bool Expand)
{
	if(!m_Sprite) return;

	for(int i=0; i<m_Sprite->m_Frames.GetSize(); i++){
		for(int j=0; j<m_Sprite->m_Frames[i]->m_Subframes.GetSize(); j++){
			if(m_Sprite->m_Frames[i]->m_Subframes[j] == Sub){
				m_Sprite->m_Frames[i]->m_EditorExpanded = Expand;

				return;
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::DrawTracker(int X, int Y)
{
	if(!m_SelectedSubframe) return;
	
	int x, y, w, h;

	x = m_SelectedSubframe->m_Rect.left - m_SelectedSubframe->m_HotspotX + X;
	y = m_SelectedSubframe->m_Rect.top - m_SelectedSubframe->m_HotspotY + Y;
	w = m_SelectedSubframe->m_Rect.right - m_SelectedSubframe->m_Rect.left;
	h = m_SelectedSubframe->m_Rect.bottom - m_SelectedSubframe->m_Rect.top;

	x--;
	y--;
	w++;
	h++;


	Game->m_Renderer->DrawRect(x, y, x+w, y+h, 0xFFFF0000);

	m_HotRect.SetRect(CPoint(x, y), CPoint(x+w, y+h));
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::DrawPoint(int X, int Y, DWORD Color)
{
	Game->m_Renderer->DrawLine(X - 4, Y,     X + 5, Y,     Color);
	Game->m_Renderer->DrawLine(X,     Y - 4, X,     Y + 5, Color);
}


//////////////////////////////////////////////////////////////////////////
BOOL CSpriteView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(&p);

	if(m_HotRect.PtInRect(p)){
		SetCursor(LoadCursor(NULL, IDC_SIZEALL));
		return TRUE;
	}
	
	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_SelectedSubframe && m_HotRect.PtInRect(point)){
		SetCapture();
		m_Dragging = true;
		m_DragFrom = m_DragOrigin = point;

		SetUndoState("Move");
	}
	
	CScrollView::OnLButtonDown(nFlags, point);
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_Dragging) EndDrag();
	
	CScrollView::OnLButtonUp(nFlags, point);
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_Dragging){
		UpdateDrag(m_DragFrom, point);
	}
	
	CScrollView::OnMouseMove(nFlags, point);
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	if(m_Dragging){
		UpdateDrag(m_DragFrom, m_DragOrigin);
		EndDrag();
		PurgeLastUndo();
	}
	else CScrollView::OnRButtonUp(nFlags, point);
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::EndDrag()
{
	ReleaseCapture();
	m_Dragging = false;

	((CMainFrame*)AfxGetMainWnd())->m_wndProperty.ShowFrame(this, m_Sprite->m_Frames[m_Sprite->m_CurrentFrame], m_SelectedSubframe, true);
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::UpdateDrag(CPoint Origin, CPoint Current)
{
	int xdiff = 0;
	int ydiff = 0;
	
	if(Current.x != 0 && Current.y != 0){
		xdiff = Current.x - Origin.x;
		ydiff = Current.y - Origin.y;
	}
	
	if(m_DragMove){
		m_Sprite->m_Frames[m_Sprite->m_CurrentFrame]->m_MoveX += xdiff;
		m_Sprite->m_Frames[m_Sprite->m_CurrentFrame]->m_MoveY += ydiff;
	}
	else{
		m_SelectedSubframe->m_HotspotX -= xdiff;
		m_SelectedSubframe->m_HotspotY -= ydiff;
	}
	
	m_DragFrom = Current;
	
	((CMainFrame*)AfxGetMainWnd())->m_wndProperty.ShowFrame(this, m_Sprite->m_Frames[m_Sprite->m_CurrentFrame], m_SelectedSubframe, true);
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::OnToolsRefresh() 
{
	CWaitCursor wait;
	SetUndoState("Temporary");
	Undo();
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::Play()
{
	m_Playing = true;

	m_LastSelectedFrame = m_Sprite->m_CurrentFrame;

	m_Object->m_PosX = m_Object->m_PosY = 0;
	m_Sprite->m_Finished = false;
	m_Sprite->m_LastFrameTime = 0;
	((CMainFrame*)AfxGetMainWnd())->m_wndWorkSpace.Refresh();
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::Stop()
{
	m_Playing = false;

	m_Sprite->m_CurrentFrame = m_LastSelectedFrame;
	m_SelectedSubframe = m_Sprite->m_Frames[m_Sprite->m_CurrentFrame]->m_Subframes[0];
	((CMainFrame*)AfxGetMainWnd())->m_wndWorkSpace.Refresh();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CSpriteView::ReloadBackground()
{
	SAFE_DELETE(m_Background);
	if(m_Sprite && m_Sprite->m_EditorBgFile){
		m_Background = ::new CBSprite(Game);
		if(!m_Background || FAILED(m_Background->LoadFile(m_Sprite->m_EditorBgFile))){
			Game->LOG(0, "Error loading background image '%s'", m_Sprite->m_EditorBgFile);
			SAFE_DELETE(m_Background);
			return E_FAIL;
		}
	}
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::GetScrollSize(CSize &Size)
{
	Size.cx = Size.cy = 0;

	if(m_Sprite && m_Background){
		RECT rect;
		m_Background->GetBoundingRect(&rect, 0, 0);
		Size.cx = /*2*100*/ - m_Sprite->m_EditorBgOffsetX + (rect.right - rect.left);
		Size.cy = /*2*100*/ - m_Sprite->m_EditorBgOffsetY + (rect.bottom - rect.top);
	}
	
	Size.cx = max(Size.cx, 1000);
	Size.cy = max(Size.cy, 1000);
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::OnViewBackground() 
{
	CBgDlg dlg;
	dlg.m_Game = Game;
	dlg.m_Alpha = m_Sprite->m_EditorBgAlpha;
	dlg.m_OffsetX = m_Sprite->m_EditorBgOffsetX;
	dlg.m_OffsetY = m_Sprite->m_EditorBgOffsetY;
	if(m_Sprite->m_EditorBgFile)
		dlg.m_Filename = CString(m_Sprite->m_EditorBgFile);
	else
		dlg.m_Filename = "";

	if(dlg.DoModal()==IDOK){
		SetUndoState("Change background image");
		m_Sprite->m_EditorBgAlpha = dlg.m_Alpha;
		m_Sprite->m_EditorBgOffsetX = dlg.m_OffsetX;
		m_Sprite->m_EditorBgOffsetY = dlg.m_OffsetY;
		
		SAFE_DELETE_ARRAY(m_Sprite->m_EditorBgFile);
		if(dlg.m_Filename!=""){
			m_Sprite->m_EditorBgFile = new char[dlg.m_Filename.GetLength()+1];
			strcpy(m_Sprite->m_EditorBgFile, LPCSTR(dlg.m_Filename));
		}
		ReloadBackground();
	}	
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::OnUpdateViewBackground(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_Sprite!=NULL);
}


//////////////////////////////////////////////////////////////////////////
void CSpriteView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int MoveX=0, MoveY=0;

	switch(nChar){
		case VK_LEFT:  MoveX = -1; break;
		case VK_RIGHT: MoveX = 1;  break;
		case VK_UP:    MoveY = -1; break;
		case VK_DOWN:  MoveY = 1;  break;
	}

	if((GetAsyncKeyState(VK_SHIFT) & 0x8000)!=0){
		MoveX *= 5;
		MoveY *= 5;
	}

	if(MoveX == 0 && MoveY == 0){
		CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}

	if(m_SelectedSubframe){
		SetUndoState("Change hotspot");
		m_SelectedSubframe->m_HotspotX -= MoveX;
		m_SelectedSubframe->m_HotspotY -= MoveY;
		((CMainFrame*)AfxGetMainWnd())->m_wndWorkSpace.Refresh();
	}
	
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}
