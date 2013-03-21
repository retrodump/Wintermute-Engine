// ProjectView.cpp : implementation of the CProjectView class
//

#include "stdafx.h"
#include "ProjectMan.h"

#include "ProjectDoc.h"
#include "ProjectView.h"
#include "MainFrm.h"

#include "ViewHint.h"
#include "ViewLog.h"
#include "ViewProps.h"
#include "ViewTree.h"
#include "PropSheetEx.h"

#include "PagePackage.h"
#include "PagePlugins.h"
#include "PageFilters.h"
#include "PageVista.h"
#include "../MFCExt/utils_mfc.h"
#include <Shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProjectView

IMPLEMENT_DYNCREATE(CProjectView, CDCGFView)

BEGIN_MESSAGE_MAP(CProjectView, CDCGFView)
	//{{AFX_MSG_MAP(CProjectView)
	ON_COMMAND(ID_PROJECT_SETTINGS, OnProjectSettings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProjectView construction/destruction


//////////////////////////////////////////////////////////////////////////
CProjectView::CProjectView()
{
	// TODO: add construction code here
	m_ViewHint = NULL;
	m_ViewLog = NULL;
	m_ViewProps = NULL;
	m_ViewTree = NULL;

	m_Scene = NULL;
	m_Actor = NULL;
	m_Actor3D = NULL;
	m_Actor3DX = NULL;
	m_Entity = NULL;
	m_Sprite = NULL;
	m_Window = NULL;
	m_Font = NULL;
	m_Sound = NULL;
	m_SoundReady = false;

	m_Filename = "";
	m_Message = "";

	m_TalkFile = "";
	m_TalkReady = false;

	m_InitialRefreshDone = false;

	m_Rect.SetRectEmpty();

	m_ThreadWorking = false;
	m_ScheduledFilename = "";

	m_LastActorTime = 0;

	m_SampleText = LOC("/str0131/abcdefghijklmnopqrstuvwxyz\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n0123456789.:,;(*!?')\n\nThe quick brown fox jumps over the lazy dog. 1234567890");
}


//////////////////////////////////////////////////////////////////////////
CProjectView::~CProjectView()
{	
	Cleanup();
}


//////////////////////////////////////////////////////////////////////////
void CProjectView::Cleanup()
{
	SAFE_DELETE(m_Scene);
	SAFE_DELETE(m_Actor);
	SAFE_DELETE(m_Entity);
	SAFE_DELETE(m_Sprite);
	SAFE_DELETE(m_Window);
	SAFE_DELETE(m_Font);
	SAFE_DELETE(m_Sound);
	m_Actor3D = NULL;
	m_Actor3DX = NULL;

	m_Filename = "";
	m_Message = "";
	m_SoundReady = false;
	
	m_TalkFile = "";
	m_TalkReady = false;
}


//////////////////////////////////////////////////////////////////////////
BOOL CProjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CDCGFView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CProjectView drawing


//////////////////////////////////////////////////////////////////////////
void CProjectView::OnDraw(CDC* pDC)
{
	if(!m_InitialRefreshDone){
		m_InitialRefreshDone = true;

		m_ViewHint->m_View = this;
		m_ViewLog->m_View = this;
		m_ViewProps->m_View = this;
		m_ViewTree->m_View = this;

		// redraw
		m_ViewProps->Redraw();
		m_ViewTree->Redraw();

		Game->m_Registry->SetIniName(GetDocument()->Game->m_Registry->GetIniName());
		Game->m_FileManager->SetBasePath(NULL);

		m_ViewHint->DisplayHint("generic.html");

		((CMainFrame*)AfxGetMainWnd())->UpdateMenuURL(AfxGetMainWnd()->GetMenu());
	}

	if(!Game->m_Renderer) return;

	CProjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);


	CSize size;
	size.cx = max(m_Rect.Width(), Game->m_Renderer->m_Width);
	size.cy = max(m_Rect.Height(), Game->m_Renderer->m_Height);


	int mode;
	CSize OldSize, PageSize, LineSize;
	GetDeviceScrollSizes(mode, OldSize, PageSize, LineSize);

	if(size!=OldSize) SetScrollSizes(MM_TEXT, size);


	CDCGFView::OnDraw(pDC);
}


