// SceneView.cpp : implementation of the CSceneView class
//

#include "stdafx.h"
#include "SceneEdit.h"

#include "SceneDoc.h"
#include "SceneView.h"
#include "MainFrm.h"
#include "PointPosDlg.h"
#include "AnimDlg.h"
#include "ScaleProp.h"
#include "RotProp.h"
#include "resource.h"
#include "../mfc_shared/utils_mfc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSceneView

IMPLEMENT_DYNCREATE(CSceneView, CScrollView)

BEGIN_MESSAGE_MAP(CSceneView, CScrollView)
	//{{AFX_MSG_MAP(CSceneView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_TOOLS_REFRESH, OnToolsRefresh)
	ON_COMMAND(ID_SCALE_PROPERTIES, OnScaleProperties)
	ON_COMMAND(ID_ROT_PROPERTIES, OnRotProperties)
	ON_COMMAND(ID_SCALE_REMOVE, OnScaleRemove)
	ON_COMMAND(ID_ROT_REMOVE, OnRotRemove)
	ON_COMMAND(ID_ACTOR_LOAD, OnActorLoad)
	ON_COMMAND(ID_ACTOR_ANIMATION, OnActorAnim)
	ON_COMMAND(ID_ACTOR_PLACE, OnActorPlace)
	ON_COMMAND(ID_ACTOR_REMOVE, OnActorRemove)
	ON_UPDATE_COMMAND_UI(ID_ACTOR_REMOVE, OnUpdateActorRemove)
	ON_UPDATE_COMMAND_UI(ID_ACTOR_ANIMATION, OnUpdateActorAnim)
	ON_COMMAND(ID_ACTOR_COPY_DATA, OnActorCopyData)
	ON_UPDATE_COMMAND_UI(ID_ACTOR_COPY_DATA, OnUpdateActorCopyData)
	ON_WM_KEYDOWN()
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_FPS, OnDisplayFPS)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_MODE, OnDisplayMode)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_CURSOR, OnDisplayCursor)
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSceneView construction/destruction

CSceneView::CSceneView()
{
	Game = ::new CAdGame;
	Game->m_EditorMode = true;
	Game->m_DoNotExpandStrings = true;
	
	m_SelectedLayer = NULL;
	m_SelectedNode = NULL;

	m_SelectedWaypoints = false;
	m_SelectedWptGroup = NULL;

	m_SelectedFree = false;
	m_SelectedFreeObject = NULL;


	m_GameWidth = 800; // TODO
	m_GameHeight = 600;

	m_Dragging = false;
	m_DragFrom = m_DragOrigin = CPoint(0, 0);
	m_DragObject = NULL;

	m_AddingPoint = m_RemovingPoint = m_AddingScale = m_AddingRot = m_PlacingActor = false;

	m_UndoPointer = -1;

	m_SelectedScale = NULL;
	m_SelectedRot = NULL;

	m_Actor = NULL;
	m_Actor3D = NULL;
	m_Actor3DX = NULL;

}



//////////////////////////////////////////////////////////////////////////
CSceneView::~CSceneView()
{
	for(int i=0; i<m_UndoStates.GetSize(); i++) delete m_UndoStates[i];
	m_UndoStates.RemoveAll();

	ClearHotObjects();
	if(m_Actor) Game->RemoveObject(m_Actor);
	m_Actor = NULL;

	if(m_Actor3D) Game->RemoveObject(m_Actor3D);
	m_Actor3D = NULL;

	if(m_Actor3DX) Game->RemoveObject(m_Actor3DX);
	m_Actor3DX = NULL;

	delete Game;
}


//////////////////////////////////////////////////////////////////////////
BOOL CSceneView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSceneView drawing
void CSceneView::OnDraw(CDC* pDC)
{
	CSceneDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	Game->m_Scene->m_ParalaxScrolling = false;
	Game->m_Scene->m_AutoScroll = false;

	CSize size(m_GameWidth, m_GameHeight);
	if(Game && Game->m_Scene){
		if(Game->m_Scene->m_ParalaxScrolling){
			// TODO: set game's resolution here
		}
		else{
			for(int i=0; i<Game->m_Scene->m_Layers.GetSize(); i++){
				if(!Game->m_Scene->m_Layers[i]->m_Active) continue;

				size.cx = max(size.cx, Game->m_Scene->m_Layers[i]->m_Width + 2*Game->m_Scene->m_EditorMarginH);
				size.cy = max(size.cy, Game->m_Scene->m_Layers[i]->m_Height + 2*Game->m_Scene->m_EditorMarginV);
			}
		}

		int mode;
		CSize OldSize, PageSize, LineSize;
		GetDeviceScrollSizes(mode, OldSize, PageSize, LineSize);

		if(size!=OldSize) SetScrollSizes(MM_TEXT, size);


		CPoint p = GetScrollPosition();
		//Game->m_Scene->m_OffsetLeft = p.x - Game->m_Scene->m_EditorMarginH;
		//Game->m_Scene->m_OffsetTop = p.y  - Game->m_Scene->m_EditorMarginV;
		Game->m_Scene->SetOffset(p.x - Game->m_Scene->m_EditorMarginH, p.y  - Game->m_Scene->m_EditorMarginV);
	}


	if(Game && Game->m_Renderer) RenderFrame();
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// setup current directory
	CSceneDoc* doc = NULL;
	if(GetDocument() && GetDocument()->IsKindOf(RUNTIME_CLASS(CSceneDoc))){
		doc = (CSceneDoc*)GetDocument();
		SetCurrentDirectory(doc->m_ProjectRoot);
	}


	CRect rc;
	GetClientRect(&rc);



	Game->m_Registry->SetBasePath(SCENEEDIT_REG_PATH);
	Game->m_EditorForceScripts = true;

	Game->DEBUG_DebugEnable(".\\SceneEdit.log");


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
/*
	m->m_SelectedWindowed = true;
	m->m_Selected16bitColors = true;
	dev->m_AdapterID = D3DADAPTER_DEFAULT;
	dev->m_Accelerated = Game->m_UseD3D;
	m->m_ReqWidth = rc.right - rc.left;
	m->m_ReqHeight = rc.bottom - rc.top;
	m->m_ReqWindowedOnly = true;
	memset(&dev->m_Guid, 0, sizeof(GUID));
*/

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
	

	Game->m_SystemFont = Game->m_FontStorage->AddFont("outline_green.fnt");
	
	
	CString Filename;
	Filename = GetDocument()->GetPathName();

	if(Filename!=""){
		Game->LOG(0, "----- Open file: '%s' -----", LPCSTR(Filename));
		Game->m_DEBUG_AbsolutePathWarning = false;
		Game->m_Scene->LoadFile((char*)LPCSTR(Filename));
		Game->m_DEBUG_AbsolutePathWarning = true;
		RegisterAllObjects();
		GetSelectedObjects();

		Game->m_EditorResolutionWidth = Game->m_Scene->m_EditorResolutionWidth;
		Game->m_EditorResolutionHeight = Game->m_Scene->m_EditorResolutionHeight;

		//Game->m_Scene->m_ShowGeometry = true;
	}
	else{
		CAdLayer* layer = ::new CAdLayer(Game);
		layer->SetName("main");
		layer->m_Width = 800;
		layer->m_Height = 600;
		layer->m_Main = true;
		Game->m_Scene->m_Layers.Add(layer);
		Game->m_Scene->m_MainLayer = layer;
	}

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 10000;
	SetScrollSizes(MM_TEXT, sizeTotal);

	ScrollToPosition(CPoint(Game->m_Scene->m_EditorMarginH, Game->m_Scene->m_EditorMarginV));

	Game->m_Renderer->m_Ready = true;
}

