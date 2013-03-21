// DCGFView.cpp : implementation file
//

#include "stdafx.h"
#include "DCGFView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef DCGF_ADVENTURE
	#if _DEBUG
		#pragma comment(lib, "wme_ad_d.lib")
	#else
		#pragma comment(lib, "wme_ad.lib")
	#endif
#else
	#if _DEBUG
		#pragma comment(lib, "wme_base_d.lib")
	#else
		#pragma comment(lib, "wme_base.lib")
	#endif
#endif


/////////////////////////////////////////////////////////////////////////////
// CDCGFView

IMPLEMENT_DYNCREATE(CDCGFView, CScrollView)

//////////////////////////////////////////////////////////////////////////
CDCGFView::CDCGFView()
{
	// create game
#ifdef DCGF_ADVENTURE
	Game = ::new CAdGame;
#else
	Game = ::new CBGame;
#endif

	Game->m_EditorMode = true;

	CString RegPath;
	RegPath.Format("Software\\%s\\%s\\Engine Settings", AfxGetApp()->m_pszRegistryKey, AfxGetApp()->m_pszAppName);
	Game->m_Registry->SetBasePath((char*)LPCSTR(RegPath));
	Game->DEBUG_DebugEnable();

	// initialize renderer
	Game->m_UseD3D = Game->m_Registry->ReadBool("Options", "Accelerated", true);
	Game->Initialize1();
	Game->Initialize2();
	Game->m_Renderer->m_Windowed = true;
	Game->m_Renderer->m_Instance = AfxGetInstanceHandle();

	m_UndoPointer = -1;

	m_ViewReady = false;
}


//////////////////////////////////////////////////////////////////////////
CDCGFView::~CDCGFView()
{
	for(int i=0; i<m_UndoStates.GetSize(); i++) delete m_UndoStates[i];
	m_UndoStates.RemoveAll();


	delete Game;
}


BEGIN_MESSAGE_MAP(CDCGFView, CScrollView)
	//{{AFX_MSG_MAP(CDCGFView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDCGFView drawing

//////////////////////////////////////////////////////////////////////////
void CDCGFView::OnDraw(CDC* pDC)
{
	RenderFrame();
}

/////////////////////////////////////////////////////////////////////////////
// CDCGFView diagnostics

#ifdef _DEBUG
void CDCGFView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDCGFView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDCGFView message handlers


//////////////////////////////////////////////////////////////////////////
void CDCGFView::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();
	
	CRect rc;
	GetClientRect(&rc);

	CHWManager* m = new CHWManager(Game);
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

	Game->m_SystemFont = Game->m_FontStorage->AddFont("system_font.fnt");
}


//////////////////////////////////////////////////////////////////////////
void CDCGFView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	
	if(m_nMapMode==0) return;
	
	if(Game->m_UseD3D){
		CBRenderD3D* rend = (CBRenderD3D*)Game->m_Renderer;
		if(rend->m_Ready){
			rend->m_Ready = false;
			rend->m_Width = cx;
			rend->m_Height = cy;
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
BOOL CDCGFView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
bool CDCGFView::Undo()
{
	CWaitCursor wait;

	BYTE* buf = m_UndoStates[m_UndoPointer]->GetData(true);
	if(buf && ProcessUndoBuffer(buf)){
		PurgeLastUndo();
		return true;
	}
	else{
		MessageBox("Cannot undo changes", "Error", MB_OK|MB_ICONERROR);
		return false;
	}
}


//////////////////////////////////////////////////////////////////////////
void CDCGFView::PurgeLastUndo()
{
	if(m_UndoPointer < m_UndoStates.GetSize()){
		delete m_UndoStates[m_UndoPointer];
		m_UndoStates.RemoveAt(m_UndoPointer);
		m_UndoPointer--;
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT CDCGFView::SetUndoState(CString Name)
{
	CBDynBuffer* buf = new CBDynBuffer(Game);
	FillUndoBuffer(buf);
	CUndoState* state = new CUndoState;
	if(FAILED(state->SetData(buf->m_Buffer, buf->GetSize(), Name))){
		delete state;
		delete buf;
		return E_FAIL;
	}

	m_UndoPointer++;
	m_UndoStates.SetAtGrow(m_UndoPointer, state);

	GetDocument()->SetModifiedFlag(TRUE);

	delete buf;
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
void CDCGFView::OnEditUndo() 
{
	Undo();	
}


//////////////////////////////////////////////////////////////////////////
void CDCGFView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_UndoPointer>=0);
}


//////////////////////////////////////////////////////////////////////////
// O V E R R I D E S
//////////////////////////////////////////////////////////////////////////
bool CDCGFView::FillUndoBuffer(CBDynBuffer *buf)
{
	return false;
}


//////////////////////////////////////////////////////////////////////////
bool CDCGFView::ProcessUndoBuffer(BYTE *buf)
{
	return false;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CDCGFView::RenderFrame()
{
	HRESULT ret = S_OK;
	if (Game->m_Renderer->m_Windowed && Game->m_Renderer->m_Ready){

		m_ViewReady = true;

		// init loop
		Game->m_Renderer->OnWindowChange();
		Game->InitLoop();		

		if(FAILED(Game->m_Renderer->Fill(0, 0, 0))) return E_FAIL;

		// display quick messages
		Game->m_Renderer->Setup2D();		
		Game->DisplayQuickMsg();

		// flip
		ret = Game->m_Renderer->Flip();	
	}

	return ret;

}