//////////////////////////////////////////////////////////////////////////
void CProjectView::OnInitialUpdate()
{
	CDCGFView::OnInitialUpdate();

	Game->m_FontStorage->RemoveFont(Game->m_SystemFont);
	Game->m_SystemFont = Game->m_FontStorage->AddFont("outline_green.fnt");

	Game->LOG(0, "----- Open file: '%s' -----", LPCSTR(GetDocument()->GetPathName()));

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CProjectView diagnostics

#ifdef _DEBUG
void CProjectView::AssertValid() const
{
	CDCGFView::AssertValid();
}

void CProjectView::Dump(CDumpContext& dc) const
{
	CDCGFView::Dump(dc);
}

CProjectDoc* CProjectView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CProjectDoc)));
	return (CProjectDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CProjectView message handlers


//////////////////////////////////////////////////////////////////////////
void CProjectView::OnPackageChange(CString Package)
{
	CProjectDoc* Doc = GetDocument();
	if(Doc){
		if(Package=="") m_ViewProps->SetCurrentPackage(NULL);
		else{
			for(int i=0; i<Doc->m_Packages.GetSize(); i++){
				if(Package.CompareNoCase(Doc->m_Packages[i]->m_Folder)==0){
					m_ViewProps->SetCurrentPackage(Doc->m_Packages[i]);
					return;
				}
			}			
		}
	}
	m_ViewProps->SetCurrentPackage(NULL);
}


//////////////////////////////////////////////////////////////////////////
void CProjectView::OnProjectSettings() 
{
	CProjectDoc* Doc = GetDocument();
	if(!Doc) return;
	
	CPropSheetEx dlgPropertySheet(LOC("/str0132/Project settings"));
	CPagePackage PagePackage;
	CPageFilters PageFilters;
	CPagePlugins PagePlugins;
	CPageVista PageVista;
	
	//////////////////////////////////////////////////////////////////////////
	// package page
	PagePackage.m_ProjectRoot = Doc->m_ProjectRoot;

	PagePackage.m_OutputDir = Doc->m_PackOutputFolder;

	PagePackage.m_CopyExe = (BOOL)Doc->m_PackCopyExe;
	PagePackage.m_ChangeIcon = (BOOL)Doc->m_PackChangeIcon;
	PagePackage.m_ExeName = Doc->m_PackExeName;
	PagePackage.m_IconName = Doc->m_PackIconName;

	PagePackage.m_BindPackage = Doc->m_BindPackage!="";
	PagePackage.m_SelectedPackage = Doc->m_BindPackage;

	PagePackage.m_CopySet = (BOOL)Doc->m_PackCopySet;
	PagePackage.m_ChangeIconSet = (BOOL)Doc->m_PackChangeIconSet;
	PagePackage.m_SetName = Doc->m_PackSetName;
	PagePackage.m_IconNameSet = Doc->m_PackIconNameSet;
	PagePackage.m_CopyD3DX = (BOOL)Doc->m_PackCopyD3DX;

	PagePackage.m_AddCrashLib = Doc->m_PackAddCrashLib;
	PagePackage.m_AddFpsDisplay = Doc->m_PackAddFpsDisplay;
	PagePackage.m_AddDebugMode = Doc->m_PackAddDebugMode;

	for(int j=0; j<Doc->m_Packages.GetSize(); j++) PagePackage.AddPackage(Doc->m_Packages[j]->m_Folder);

	dlgPropertySheet.AddPage(&PagePackage);

	//////////////////////////////////////////////////////////////////////////
	// filters page
	CString MaskList;
	for(int i=0; i<Doc->m_Filters.GetSize(); i++){
		CPackagerFilter* filter = Doc->m_Filters[i];

		if(filter->m_Type==CPackagerFilter::FILTER_EXCLUDE){
			PageFilters.m_ActiveExclude = filter->m_Active;
			MaskList = "";
			for(int j=0; j<filter->m_Masks.GetSize(); j++){
				if(j!=0) MaskList+=";";
				MaskList+=filter->m_Masks[j];
			}
			PageFilters.m_MasksExclude = MaskList;
		}

		else if(Doc->m_Filters[i]->m_Type==CPackagerFilter::FILTER_SCRIPT){
			PageFilters.m_ActiveScript = filter->m_Active;
			MaskList = "";
			for(int j=0; j<filter->m_Masks.GetSize(); j++){
				if(j!=0) MaskList+=";";
				MaskList+=filter->m_Masks[j];
			}
			PageFilters.m_MasksScript = MaskList;
		}

		else if(Doc->m_Filters[i]->m_Type==CPackagerFilter::FILTER_UNCOMPRESS){
			PageFilters.m_ActiveUncompress = filter->m_Active;
			MaskList = "";
			for(int j=0; j<filter->m_Masks.GetSize(); j++){
				if(j!=0) MaskList+=";";
				MaskList+=filter->m_Masks[j];
			}
			PageFilters.m_MasksUncompress = MaskList;
		}

		else if(Doc->m_Filters[i]->m_Type==CPackagerFilter::FILTER_COPY){
			PageFilters.m_ActiveCopy = filter->m_Active;
			MaskList = "";
			for(int j=0; j<filter->m_Masks.GetSize(); j++){
				if(j!=0) MaskList+=";";
				MaskList+=filter->m_Masks[j];
			}
			PageFilters.m_MasksCopy = MaskList;
		}	
	}
	dlgPropertySheet.AddPage(&PageFilters);


	//////////////////////////////////////////////////////////////////////////
	// plugins page
	PagePlugins.m_Game = Doc->Game;
	PagePlugins.m_SelectedPlugins.Copy(Doc->m_PluginList);
	
	dlgPropertySheet.AddPage(&PagePlugins);

	//////////////////////////////////////////////////////////////////////////
	// vista page
	PageVista.m_ProjectRoot = Doc->m_ProjectRoot;

	PageVista.m_AddGeData = Doc->m_PackAddGeData;
	PageVista.m_GdfFile = Doc->m_PackGdfFile;
	PageVista.m_ThumbnailFile = Doc->m_PackGdfThumbnail;

	dlgPropertySheet.AddPage(&PageVista);



	if(dlgPropertySheet.DoModal() == IDOK)
	{
		for(int i=0; i<Doc->m_Filters.GetSize(); i++){
			CPackagerFilter* filter = Doc->m_Filters[i];

			if(filter->m_Type==CPackagerFilter::FILTER_EXCLUDE){
				filter->m_Active = (PageFilters.m_ActiveExclude!=FALSE);
				filter->m_Masks.RemoveAll();
				for(int j=1; j<=NumEntries(PageFilters.m_MasksExclude, ';'); j++){
					filter->m_Masks.Add(Entry(j, PageFilters.m_MasksExclude, ';'));
				}
			}

			else if(filter->m_Type==CPackagerFilter::FILTER_SCRIPT){
				filter->m_Active = (PageFilters.m_ActiveScript!=FALSE);
				filter->m_Masks.RemoveAll();
				for(int j=1; j<=NumEntries(PageFilters.m_MasksScript, ';'); j++){
					filter->m_Masks.Add(Entry(j, PageFilters.m_MasksScript, ';'));
				}
			}

			else if(filter->m_Type==CPackagerFilter::FILTER_UNCOMPRESS){
				filter->m_Active = (PageFilters.m_ActiveUncompress!=FALSE);
				filter->m_Masks.RemoveAll();
				for(int j=1; j<=NumEntries(PageFilters.m_MasksUncompress, ';'); j++){
					filter->m_Masks.Add(Entry(j, PageFilters.m_MasksUncompress, ';'));
				}
			}

			else if(filter->m_Type==CPackagerFilter::FILTER_COPY){
				filter->m_Active = (PageFilters.m_ActiveCopy!=FALSE);
				filter->m_Masks.RemoveAll();
				for(int j=1; j<=NumEntries(PageFilters.m_MasksCopy, ';'); j++){
					filter->m_Masks.Add(Entry(j, PageFilters.m_MasksCopy, ';'));
				}
			}
		}

		char RelPath[MAX_PATH];
		if(!PathRelativePathTo(RelPath, Doc->m_ProjectRoot, FILE_ATTRIBUTE_NORMAL, PagePackage.m_OutputDir, FILE_ATTRIBUTE_NORMAL)){
			strcpy(RelPath, LPCSTR(PagePackage.m_OutputDir));
		}		
		Doc->m_PackOutputFolder = RelPath;

		Doc->m_PackCopyExe = (PagePackage.m_CopyExe!=FALSE);
		Doc->m_PackExeName = PagePackage.m_ExeName;

		Doc->m_PackChangeIcon = (PagePackage.m_ChangeIcon!=FALSE);
		if(!PathRelativePathTo(RelPath, Doc->m_ProjectRoot, FILE_ATTRIBUTE_NORMAL, PagePackage.m_IconName, FILE_ATTRIBUTE_NORMAL)){
			strcpy(RelPath, LPCSTR(PagePackage.m_IconName));
		}		
		Doc->m_PackIconName = RelPath;


		Doc->m_PackAddCrashLib = (PagePackage.m_AddCrashLib!=FALSE);
		Doc->m_PackAddFpsDisplay = (PagePackage.m_AddFpsDisplay!=FALSE);
		Doc->m_PackAddDebugMode = (PagePackage.m_AddDebugMode!=FALSE);


		if(PagePackage.m_BindPackage!=FALSE){
			Doc->m_BindPackage = PagePackage.m_SelectedPackage;
		}
		else Doc->m_BindPackage = "";


		Doc->m_PackCopySet = (PagePackage.m_CopySet!=FALSE);
		Doc->m_PackSetName = PagePackage.m_SetName;

		Doc->m_PackCopyD3DX = (PagePackage.m_CopyD3DX!=FALSE);

		Doc->m_PackChangeIconSet = (PagePackage.m_ChangeIconSet!=FALSE);
		if(!PathRelativePathTo(RelPath, Doc->m_ProjectRoot, FILE_ATTRIBUTE_NORMAL, PagePackage.m_IconNameSet, FILE_ATTRIBUTE_NORMAL)){
			strcpy(RelPath, LPCSTR(PagePackage.m_IconNameSet));
		}		
		Doc->m_PackIconNameSet = RelPath;


		Doc->m_PluginList.Copy(PagePlugins.m_SelectedPlugins);

		Doc->m_PackAddGeData = (PageVista.m_AddGeData!=FALSE);
		Doc->m_PackGdfFile = PageVista.m_GdfFile;
		Doc->m_PackGdfThumbnail = PageVista.m_ThumbnailFile;

		Doc->SetModifiedFlag(TRUE);
	}
}


//////////////////////////////////////////////////////////////////////////
void CProjectView::SelectFile(CString Filename)
{
	CProjectDoc* doc = GetDocument();
	if(doc){
		Filename = doc->GetFullPath(Filename);
		if(m_ViewTree) m_ViewTree->SelectFile(Filename);
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT CProjectView::PreviewFile(CString Filename)
{
	if(m_ThreadWorking){
		m_ScheduledFilename = Filename;
		return S_OK;
	}
	
	Filename = GetRelativePath(Game, Filename);
	if(m_Filename!="" && Filename !="" && Filename==m_Filename) return S_OK;

	//Game->LOG(0, "Cleanup");
	Cleanup();
	m_Filename = Filename;

	if(!GetDocument()->m_EditorPreview){
		m_Message = LOC("/str0133/Preview is disabled");
		m_Filename = "";
		return S_OK;
	}

	m_ScheduledFilename = "";
	m_ThreadWorking = true;
	AfxBeginThread(LoaderThread, this, THREAD_PRIORITY_NORMAL);

	return S_OK;
}



//////////////////////////////////////////////////////////////////////////
HRESULT CProjectView::OnDrawContent()
{
	if(m_ThreadWorking){
		Game->m_SystemFont->DrawText((BYTE*)LOC("/str0134/Loading preview..."), 0, 0, Game->m_Renderer->m_Width);
		return S_OK;
	}

	
	if(m_Message!=""){
		Game->m_SystemFont->DrawText((BYTE*)LPCSTR(m_Message), 0, (Game->m_Renderer->m_Height - Game->m_SystemFont->GetTextHeight((BYTE*)" ", 1000)) / 2, Game->m_Renderer->m_Width, TAL_CENTER);
	}	
	if(!GetDocument()->m_EditorPreview) return S_OK;
	

	CPoint p = GetScrollPosition();
	int mode;
	CSize TotalSize, PageSize, LineSize;
	GetDeviceScrollSizes(mode, TotalSize, PageSize, LineSize);

	CBSprite* Sprite = NULL;
	bool IgnoreHotspot = false;
	if(m_Sprite) Sprite = m_Sprite;
	if(m_Entity && m_Entity->m_Sprite) Sprite = m_Entity->m_Sprite;
	if(m_Actor){
		CAdSpriteSet* SpriteSet = m_Actor->m_StandSprite;
		if(!SpriteSet) SpriteSet = m_Actor->GetAnimByName("idle");

		if(SpriteSet){
			if(timeGetTime() - m_LastActorTime >= 2000){
				m_Actor->m_Dir = (TDirection)(m_Actor->m_Dir + 1);
				m_LastActorTime = timeGetTime();
			}
			int NumTries=0;
			while(Sprite==NULL && NumTries<NUM_DIRECTIONS){
				if(m_Actor->m_Dir>=NUM_DIRECTIONS) m_Actor->m_Dir = (TDirection)0;
				if(SpriteSet->m_Sprites[m_Actor->m_Dir]==NULL){
					NumTries++;
					m_Actor->m_Dir = (TDirection)(m_Actor->m_Dir + 1);
				}
				else{
					Sprite = SpriteSet->m_Sprites[m_Actor->m_Dir];
					IgnoreHotspot = true;
				}
			}
		}
	}



	//////////////////////////////////////////////////////////////////////////
	if(Sprite){
		CPoint pos;
		pos.x = (TotalSize.cx - m_Rect.Width())/2 - p.x;
		pos.y = (TotalSize.cy - m_Rect.Height())/2 - p.y;

		if(!IgnoreHotspot)
			GetSpriteHotspot(Sprite, &m_SpriteHotspot.x, &m_SpriteHotspot.y);
		
		pos.x += m_SpriteHotspot.x;
		pos.y += m_SpriteHotspot.y;

		Sprite->GetCurrentFrame();
		Sprite->Display(pos.x, pos.y);

		if(m_SpriteHotspot.x != 0 || m_SpriteHotspot.y != 0){
			Game->m_Renderer->SetupLines();
			Game->m_Renderer->DrawLine(pos.x - 2, pos.y, pos.x + 3, pos.y, 0xFFFF0000);
			Game->m_Renderer->DrawLine(pos.x, pos.y - 2, pos.x, pos.y + 3, 0xFFFF0000);
			Game->m_Renderer->Setup2D();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	if(m_Scene){
		//m_Scene->m_OffsetLeft = p.x;
		//m_Scene->m_OffsetTop = p.y;
		m_Scene->SetOffset(p.x, p.y);

		if(m_Actor3D)
		{
			if(timeGetTime() - m_LastActorTime >= 50){
				m_Actor3D->m_Angle = CBUtils::NormalizeAngle(m_Actor3D->m_Angle + 5);
				m_LastActorTime = timeGetTime();
			}
		}

		if(m_Actor3DX)
		{
			if(timeGetTime() - m_LastActorTime >= 50){
				m_Actor3DX->m_Angle = CBUtils::NormalizeAngle(m_Actor3DX->m_Angle + 5);
				m_LastActorTime = timeGetTime();
			}
		}

		m_Scene->Update();
		m_Scene->Display();
	}

	//////////////////////////////////////////////////////////////////////////
	if(m_Window){
		CPoint pos;
		pos.x = (TotalSize.cx - m_Rect.Width())/2 - p.x;
		pos.y = (TotalSize.cy - m_Rect.Height())/2 - p.y;
		
		m_Window->Display(pos.x, pos.y);
	}

	//////////////////////////////////////////////////////////////////////////
	if(m_Sound){
		if(m_SoundReady){
			m_Sound->Play();
			m_SoundReady = false;
		}
		else{
			if(!m_Sound->IsPlaying()) m_Message = "";
		}
	}

	//////////////////////////////////////////////////////////////////////////
	if(m_TalkFile!=""){
		if(!m_TalkReady){
			m_Entity = ::new CAdEntity(Game);

			m_Entity->m_PosX = TotalSize.cx / 2;
			m_Entity->m_PosY = TotalSize.cy / 2;


			CString SoundName;
			int i = m_Filename.Find(".talk");
			SoundName = m_Filename.Left(i) + ".wav";

			m_Entity->Talk("", (char*)(LPCSTR)SoundName, 0, "");
			m_TalkReady = true;
		}
		else if(m_Entity){
			m_Entity->Update();
			m_Entity->Display();
			((CAdGame*)Game)->DisplaySentences(true);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	if(m_Font) m_Font->DrawText((BYTE*)LPCSTR(m_SampleText), 0, 30, Game->m_Renderer->m_Width);



	if(m_Filename!=""){
		CString Title = CString(LOC("/str0135/File")) + ": " + m_Filename;
		Game->m_SystemFont->DrawText((BYTE*)LPCSTR(Title), 0, 0, Game->m_Renderer->m_Width);
	}

	if(m_ScheduledFilename!=""){
		PreviewFile(m_ScheduledFilename);
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
UINT CProjectView::LoaderThread(LPVOID Param)
{
	CProjectView* _this = (CProjectView*)Param;
	if(_this == NULL ||!_this->IsKindOf(RUNTIME_CLASS(CProjectView))){
		return 1;
	}

	_this->m_RenderingDisabled = true;
	//_this->Game->LOG(0, "Loader start");
	CString Ext = GetExt(_this->m_Filename);

	_this->Game->SetEngineLogCallback(CProjectView::EngineLogCallback, (void*)_this);

	//////////////////////////////////////////////////////////////////////////
	if(Ext.CompareNoCase("sprite")==0 || Ext.CompareNoCase("bmp")==0  || Ext.CompareNoCase("tga")==0 || Ext.CompareNoCase("png")==0 || Ext.CompareNoCase("jpg")==0){
		_this->m_Sprite = ::new CBSprite(_this->Game);
		if(!_this->m_Sprite || FAILED(_this->m_Sprite->LoadFile((char*)LPCSTR(_this->m_Filename)))){
			SAFE_DELETE(_this->m_Sprite);
			_this->m_Message = LOC("/str0136/Error loading sprite");
		}
		else{
			_this->m_Sprite->GetBoundingRect(&_this->m_Rect, 0, 0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	else if(Ext.CompareNoCase("scene")==0){
		//_this->Game->LOG(0, "Load scene");
		_this->m_Scene = ::new CAdScene(_this->Game);
		if(!_this->m_Scene || FAILED(_this->m_Scene->LoadFile((char*)LPCSTR(_this->m_Filename)))){
			SAFE_DELETE(_this->m_Scene);
			_this->m_Message = LOC("/str0137/Error loading scene");
		}
		else{
			int width = 0, height = 0;
			for(int i=0; i<_this->m_Scene->m_Layers.GetSize(); i++){
				if(!_this->m_Scene->m_Layers[i]->m_Active) continue;
				width = max(width, _this->m_Scene->m_Layers[i]->m_Width);
				height = max(height, _this->m_Scene->m_Layers[i]->m_Height);
			}
			_this->m_Rect.SetRect(0, 0, width, height);

			_this->m_Scene->m_ParalaxScrolling = false;
			_this->m_Scene->m_AutoScroll = false;
			_this->m_Scene->m_ShowGeometry = false;
		}
	}


	//////////////////////////////////////////////////////////////////////////
	else if(Ext.CompareNoCase("act3d")==0){
		_this->m_Scene = ::new CAdScene(_this->Game);
		if(!_this->m_Scene || FAILED(_this->m_Scene->LoadFile("preview.scene"))){
			SAFE_DELETE(_this->m_Scene);
			_this->m_Message = LOC("/str0137/Error loading scene");
		}
		else{
			int width = 0, height = 0;
			for(int i=0; i<_this->m_Scene->m_Layers.GetSize(); i++){
				if(!_this->m_Scene->m_Layers[i]->m_Active) continue;
				width = max(width, _this->m_Scene->m_Layers[i]->m_Width);
				height = max(height, _this->m_Scene->m_Layers[i]->m_Height);
			}
			_this->m_Rect.SetRect(0, 0, width, height);

			_this->m_Scene->m_ParalaxScrolling = false;
			_this->m_Scene->m_AutoScroll = false;


			CAdObject3D* Act = CAdActor3D::CreateFromFile(_this->Game, (char*)LPCSTR(_this->m_Filename));
			if(!Act)
			{
				_this->m_Message = LOC("/str0138/Error loading actor");
			}
			else
			{
				if(strcmp(Act->GetClassName(), "CAdActor3D")==0)
				{
					_this->m_Actor3D = (CAdActor3D*)Act;
					_this->m_Scene->AddObject(_this->m_Actor3D);
					_this->m_Actor3D->SkipTo(130, 460);
					_this->m_Actor3D->m_Active = true;
				}
				else
				{
					_this->m_Actor3DX = (CAdActor3DX*)Act;
					_this->m_Scene->AddObject(_this->m_Actor3DX);
					_this->m_Actor3DX->SkipTo(130, 460);
					_this->m_Actor3DX->m_Active = true;
				}
			}
		}
	}


	//////////////////////////////////////////////////////////////////////////
	else if(Ext.CompareNoCase("actor")==0){
		_this->m_Actor = ::new CAdActor(_this->Game);
		if(!_this->m_Actor || FAILED(_this->m_Actor->LoadFile((char*)LPCSTR(_this->m_Filename)))){
			SAFE_DELETE(_this->m_Actor);
			_this->m_Message = LOC("/str0138/Error loading actor");
		}
		else{
			_this->m_Actor->m_Dir = DI_DOWN;
			_this->m_Rect.SetRectEmpty();
			_this->m_LastActorTime = timeGetTime();
			CAdSpriteSet* Spriteset = _this->m_Actor->m_StandSprite;
			if(!Spriteset) Spriteset = _this->m_Actor->GetAnimByName("idle");
			if(Spriteset){
				for(int i=0; i<NUM_DIRECTIONS; i++){
					if(Spriteset->m_Sprites[i]){
						Spriteset->m_Sprites[i]->GetBoundingRect(&_this->m_Rect, 0, 0);
						_this->GetSpriteHotspot(Spriteset->m_Sprites[i], &_this->m_SpriteHotspot.x, &_this->m_SpriteHotspot.y);
						break;
					}
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	else if(Ext.CompareNoCase("entity")==0){
		_this->m_Entity = ::new CAdEntity(_this->Game);
		if(!_this->m_Entity || FAILED(_this->m_Entity->LoadFile((char*)LPCSTR(_this->m_Filename)))){
			SAFE_DELETE(_this->m_Entity);
			_this->m_Message = LOC("/str0139/Error loading entity");
		}
		else{
			if(_this->m_Entity->m_Sprite){
				_this->m_Entity->m_Sprite->GetBoundingRect(&_this->m_Rect, 0, 0);
			}
			else _this->m_Rect.SetRectEmpty();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	else if(Ext.CompareNoCase("talk")==0){
		_this->m_TalkFile = _this->m_Filename;
		_this->m_Rect.SetRectEmpty();
	}

	//////////////////////////////////////////////////////////////////////////
	else if(Ext.CompareNoCase("font")==0 || Ext.CompareNoCase("fnt")==0)
	{
		_this->m_Font = CBFont::CreateFromFile(_this->Game, (char*)LPCSTR(_this->m_Filename));
		if(!_this->m_Font) _this->m_Message = LOC("/str0140/Error loading font");
		else _this->m_Rect.SetRectEmpty();
		/*
		_this->m_Font = ::new CBFont(_this->Game);
		if(!_this->m_Font || FAILED(_this->m_Font->LoadFile((char*)LPCSTR(_this->m_Filename)))){
			SAFE_DELETE(_this->m_Font);
			_this->m_Message = LOC("/str0140/Error loading font");
		}
		else{
			_this->m_Rect.SetRectEmpty();
		}
		*/
	}

	//////////////////////////////////////////////////////////////////////////
	else if(Ext.CompareNoCase("window")==0){
		_this->m_Window = ::new CUIWindow(_this->Game);
		if(!_this->m_Window || FAILED(_this->m_Window->LoadFile((char*)LPCSTR(_this->m_Filename)))){
			SAFE_DELETE(_this->m_Window);
			_this->m_Message = LOC("/str0141/Error loading window");
		}
		else{
			_this->m_Window->m_PosX = 0;
			_this->m_Window->m_PosY = 0;
			_this->m_Rect.SetRect(0, 0, _this->m_Window->m_Width, _this->m_Window->m_Height);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	else if(Ext.CompareNoCase("wav")==0 || Ext.CompareNoCase("ogg")==0){
		_this->m_Sound = ::new CBSound(_this->Game);
		if(!_this->m_Sound || FAILED(_this->m_Sound->SetSound((char*)LPCSTR(_this->m_Filename), SOUND_SFX, true))){
			SAFE_DELETE(_this->m_Sound);
			_this->m_Message = LOC("/str0142/Error loading sound");
		}
		else{
			_this->m_Message = LOC("/str0143/Playing the sound...");
			_this->m_Rect.SetRectEmpty();
			_this->m_SoundReady = true;
		}
	}


	else{
		_this->m_Message = LOC("/str0144/Preview not available");
		_this->m_Rect.SetRectEmpty();
	}


	_this->Game->SetEngineLogCallback();
	_this->m_ThreadWorking = false;
	_this->m_RenderingDisabled = false;

	//_this->Game->LOG(0, "Loader end");
	return 0;
}


//////////////////////////////////////////////////////////////////////////
void CProjectView::GetSpriteHotspot(CBSprite* Sprite, LONG* X, LONG* Y)
{
	*X = 0;
	*Y = 0;
	if(Sprite->m_Frames.GetSize() > 0){
		CBFrame* f = Sprite->m_Frames[0];
		if(f->m_Subframes.GetSize()>0){
			*X = f->m_Subframes[0]->m_HotspotX;
			*Y += f->m_Subframes[0]->m_HotspotY;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void _stdcall CProjectView::EngineLogCallback(char *Text, HRESULT Result, void *Data)
{
	CProjectView* _this = (CProjectView*)Data;
	if(_this) _this->GetDocument()->AddLog(CString(Text));
}