/////////////////////////////////////////////////////////////////////////////
// CSceneView printing

//////////////////////////////////////////////////////////////////////////
void CSceneView::OnFilePrintPreview() 
{
	BCGPrintPreview (this);
}


//////////////////////////////////////////////////////////////////////////
BOOL CSceneView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


/////////////////////////////////////////////////////////////////////////////
// CSceneView diagnostics

#ifdef _DEBUG
void CSceneView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CSceneView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CSceneDoc* CSceneView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSceneDoc)));
	return (CSceneDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSceneView message handlers

void CSceneView::OnContextMenu(CWnd*, CPoint point)
{
	theApp.ShowPopupMenu (IDR_CONTEXT_MENU, point, this);
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::OnSize(UINT nType, int cx, int cy) 
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
}


//////////////////////////////////////////////////////////////////////////
BOOL CSceneView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CSceneView::RenderFrame()
{
	HRESULT ret = S_OK;
	if (Game->m_Renderer->m_Windowed && Game->m_Renderer->m_Ready){
		// init loop
		

		Game->m_Renderer->OnWindowChange();
		Game->InitLoop();		

		if(FAILED(Game->m_Renderer->Fill(0, 0, 0))) return E_FAIL;

		Game->m_Scene->Update();
		Game->m_Scene->Display();

		CRect ClientRect;
		GetClientRect(&ClientRect);

		CPoint p = GetScrollPosition();


		int i;

		if(!m_Dragging) ClearHotObjects();

		// draw regions
		Game->m_Renderer->SetupLines();
		if(Game->m_Scene->m_MainLayer && Game->m_Scene->m_MainLayer->m_Active){
			// draw regions
			for(i=0; i<Game->m_Scene->m_MainLayer->m_Nodes.GetSize(); i++){
				if(Game->m_Scene->m_MainLayer->m_Nodes[i]==m_SelectedNode) continue;
				if(Game->m_Scene->m_MainLayer->m_Nodes[i]->m_Type==OBJECT_REGION && Game->m_Scene->m_MainLayer->m_Nodes[i]->m_Region->m_Active) DrawRegion(Game->m_Scene->m_MainLayer->m_Nodes[i], Game->m_Scene->m_MainLayer->m_Nodes[i]->m_Region, false, Game->m_Scene->m_MainLayer->m_Nodes[i]->m_Region->m_Blocked, Game->m_Scene->m_MainLayer->m_Nodes[i]->m_Region->m_Decoration);
				else if(Game->m_Scene->m_MainLayer->m_Nodes[i]->m_Type==OBJECT_ENTITY && Game->m_Scene->m_MainLayer->m_Nodes[i]->m_Entity->m_Active && Game->m_Scene->m_MainLayer->m_Nodes[i]->m_Entity->m_Region) DrawEntity(Game->m_Scene->m_MainLayer->m_Nodes[i], Game->m_Scene->m_MainLayer->m_Nodes[i]->m_Entity, false);
			}
			// selected comes last
			if(m_SelectedNode && m_SelectedNode->m_Type==OBJECT_REGION) DrawRegion(m_SelectedNode, m_SelectedNode->m_Region, true, m_SelectedNode->m_Region->m_Blocked, m_SelectedNode->m_Region->m_Decoration);
		}
		if(m_SelectedNode && m_SelectedNode->m_Type==OBJECT_ENTITY) DrawEntity(m_SelectedNode, m_SelectedNode->m_Entity, true);

		if(m_SelectedFreeObject && m_SelectedFreeObject->m_Type==OBJECT_ENTITY) DrawEntity(NULL, (CAdEntity*)m_SelectedFreeObject, true);

		// draw waypoints
		for(i=0; i<Game->m_Scene->m_WaypointGroups.GetSize(); i++){
			if(Game->m_Scene->m_WaypointGroups[i] == m_SelectedWptGroup) continue;
			DrawWaypoints(Game->m_Scene->m_WaypointGroups[i], false);
		}
		if(m_SelectedWptGroup) DrawWaypoints(m_SelectedWptGroup, true);

		// draw bounding rectangle
		CSize size = GetTotalSize();
		Game->m_Renderer->DrawRect(Game->m_Scene->m_EditorMarginH - 2 - p.x, Game->m_Scene->m_EditorMarginV - 2 - p.y, size.cx - Game->m_Scene->m_EditorMarginH - p.x + 1, size.cy - Game->m_Scene->m_EditorMarginV - p.y + 1, Game->m_Scene->m_EditorColFrame, 2);

		// draw rotation levels
		for(i=0; i<Game->m_Scene->m_RotLevels.GetSize(); i++){
			DrawRot(Game->m_Scene->m_RotLevels[i]);
		}

		// draw scale levels
		for(i=0; i<Game->m_Scene->m_ScaleLevels.GetSize(); i++){
			DrawScale(Game->m_Scene->m_ScaleLevels[i]);
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
void CSceneView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	if(m_nMapMode==0) return;
	
	CScrollView::OnPrepareDC(pDC, pInfo);
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	if(pActivateView==pDeactiveView && pActivateView==this){
		((CMainFrame*)AfxGetMainWnd())->m_wndWorkSpace.ActivateSceneView(bActivate?this:NULL);
	}
	
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::OnDisplayFPS(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	if (!((CSceneEditApp*)AfxGetApp())->m_Active) pCmdUI->SetText("inactive");
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::OnDisplayCursor(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	CString str;
	CPoint point;
	CRect rect;

	GetCursorPos(&point);
	ScreenToClient(&point);

	GetClientRect(&rect);

	if(rect.PtInRect(point)){		
		point += GetDeviceScrollPosition();
		str.Format("%d, %d", point.x - Game->m_Scene->m_EditorMarginH, point.y - Game->m_Scene->m_EditorMarginV);
		pCmdUI->SetText(str);
	}
	else pCmdUI->SetText("");
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::OnDisplayMode(CCmdUI *pCmdUI)
{
	CString str;
	CRect rc;
	GetClientRect(&rc);
	str.Format("Window: %dx%d (%s)", rc.Width(), rc.Height(), Game->m_UseD3D?"accelerated":"software");
	pCmdUI->SetText(str);
	
	pCmdUI->Enable();
}



//////////////////////////////////////////////////////////////////////////
void CSceneView::DrawRegion(CAdSceneNode* Node, CBRegion *Region, bool Selected, bool Blocked, bool Decor)
{
	if(!Blocked && Node->m_Type!=OBJECT_ENTITY && !Game->m_Scene->m_EditorShowRegions) return;
	if(Blocked && !Game->m_Scene->m_EditorShowBlocked) return;
	if(Decor && !Game->m_Scene->m_EditorShowDecor) return;

	if(!Region->m_Active || Region->m_Points.GetSize()<2) return;

	DWORD Color;
	if(Selected){
		if(Node->m_Type==OBJECT_ENTITY) Color = Game->m_Scene->m_EditorColEntitySel;
		else if(Blocked) Color = Game->m_Scene->m_EditorColBlockedSel;
		else if(Decor) Color = Game->m_Scene->m_EditorColDecorSel;
		else Color = Game->m_Scene->m_EditorColRegionSel;
	}
	else{
		if(Node->m_Type==OBJECT_ENTITY) Color = Game->m_Scene->m_EditorColEntity;
		else if(Blocked) Color = Game->m_Scene->m_EditorColBlocked;
		else if(Decor) Color = Game->m_Scene->m_EditorColDecor;
		else Color = Game->m_Scene->m_EditorColRegion;
	}

	CPoint point = GetDeviceScrollPosition();
	int ModX =  - point.x + Game->m_Scene->m_EditorMarginH;
	int ModY =  - point.y + Game->m_Scene->m_EditorMarginV;

	int i;

	for(i=0; i<Region->m_Points.GetSize(); i++){
		int j;
		j=i+1;
		if(j>=Region->m_Points.GetSize()) j=0;

		Game->m_Renderer->DrawLine(Region->m_Points[i]->x + ModX, Region->m_Points[i]->y + ModY,
								   Region->m_Points[j]->x + ModX, Region->m_Points[j]->y + ModY,
								   Color);
	}

	if(Selected){
		CRect rc(Region->m_Rect);
		rc.OffsetRect(CPoint(ModX, ModY));
		if(!m_Dragging) m_HotObjects.Add(new CHotObject(Node, &rc));
		for(i=0; i<Region->m_Points.GetSize(); i++){

			/*
			Game->m_Renderer->DrawLine(Region->m_Points[i]->x + ModX + 2, Region->m_Points[i]->y + ModY - 2,
									   Region->m_Points[i]->x + ModX - 3, Region->m_Points[i]->y + ModY + 3,
									   Color);


			Game->m_Renderer->DrawLine(Region->m_Points[i]->x + ModX - 2, Region->m_Points[i]->y + ModY - 2,
									   Region->m_Points[i]->x + ModX + 3, Region->m_Points[i]->y + ModY + 3,
									   Color);

			*/
			#define POINT_EXPAND 4

			CPoint p(Region->m_Points[i]->x + ModX, Region->m_Points[i]->y + ModY);
			Game->m_Renderer->DrawRect(p.x-POINT_EXPAND+1, p.y-POINT_EXPAND+1, p.x+POINT_EXPAND-1, p.y+POINT_EXPAND-1, Color);
		
			CRect rc(Region->m_Points[i]->x + ModX - 3, Region->m_Points[i]->y + ModY - 3, Region->m_Points[i]->x + ModX + 4, Region->m_Points[i]->y + ModY + 4);
			if(!m_Dragging) m_HotObjects.Add(new CHotObject(Region->m_Points[i], &rc));		
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::DrawEntity(CAdSceneNode* Node, CAdEntity *Entity, bool Selected)
{	
	if(!Entity->m_Active) return;

	if(Entity->m_Region){
		if(!Game->m_Scene->m_EditorShowEntities) return;

		DrawRegion(Node, Entity->m_Region, Selected, false, false);
		return;
	}
	
	CPoint point = GetDeviceScrollPosition();
	int ModX =  - point.x + Game->m_Scene->m_EditorMarginH;
	int ModY =  - point.y + Game->m_Scene->m_EditorMarginV;

	RECT rc;
	if(Entity->m_CurrentSprite)		
		Entity->m_CurrentSprite->GetBoundingRect(&rc, Entity->m_PosX + ModX, Entity->m_PosY + ModY);
	else SetRect(&rc, Entity->m_PosX + ModX, Entity->m_PosY + ModY, Entity->m_PosX + ModX + 100, Entity->m_PosY + ModY + 100);
	Game->m_Renderer->DrawRect(rc.left, rc.top, rc.right, rc.bottom, Game->m_Scene->m_EditorColEntitySel);

	// hotspot
	if(Entity->m_PosX + ModX != rc.left || Entity->m_PosY + ModY != rc.top)
	{
		Game->m_Renderer->DrawLine(Entity->m_PosX + ModX, Entity->m_PosY - 3 + ModY, Entity->m_PosX + ModX, Entity->m_PosY + 4 + ModY, Game->m_Scene->m_EditorColEntitySel);
		Game->m_Renderer->DrawLine(Entity->m_PosX - 3 + ModX, Entity->m_PosY + ModY, Entity->m_PosX + 4 + ModX, Entity->m_PosY + ModY, Game->m_Scene->m_EditorColEntitySel);
	}

	if(!m_Dragging){
		if(Node) m_HotObjects.Add(new CHotObject(Node, &CRect(rc)));
		else m_HotObjects.Add(new CHotObject(Entity, &CRect(rc)));
	}
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::DrawWaypoints(CAdWaypointGroup* Group, bool Selected)
{
	if(!Game->m_Scene->m_EditorShowBlocked) return;

	if(!Group->m_Active) return;

	CPoint point = GetDeviceScrollPosition();
	int ModX =  - point.x + Game->m_Scene->m_EditorMarginH;
	int ModY =  - point.y + Game->m_Scene->m_EditorMarginV;

	for(int i=0; i<Group->m_Points.GetSize(); i++){

		Game->m_Renderer->DrawLine(Group->m_Points[i]->x + ModX + 2, Group->m_Points[i]->y + ModY - 2,
								   Group->m_Points[i]->x + ModX - 3, Group->m_Points[i]->y + ModY + 3,
								   Selected?Game->m_Scene->m_EditorColWaypointsSel:Game->m_Scene->m_EditorColWaypoints);


		Game->m_Renderer->DrawLine(Group->m_Points[i]->x + ModX - 2, Group->m_Points[i]->y + ModY - 2,
								   Group->m_Points[i]->x + ModX + 3, Group->m_Points[i]->y + ModY + 3,
								   Selected?Game->m_Scene->m_EditorColWaypointsSel:Game->m_Scene->m_EditorColWaypoints);


		if(Selected){
			CRect rc(Group->m_Points[i]->x + ModX - 3, Group->m_Points[i]->y + ModY - 3, Group->m_Points[i]->x + ModX + 4, Group->m_Points[i]->y + ModY + 4);
			if(!m_Dragging) m_HotObjects.Add(new CHotObject(Group->m_Points[i], &rc));
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::ClearHotObjects()
{
		for(int i=0; i<m_HotObjects.GetSize(); i++) delete m_HotObjects[i];
		m_HotObjects.RemoveAll();
}


//////////////////////////////////////////////////////////////////////////
CHotObject* CSceneView::GetObjectAt(int X, int Y)
{
	for(int i=m_HotObjects.GetSize()-1; i>=0; i--){
		if(m_HotObjects[i]->m_Rect.PtInRect(CPoint(X, Y))) return m_HotObjects[i];
	}
	return NULL;
}


//////////////////////////////////////////////////////////////////////////
BOOL CSceneView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(&p);

	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
	{
		SetCursor(NULL);
		return TRUE;
	}


	CHotObject* obj = GetObjectAt(p.x, p.y);

	if(m_AddingPoint){
		SetCursor(AfxGetApp()->LoadCursor(IDC_ADD_POINT));
		return TRUE;
	}
	else if(m_RemovingPoint){
		if(obj && obj->IsPoint()) SetCursor(AfxGetApp()->LoadCursor(IDC_REMOVE_POINT));
		else SetCursor(AfxGetApp()->LoadCursor(IDC_NO_REMOVE));
		return TRUE;
	}
	else if(m_AddingScale){
		SetCursor(AfxGetApp()->LoadCursor(IDC_ADD_SCALE));
		return TRUE;
	}
	else if(m_AddingRot){
		SetCursor(AfxGetApp()->LoadCursor(IDC_ADD_ROT));
		return TRUE;
	}

	else{
		if(obj){
			if(obj->IsPoint()) SetCursor(AfxGetApp()->LoadCursor(IDC_MOVE_POINT));
			else if(obj->IsScale()) SetCursor(LoadCursor(NULL, IDC_SIZENS));
			else if(obj->IsRot()) SetCursor(LoadCursor(NULL, IDC_SIZEWE));
			else SetCursor(LoadCursor(NULL, IDC_SIZEALL));
			return TRUE;
		}
	}

	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	CPoint cur;
	GetCursorPos(&cur);
	ScreenToClient(&cur);
	cur += GetDeviceScrollPosition();
	cur.x -= Game->m_Scene->m_EditorMarginH;
	cur.y -= Game->m_Scene->m_EditorMarginV;

	
	CHotObject* obj = GetObjectAt(point.x, point.y);

	if(m_AddingPoint){
		if(m_SelectedNode){
			CBRegion* rgn=NULL;
			if(m_SelectedNode->m_Region) rgn = m_SelectedNode->m_Region;
			else if(m_SelectedNode->m_Entity && m_SelectedNode->m_Entity->m_Region) rgn = m_SelectedNode->m_Entity->m_Region;

			if(rgn){
				SetUndoState("Add point");
				AddPointToRegion(cur.x, cur.y, rgn);
				RestoreState();
				UpdateRegions();
				((CMainFrame*)AfxGetMainWnd())->m_wndWorkSpace.Refresh(true);
			}
		}
		else if(m_SelectedWptGroup){
			SetUndoState("Add point");
			m_SelectedWptGroup->m_Points.Add(::new CBPoint(cur.x, cur.y));
			RestoreState();
		}
	}
	else if(m_RemovingPoint){
		if(obj && obj->IsPoint()){
			SetUndoState("Remove point");
			if(m_SelectedNode){				
				CBRegion* rgn=NULL;
				if(m_SelectedNode->m_Region) rgn = m_SelectedNode->m_Region;
				else if(m_SelectedNode->m_Entity && m_SelectedNode->m_Entity->m_Region) rgn = m_SelectedNode->m_Entity->m_Region;

				if(rgn){
					for(int i=0; i<rgn->m_Points.GetSize(); i++){
						if(rgn->m_Points[i]->x==obj->m_Point->x && rgn->m_Points[i]->y==obj->m_Point->y){
							delete rgn->m_Points[i];
							rgn->m_Points.RemoveAt(i);
							UpdateRegions();
							((CMainFrame*)AfxGetMainWnd())->m_wndWorkSpace.Refresh(true);
							break;
						}
					}
				}
			}
			else{

				for(int i=0; i<m_SelectedWptGroup->m_Points.GetSize(); i++){
					if(m_SelectedWptGroup->m_Points[i]->x==obj->m_Point->x && m_SelectedWptGroup->m_Points[i]->y==obj->m_Point->y){
						delete m_SelectedWptGroup->m_Points[i];
						m_SelectedWptGroup->m_Points.RemoveAt(i);
						break;
					}
				}
			}
			RestoreState();
		}
	}
	else if(m_AddingScale){		
		CAdScaleLevel* scale = ::new CAdScaleLevel(Game);
		scale->m_PosY = cur.y;
		scale->m_Scale = 100;

		CScaleProp dlg;
		dlg.m_Scale = (int)scale->m_Scale;
		dlg.m_PosY = scale->m_PosY;
		if(dlg.DoModal()==IDOK){
			SetUndoState("Add scale");
			scale->m_Scale = (float)dlg.m_Scale;
			scale->m_PosY = dlg.m_PosY;
			Game->RegisterObject(scale);
			Game->m_Scene->m_ScaleLevels.Add(scale);
			Game->m_Scene->SortScaleLevels();
		}
		else delete scale;

		RestoreState();
	}
	else if(m_AddingRot){		
		CAdRotLevel* rot = ::new CAdRotLevel(Game);
		rot->m_PosX = cur.x;
		rot->m_Rotation = 0.0f;

		CRotProp dlg;
		dlg.m_Rotation = (int)rot->m_Rotation;
		dlg.m_PosX = rot->m_PosX;
		if(dlg.DoModal()==IDOK){
			SetUndoState("Add rotation");
			rot->m_Rotation = (float)dlg.m_Rotation;
			rot->m_PosX = dlg.m_PosX;
			Game->RegisterObject(rot);
			Game->m_Scene->m_RotLevels.Add(rot);
			Game->m_Scene->SortRotLevels();
		}
		else delete rot;

		RestoreState();
	}	
	else if(m_PlacingActor){
		ReleaseCapture();
		RestoreState();
		ShowCursor(TRUE);
	}
	else{
		if(obj){
			SetCapture();
			m_Dragging = true;
			m_DragFrom = m_DragOrigin = point;
			m_DragObject = obj;

			SetUndoState("Move");
		}
	}
	
	CScrollView::OnLButtonDown(nFlags, point);
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_Dragging) EndDrag();
	
	CScrollView::OnLButtonUp(nFlags, point);
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_Dragging && m_DragObject){
		UpdateDrag(m_DragFrom, point);
	}

	if(m_PlacingActor){
		CPoint cur;
		GetCursorPos(&cur);
		ScreenToClient(&cur);
		cur += GetDeviceScrollPosition();
		cur.x -= Game->m_Scene->m_EditorMarginH;
		cur.y -= Game->m_Scene->m_EditorMarginV;

		if(m_Actor)
		{
			m_Actor->m_PosX = cur.x;
			m_Actor->m_PosY = cur.y;
		}
		else if(m_Actor3D)
		{
			m_Actor3D->SkipTo(cur.x, cur.y, true);
		}
		else if(m_Actor3DX)
		{
			m_Actor3DX->SkipTo(cur.x, cur.y, true);
		}
	}
	
	CScrollView::OnMouseMove(nFlags, point);
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	m_SelectedScale = NULL;
	m_SelectedRot = NULL;

	if(m_AddingPoint || m_RemovingPoint || m_AddingScale || m_AddingRot){
		RestoreState();
		return;
	}

	if(m_Dragging){
		UpdateDrag(m_DragFrom, m_DragOrigin);
		EndDrag();
		PurgeLastUndo();
		return;
	}

	if(m_PlacingActor){
		if(m_Actor)
		{
			m_Actor->m_Dir = (TDirection)(m_Actor->m_Dir + 1);
			if(m_Actor->m_Dir>=NUM_DIRECTIONS) m_Actor->m_Dir = (TDirection)0;
		}
		else if(m_Actor3D)
		{
			m_Actor3D->m_Angle = CBUtils::NormalizeAngle(m_Actor3D->m_Angle + 45.0f);
		}
		else if(m_Actor3DX)
		{
			m_Actor3DX->m_Angle = CBUtils::NormalizeAngle(m_Actor3DX->m_Angle + 45.0f);
		}

		return;
	}

	CHotObject* obj = GetObjectAt(point.x, point.y);	
	if(obj && obj->IsScale()){
		SetFocus();
		m_SelectedScale = obj->m_Scale;
		ClientToScreen(&point);
		theApp.ShowPopupMenu(IDR_POPUP_SCALE, point, AfxGetMainWnd ());
		return;
	}
	else if(obj && obj->IsRot()){
		SetFocus();
		m_SelectedRot = obj->m_Rot;
		ClientToScreen(&point);
		theApp.ShowPopupMenu(IDR_POPUP_ROTATION, point, AfxGetMainWnd());
		return;
	}

	CScrollView::OnRButtonUp(nFlags, point);
}



//////////////////////////////////////////////////////////////////////////
void CSceneView::EndDrag()
{
	ReleaseCapture();

	//if(m_DragObject->IsNode() && m_DragObject->m_Node->m_Type==OBJECT_REGION) m_DragObject->m_Node->m_Region->CreateRegion();
	UpdateRegions();

	m_Dragging = false;
	m_DragObject = NULL;
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::UpdateDrag(CPoint Origin, CPoint Current)
{
	int xdiff = 0;
	int ydiff = 0;
	
	if(Current.x != 0 && Current.y != 0){
		xdiff = Current.x - Origin.x;
		ydiff = Current.y - Origin.y;
	}
	
	if(m_DragObject->IsPoint()){
		m_DragObject->m_Point->x += xdiff;
		m_DragObject->m_Point->y += ydiff;
	}
	else if(m_DragObject->IsNode()){
		if(m_DragObject->m_Node->m_Type == OBJECT_ENTITY){
			if(!m_DragObject->m_Node->m_Entity->m_Region){
				m_DragObject->m_Node->m_Entity->m_PosX += xdiff;
				m_DragObject->m_Node->m_Entity->m_PosY += ydiff;
			}
			else{
				CBRegion* rgn = m_DragObject->m_Node->m_Entity->m_Region;
				for(int i=0; i<rgn->m_Points.GetSize(); i++){
					rgn->m_Points[i]->x += xdiff;
					rgn->m_Points[i]->y += ydiff;
				}
			}
		}
		else if(m_DragObject->m_Node->m_Type == OBJECT_REGION){
			CAdRegion* rgn = m_DragObject->m_Node->m_Region;
			for(int i=0; i<rgn->m_Points.GetSize(); i++){
				rgn->m_Points[i]->x += xdiff;
				rgn->m_Points[i]->y += ydiff;
			}
		}
	}
	else if(m_DragObject->IsEntity()){
		m_DragObject->m_Entity->m_PosX += xdiff;
		m_DragObject->m_Entity->m_PosY += ydiff;
	}
	else if(m_DragObject->IsScale()){
		m_DragObject->m_Scale->m_PosY += ydiff;
		Game->m_Scene->SortScaleLevels();
	}
	else if(m_DragObject->IsRot()){
		m_DragObject->m_Rot->m_PosX += xdiff;
		Game->m_Scene->SortRotLevels();
	}
	
	m_DragFrom = Current;
	
	AfterPosChange();	

}


//////////////////////////////////////////////////////////////////////////
void CSceneView::AfterPosChange()
{
	if(m_SelectedWaypoints && m_SelectedWptGroup){
		((CMainFrame*)AfxGetMainWnd())->m_wndProperty.ShowWaypoints(m_SelectedWptGroup, this, true);
	}
	else if(m_SelectedFree && m_SelectedFreeObject){
		((CMainFrame*)AfxGetMainWnd())->m_wndProperty.ShowEntity((CAdEntity*)m_SelectedFreeObject, this, true);
	}
	else if(m_SelectedNode){
		if(m_SelectedNode->m_Type==OBJECT_REGION) ((CMainFrame*)AfxGetMainWnd())->m_wndProperty.ShowRegion(m_SelectedNode->m_Region, this, true);
		else if(m_SelectedNode->m_Type==OBJECT_ENTITY){
			if(m_SelectedNode->m_Entity->m_Region){
				m_SelectedNode->m_Entity->m_Region->CreateRegion();
				m_SelectedNode->m_Entity->UpdatePosition();
			}
			((CMainFrame*)AfxGetMainWnd())->m_wndProperty.ShowEntity(m_SelectedNode->m_Entity, this, true);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CHotObject* hot_obj = GetObjectAt(point.x, point.y);
	if(hot_obj && hot_obj->IsPoint()){
		CPointPosDlg dlg;
		dlg.m_PosX = hot_obj->m_Point->x;
		dlg.m_PosY = hot_obj->m_Point->y;
		if(dlg.DoModal()==IDOK){
			SetUndoState("Move point");
			hot_obj->m_Point->x = dlg.m_PosX;
			hot_obj->m_Point->y = dlg.m_PosY;
			UpdateRegions();
		}

		CScrollView::OnLButtonDblClk(nFlags, point);
		return;
	}


	CBObject* obj = Game->m_Renderer->GetObjectAt(point.x, point.y);
	bool handled = false;
	if(obj){
		for(int i=0; i<Game->m_Scene->m_Layers.GetSize(); i++){
			for(int j=0; j<Game->m_Scene->m_Layers[i]->m_Nodes.GetSize(); j++){
				CAdSceneNode* Node = Game->m_Scene->m_Layers[i]->m_Nodes[j];
				if(Node->m_Type==OBJECT_ENTITY && Node->m_Entity && Node->m_Entity==obj){
					m_SelectedLayer = Game->m_Scene->m_Layers[i];
					m_SelectedNode = Node;
					m_SelectedWaypoints = false;
					m_SelectedWptGroup = NULL;
					m_SelectedFree = false;
					m_SelectedFreeObject = NULL;
					((CMainFrame*)AfxGetMainWnd())->m_wndWorkSpace.Refresh(true);
					handled = true;
					break;
				}
			}
		}
		if(!handled){
			for(int i=0; i<Game->m_Scene->m_Objects.GetSize(); i++){
				if(Game->m_Scene->m_Objects[i]==obj){
					m_SelectedFree = true;
					m_SelectedFreeObject = Game->m_Scene->m_Objects[i];
					m_SelectedLayer = NULL;
					m_SelectedNode = NULL;
					m_SelectedWaypoints = false;
					m_SelectedWptGroup = NULL;
					((CMainFrame*)AfxGetMainWnd())->m_wndWorkSpace.Refresh(true);
					break;
				}
			}
		}
	}
	
	CScrollView::OnLButtonDblClk(nFlags, point);
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::RegisterAllObjects()
{
	CAdScene* Scene = Game->m_Scene;
	if(!Scene) return;

	for(int i=0; i<Scene->m_Layers.GetSize(); i++){
		for(int j=0; j<Scene->m_Layers[i]->m_Nodes.GetSize(); j++){
			if(Scene->m_Layers[i]->m_Nodes[j]->m_Type==OBJECT_ENTITY && Scene->m_Layers[i]->m_Nodes[j]->m_Entity)
				Scene->m_Layers[i]->m_Nodes[j]->m_Entity->m_EditorAlwaysRegister = true;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::UpdateRegions()
{
	CAdScene* Scene = Game->m_Scene;
	if(!Scene) return;

	for(int i=0; i<Scene->m_Layers.GetSize(); i++){
		for(int j=0; j<Scene->m_Layers[i]->m_Nodes.GetSize(); j++){
			if(Scene->m_Layers[i]->m_Nodes[j]->m_Type==OBJECT_REGION && Scene->m_Layers[i]->m_Nodes[j]->m_Region)
				Scene->m_Layers[i]->m_Nodes[j]->m_Region->CreateRegion();

			if(Scene->m_Layers[i]->m_Nodes[j]->m_Type==OBJECT_ENTITY && Scene->m_Layers[i]->m_Nodes[j]->m_Entity->m_Region){
				Scene->m_Layers[i]->m_Nodes[j]->m_Entity->m_Region->CreateRegion();
				Scene->m_Layers[i]->m_Nodes[j]->m_Entity->UpdatePosition();
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::AddPoint()
{
	m_RemovingPoint = false;
	m_AddingScale = false;
	m_AddingPoint = true;
	m_AddingRot = false;
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::RemovePoint()
{
	m_AddingPoint = false;
	m_RemovingPoint = true;
	m_AddingScale = false;
	m_AddingRot = false;
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::AddScale()
{
	m_AddingPoint = false;
	m_RemovingPoint = false;
	m_AddingScale = true;
	m_AddingRot = false;
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::AddRot()
{
	m_AddingPoint = false;
	m_RemovingPoint = false;
	m_AddingScale = false;
	m_AddingRot = true;
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::RestoreState()
{
	m_AddingPoint = false;
	m_RemovingPoint = false;
	m_AddingScale = false;
	m_PlacingActor = false;
	m_AddingRot = false;

	// refresh cursor
	POINT p;
	GetCursorPos(&p);
	SetCursorPos(p.x, p.y);
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::AddPointToRegion(int x, int y, CBRegion *Region)
{
	double min_dist = DBL_MAX;
	int min_index = -1;

	for(int i=0; i<Region->m_Points.GetSize(); i++){
		int j;
		j=i+1;
		if(j>=Region->m_Points.GetSize()) j=0;

		double dist = PointDist(Region->m_Points[i]->x, Region->m_Points[i]->y, Region->m_Points[j]->x, Region->m_Points[j]->y, x, y);
		if(dist<min_dist){
			min_dist = dist;
			min_index = i;
		}

	}
	Region->m_Points.InsertAt(min_index+1, ::new CBPoint(x, y));
}


//////////////////////////////////////////////////////////////////////////
double CSceneView::PointDist(int X1, int Y1, int X2, int Y2, int PX, int PY)
{
	double a, b, c;

	double x,y;

	x = PX - X2;
	y = PY - Y2;
	a = fabs(sqrt(x*x + y*y));

	x = PX - X1;
	y = PY - Y1;
	b = fabs(sqrt(x*x + y*y));

	x = X1 - X2;
	y = Y1 - Y2;
	c = fabs(sqrt(x*x + y*y));

	double alpha = fabs(acos((b*b+c*c-a*a)/(2*b*c)));
	double beta = fabs(acos((a*a+c*c-b*b)/(2*a*c)));
	
	double ret = fabs(sin(alpha) * b) /*+ b+a*/;

	if(alpha > DegToRad(90) || beta > DegToRad(90)) ret += 1000;
	return ret;
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::SetDirty(bool IsDirty)
{
	if(IsDirty) GetDocument()->SetModifiedFlag(TRUE);
	else GetDocument()->SetModifiedFlag(FALSE);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CSceneView::SetUndoState(CString Name)
{
	//MessageBox(Name);
	UpdateSelectionState();
	CBDynBuffer* buf = new CBDynBuffer(Game);
	Game->m_Scene->SaveAsText(buf, 0);
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
void CSceneView::OnEditUndo() 
{
	Undo();
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_UndoPointer>=0);
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::Undo() 
{
	CWaitCursor wait;

	BYTE* buf = m_UndoStates[m_UndoPointer]->GetData(true);
	if(buf && SUCCEEDED(Game->m_Scene->LoadBuffer(buf, true))){
		RegisterAllObjects();
		GetSelectedObjects();

		PurgeLastUndo();

		Game->m_EditorResolutionWidth = Game->m_Scene->m_EditorResolutionWidth;
		Game->m_EditorResolutionHeight = Game->m_Scene->m_EditorResolutionHeight;

		((CMainFrame*)AfxGetMainWnd())->m_wndWorkSpace.Refresh(true);
	}
	else MessageBox("Cannot undo changes", "Error", MB_OK|MB_ICONERROR);
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::PurgeLastUndo()
{
	if(m_UndoPointer < m_UndoStates.GetSize()){
		delete m_UndoStates[m_UndoPointer];
		m_UndoStates.RemoveAt(m_UndoPointer);
		m_UndoPointer--;
	}
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::UpdateSelectionState()
{
	CAdScene* Scene = Game->m_Scene;
	if(!Scene) return;

	int i;

	for(i=0; i<Scene->m_Layers.GetSize(); i++){
		Scene->m_Layers[i]->m_EditorSelected = (m_SelectedLayer == Scene->m_Layers[i]);
		for(int j=0; j<Scene->m_Layers[i]->m_Nodes.GetSize(); j++){
			if(Scene->m_Layers[i]->m_Nodes[j]->m_Type==OBJECT_ENTITY){
				Scene->m_Layers[i]->m_Nodes[j]->m_Entity->m_EditorSelected = (m_SelectedNode==Scene->m_Layers[i]->m_Nodes[j]);
			}
			if(Scene->m_Layers[i]->m_Nodes[j]->m_Type==OBJECT_REGION){
				Scene->m_Layers[i]->m_Nodes[j]->m_Region->m_EditorSelected = (m_SelectedNode==Scene->m_Layers[i]->m_Nodes[j]);
			}
		}
	}

	for(i=0; i<Scene->m_WaypointGroups.GetSize(); i++){
		Scene->m_WaypointGroups[i]->m_EditorSelected = (m_SelectedWptGroup==Scene->m_WaypointGroups[i]);
	}

	for(i=0; i<Scene->m_Objects.GetSize(); i++){
		Scene->m_Objects[i]->m_EditorSelected = (m_SelectedFreeObject==Scene->m_Objects[i]);
	}
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::GetSelectedObjects()
{
	CAdScene* Scene = Game->m_Scene;
	if(!Scene) return;

	int i;

	m_SelectedLayer = NULL;
	m_SelectedNode = NULL;
	m_SelectedWptGroup = NULL;
	m_SelectedWaypoints = false;
	m_SelectedFree = false;
	m_SelectedFreeObject = NULL;

	for(i=0; i<Scene->m_Layers.GetSize(); i++){
		if(Scene->m_Layers[i]->m_EditorSelected) m_SelectedLayer = Scene->m_Layers[i];
		for(int j=0; j<Scene->m_Layers[i]->m_Nodes.GetSize(); j++){
			if(Scene->m_Layers[i]->m_Nodes[j]->m_Type==OBJECT_ENTITY){
				if(Scene->m_Layers[i]->m_Nodes[j]->m_Entity->m_EditorSelected) m_SelectedNode=Scene->m_Layers[i]->m_Nodes[j];
			}
			if(Scene->m_Layers[i]->m_Nodes[j]->m_Type==OBJECT_REGION){
				if(Scene->m_Layers[i]->m_Nodes[j]->m_Region->m_EditorSelected) m_SelectedNode=Scene->m_Layers[i]->m_Nodes[j];
			}
		}
	}

	for(i=0; i<Scene->m_WaypointGroups.GetSize(); i++){
		if(Scene->m_WaypointGroups[i]->m_EditorSelected){
			m_SelectedWptGroup = Scene->m_WaypointGroups[i];
			m_SelectedNode = NULL;
		}
	}

	for(i=0; i<Scene->m_Objects.GetSize(); i++){
		if(Scene->m_Objects[i]->m_EditorSelected){
			m_SelectedFree = true;
			m_SelectedFreeObject = Scene->m_Objects[i];
		}
	}

	if(!m_SelectedLayer && !m_SelectedFree) m_SelectedWaypoints = true;
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::OnToolsRefresh() 
{
	CWaitCursor wait;
	SetUndoState("Temporary");
	Undo();
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::DrawScale(CAdScaleLevel *Scale)
{
	if(!Game->m_Scene->m_EditorShowScale) return;

	CPoint point = GetDeviceScrollPosition();
	CPoint p = GetScrollPosition();
	int ModX =  - point.x + Game->m_Scene->m_EditorMarginH;
	int ModY =  - point.y + Game->m_Scene->m_EditorMarginV;
	CSize size = GetTotalSize();

	// draw line
	Game->m_Renderer->DrawLine(0+ModX, Scale->m_PosY+ModY, size.cx - Game->m_Scene->m_EditorMarginH - p.x + 1, Scale->m_PosY+ModY, Game->m_Scene->m_EditorColScale);

	// draw handle
	int xxx = max(0, ModX);
	Game->m_Renderer->DrawLine(xxx, Scale->m_PosY+ModY, xxx, Scale->m_PosY+ModY+4, Game->m_Scene->m_EditorColScale);
	Game->m_Renderer->DrawLine(xxx+10, Scale->m_PosY+ModY, xxx+10, Scale->m_PosY+ModY+4, Game->m_Scene->m_EditorColScale);
	Game->m_Renderer->DrawLine(xxx, Scale->m_PosY+ModY+4, xxx+11, Scale->m_PosY+ModY+4, Game->m_Scene->m_EditorColScale);

	CRect rc;
	rc.SetRect(xxx, Scale->m_PosY+ModY, xxx+11, Scale->m_PosY+ModY+4);
	if(!m_Dragging) m_HotObjects.Add(new CHotObject(Scale, &rc));

	if(Game->m_SystemFont){
		char str[100];
		sprintf(str, "%d%%", (int)Scale->m_Scale);
		Game->m_Renderer->Setup2D();
		Game->m_SystemFont->DrawText((BYTE*)str, xxx, Scale->m_PosY+ModY - Game->m_SystemFont->GetTextHeight((BYTE*)"", 1000), 100);
		Game->m_Renderer->SetupLines();
	}
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::DrawRot(CAdRotLevel *Rot)
{
	if(!Game->m_Scene->m_EditorShowScale) return;

	CPoint point = GetDeviceScrollPosition();
	CPoint p = GetScrollPosition();
	int ModX =  - point.x + Game->m_Scene->m_EditorMarginH;
	int ModY =  - point.y + Game->m_Scene->m_EditorMarginV;
	CSize size = GetTotalSize();

	// draw line
	Game->m_Renderer->DrawLine(Rot->m_PosX+ModX, 0+ModY, Rot->m_PosX+ModX, size.cy - Game->m_Scene->m_EditorMarginV - p.y + 1, Game->m_Scene->m_EditorColScale);

	// draw handle
	int yyy = max(0, ModY);
	Game->m_Renderer->DrawLine(Rot->m_PosX+ModX, yyy, Rot->m_PosX+ModX+4, yyy, Game->m_Scene->m_EditorColScale);
	Game->m_Renderer->DrawLine(Rot->m_PosX+ModX, yyy+10, Rot->m_PosX+ModX+4, yyy+10, Game->m_Scene->m_EditorColScale);
	Game->m_Renderer->DrawLine(Rot->m_PosX+ModX+4, yyy, Rot->m_PosX+ModX+4, yyy+11, Game->m_Scene->m_EditorColScale);

	CRect rc;
	rc.SetRect(Rot->m_PosX+ModX, yyy, Rot->m_PosX+ModX+4, yyy+11);
	if(!m_Dragging) m_HotObjects.Add(new CHotObject(Rot, &rc));

	if(Game->m_SystemFont){
		char str[100];
		sprintf(str, "%+d°", (int)Rot->m_Rotation);
		Game->m_Renderer->Setup2D();
		Game->m_SystemFont->DrawText((BYTE*)str, Rot->m_PosX+ModX - Game->m_SystemFont->GetTextWidth((BYTE*)"", 1000) + 6, yyy, 100);
		Game->m_Renderer->SetupLines();
	}
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::OnScaleProperties() 
{
	if(m_SelectedScale){
		CScaleProp dlg;
		dlg.m_Scale = (int)m_SelectedScale->m_Scale;
		dlg.m_PosY = m_SelectedScale->m_PosY;
		if(dlg.DoModal()==IDOK){
			SetUndoState("Change scale level properties");
			m_SelectedScale->m_PosY = dlg.m_PosY;
			m_SelectedScale->m_Scale = (float)dlg.m_Scale;
			Game->m_Scene->SortScaleLevels();
		}
	}	
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::OnRotProperties() 
{
	if(m_SelectedRot){
		CRotProp dlg;
		dlg.m_Rotation = (int)m_SelectedRot->m_Rotation;
		dlg.m_PosX = m_SelectedRot->m_PosX;
		if(dlg.DoModal()==IDOK){
			SetUndoState("Change rotation level properties");
			m_SelectedRot->m_PosX = dlg.m_PosX;
			m_SelectedRot->m_Rotation = (float)dlg.m_Rotation;
			Game->m_Scene->SortRotLevels();
		}
	}	
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::OnScaleRemove() 
{
	if(m_SelectedScale){
		if(MessageBox("Do you really want to remove this scale level?", NULL, MB_YESNO|MB_ICONQUESTION)==IDYES){
			for(int i=0; i<Game->m_Scene->m_ScaleLevels.GetSize(); i++){
				if(Game->m_Scene->m_ScaleLevels[i]==m_SelectedScale){
					SetUndoState("Remove scale level");
					Game->UnregisterObject(m_SelectedScale);
					m_SelectedScale = NULL;
					Game->m_Scene->m_ScaleLevels.RemoveAt(i);
					Game->m_Scene->SortScaleLevels();
					break;
				}
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::OnRotRemove() 
{
	if(m_SelectedRot){
		if(MessageBox("Do you really want to remove this rotation level?", NULL, MB_YESNO|MB_ICONQUESTION)==IDYES){
			for(int i=0; i<Game->m_Scene->m_RotLevels.GetSize(); i++){
				if(Game->m_Scene->m_RotLevels[i]==m_SelectedRot){
					SetUndoState("Remove rotation level");
					Game->UnregisterObject(m_SelectedRot);
					m_SelectedRot = NULL;
					Game->m_Scene->m_RotLevels.RemoveAt(i);
					Game->m_Scene->SortRotLevels();
					break;
				}
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::OnActorLoad() 
{
	CString OrigFile;
	if(m_Actor && m_Actor->m_Filename) OrigFile = CString(m_Actor->m_Filename);
	else if(m_Actor3D && m_Actor3D->m_Filename) OrigFile = CString(m_Actor3D->m_Filename);
	else if(m_Actor3DX && m_Actor3DX->m_Filename) OrigFile = CString(m_Actor3DX->m_Filename);
	CString NewFile = GetRelativeFilename(Game, OrigFile, "Actor Files (*.actor; *.act3d)|*.actor;*.act3d", ((CSceneEditApp*)AfxGetApp())->m_LastDirActor, this);
	if(NewFile!=""){
		if(m_Actor)
		{
			Game->RemoveObject(m_Actor);
			m_Actor = NULL;
		}
		if(m_Actor3D)
		{
			Game->RemoveObject(m_Actor3D);
			m_Actor3D = NULL;
		}
		if(m_Actor3DX)
		{
			Game->RemoveObject(m_Actor3DX);
			m_Actor3DX = NULL;
		}

		bool Success = false;
		CString Ext = GetExt(NewFile);
		if(Ext.CompareNoCase("act3d")==0)
		{
			CAdObject3D* Act = CAdActor3D::CreateFromFile(Game, (char*)LPCSTR(NewFile));
			if(!Act)
			{
				MessageBox("Error loading actor '" + NewFile + "'", "Error", MB_ICONERROR);
			}
			else
			{
				if(strcmp(Act->GetClassName(), "CAdActor3DX")==0)
				{
					m_Actor3DX = (CAdActor3DX*)Act;
					m_Actor3DX->m_Angle = 0.0f;
				}
				else
				{
					m_Actor3D = (CAdActor3D*)Act;
					m_Actor3D->m_Angle = 0.0f;
				}
				Success = true;				
			}
		}
		else
		{
			m_Actor = ::new CAdActor(Game);
			if(!m_Actor || FAILED(m_Actor->LoadFile((char*)LPCSTR(NewFile)))){
				SAFE_DELETE(m_Actor);
				MessageBox("Error loading actor '" + NewFile + "'", "Error", MB_ICONERROR);
			}
			else Success = true;
		}

		if(Success)
		{
			CSize size = GetTotalSize();
			CPoint p = GetDeviceScrollPosition();
			
			if(m_Actor)
			{
				m_Actor->m_PosX = -Game->m_Scene->m_EditorMarginH + p.x + size.cx / 2;
				m_Actor->m_PosY = -Game->m_Scene->m_EditorMarginV + p.y + size.cy / 2;

				Game->AddObject(m_Actor);
				m_Actor->m_Active = true;
			}
			else if(m_Actor3D)
			{
				m_Actor3D->SkipTo(-Game->m_Scene->m_EditorMarginH + p.x + size.cx / 2, -Game->m_Scene->m_EditorMarginV + p.y + size.cy / 2, true);
				Game->AddObject(m_Actor3D);
				m_Actor3D->m_Active = true;
			}
			else if(m_Actor3DX)
			{
				m_Actor3DX->SkipTo(-Game->m_Scene->m_EditorMarginH + p.x + size.cx / 2, -Game->m_Scene->m_EditorMarginV + p.y + size.cy / 2, true);
				Game->AddObject(m_Actor3DX);
				m_Actor3DX->m_Active = true;
			}

			OnActorPlace();
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::OnActorPlace() 
{
	if(!m_Actor && !m_Actor3D && !m_Actor3DX){
		OnActorLoad();
		return;
	}

	CPoint cur;
	if(m_Actor)
	{
		cur.x = m_Actor->m_PosX;
		cur.y = m_Actor->m_PosY;
	}
	else if(m_Actor3D)
	{
		cur.x = m_Actor3D->m_PosX;
		cur.y = m_Actor3D->m_PosY;
	}
	else if(m_Actor3DX)
	{
		cur.x = m_Actor3DX->m_PosX;
		cur.y = m_Actor3DX->m_PosY;
	}

	cur.x += Game->m_Scene->m_EditorMarginH;
	cur.y += Game->m_Scene->m_EditorMarginV;
	cur -= GetDeviceScrollPosition();
	ClientToScreen(&cur);

	SetCursorPos(cur.x, cur.y);


	if(m_Actor) m_Actor->m_Active = true;
	else if(m_Actor3D) m_Actor3D->m_Active = true;
	else if(m_Actor3DX) m_Actor3DX->m_Active = true;

	RestoreState();
	m_PlacingActor = true;
	ShowCursor(FALSE);
	SetCapture();
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::OnActorRemove() 
{
	if(m_Actor) m_Actor->m_Active = false;
	else if(m_Actor3D) m_Actor3D->m_Active = false;
	else if(m_Actor3DX) m_Actor3DX->m_Active = false;
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::OnUpdateActorRemove(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_Actor!=NULL || m_Actor3D!=NULL || m_Actor3DX!=NULL);
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::OnActorCopyData() 
{
	CString text;
	int X, Y;
	TDirection Dir;

	if(!m_Actor && !m_Actor3D && !m_Actor3DX) return;

	if(m_Actor)
	{
		X = m_Actor->m_PosX;
		Y = m_Actor->m_PosY;
		Dir = m_Actor->m_Dir;
	}
	else if(m_Actor3D)
	{
		X = m_Actor3D->m_PosX;
		Y = m_Actor3D->m_PosY;
		Dir = m_Actor3D->AngleToDir(m_Actor3D->m_Angle);
	}
	else
	{
		X = m_Actor3DX->m_PosX;
		Y = m_Actor3DX->m_PosY;
		Dir = m_Actor3DX->AngleToDir(m_Actor3DX->m_Angle);
	}

	text.Format("actor.GoTo(%d, %d);\r\nactor.TurnTo(", X, Y);
	switch(Dir){
		case DI_UP:        text += "DI_UP";        break;
		case DI_UPRIGHT:   text += "DI_UPRIGHT";   break;
		case DI_RIGHT:     text += "DI_RIGHT";     break;
		case DI_DOWNRIGHT: text += "DI_DOWNRIGHT"; break;
		case DI_DOWN:      text += "DI_DOWN";      break;
		case DI_DOWNLEFT:  text += "DI_DOWNLEFT";  break;
		case DI_LEFT:      text += "DI_LEFT";      break;
		case DI_UPLEFT:    text += "DI_UPLEFT";    break;
	}

	text += ");\r\n";

	if(OpenClipboard ()){
		EmptyClipboard();

		HGLOBAL hClipbuffer = ::GlobalAlloc (GMEM_DDESHARE, text.GetLength() + 1);
		LPTSTR lpszBuffer = (LPTSTR)GlobalLock(hClipbuffer);

		_tcscpy (lpszBuffer, (LPCTSTR)text);

		::GlobalUnlock (hClipbuffer);
		::SetClipboardData (CF_TEXT, hClipbuffer);

		CloseClipboard();
	}
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::OnUpdateActorCopyData(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((m_Actor && m_Actor->m_Active) || (m_Actor3D && m_Actor3D->m_Active) || (m_Actor3DX && m_Actor3DX->m_Active));
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar==VK_ADD || nChar==VK_SUBTRACT)
	{
		CBRegion* rgn=NULL;
		if(m_SelectedNode)
		{
			if(m_SelectedNode->m_Region) rgn = m_SelectedNode->m_Region;
			else if(m_SelectedNode->m_Entity && m_SelectedNode->m_Entity->m_Region) rgn = m_SelectedNode->m_Entity->m_Region;

			if(rgn)
			{
				if(nChar==VK_ADD) AddPoint();
				else RemovePoint();

				::SendMessage(GetSafeHwnd(), WM_SETCURSOR, 0, 0);
			}
		}
		else if(m_SelectedWptGroup)
		{
			if(nChar==VK_ADD) AddPoint();
			else RemovePoint();
		}
		return;
	}


	int MoveX=0, MoveY=0;
	bool SwapState = false;

	switch(nChar){
		case VK_LEFT:  MoveX = -1; break;
		case VK_RIGHT: MoveX = 1;  break;
		case VK_UP:    MoveY = -1; break;
		case VK_DOWN:  MoveY = 1;  break;
		case VK_SPACE: SwapState = true;  break;
	}

	if((GetAsyncKeyState(VK_SHIFT) & 0x8000)!=0)
	{
		MoveX *= 5;
		MoveY *= 5;
	}

	if(MoveX == 0 && MoveY == 0 && !SwapState){
		CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}

	SetUndoState("Move object");

	if(m_SelectedNode){
		if(m_SelectedNode->m_Type==OBJECT_ENTITY){
			if(m_SelectedNode->m_Entity->m_Region){
				CBRegion* rgn = m_SelectedNode->m_Entity->m_Region;
				for(int i=0; i<rgn->m_Points.GetSize(); i++){
					rgn->m_Points[i]->x += MoveX;
					rgn->m_Points[i]->y += MoveY;
				}
			}
			else{
				m_SelectedNode->m_Entity->m_PosX += MoveX;
				m_SelectedNode->m_Entity->m_PosY += MoveY;
			}
			if(SwapState) m_SelectedNode->m_Entity->m_Active = !m_SelectedNode->m_Entity->m_Active;
		}
		else if(m_SelectedNode->m_Type==OBJECT_REGION){
			CAdRegion* rgn = m_SelectedNode->m_Region;
			for(int i=0; i<rgn->m_Points.GetSize(); i++){
				rgn->m_Points[i]->x += MoveX;
				rgn->m_Points[i]->y += MoveY;
			}
			if(SwapState) rgn->m_Active = !rgn->m_Active;
		}
	}
	else if(m_SelectedFreeObject){
		m_SelectedFreeObject->m_PosX += MoveX;
		m_SelectedFreeObject->m_PosY += MoveY;

		if(SwapState) m_SelectedFreeObject->m_Active = !m_SelectedFreeObject->m_Active;
	}

	AfterPosChange();
	if(SwapState)
	{
		((CMainFrame*)AfxGetMainWnd())->m_wndWorkSpace.Refresh(true);
	}
	
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}


//////////////////////////////////////////////////////////////////////////
void CSceneView::OnActorAnim() 
{
	CAnimDlg Dlg;
	Dlg.m_Actor = m_Actor3DX;
	Dlg.DoModal();
}

//////////////////////////////////////////////////////////////////////////
void CSceneView::OnUpdateActorAnim(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_Actor3DX!=NULL);
}
