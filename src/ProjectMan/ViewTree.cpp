// ViewTree.cpp : implementation file
//

#include "stdafx.h"
#include "ProjectMan.h"
#include "ProjectView.h"
#include "ViewTree.h"
#include "ViewProps.h"
#include "TreeItem.h"
#include "FolderDlg.h"
#include "resource.h"
#include "../MFCExt/utils_mfc.h"
#include "../MFCExt/objects.h"
#include "../MFCExt/BrowseForFolder.h"
#include "../MFCExt/CShellFileOp.h"
#include "../MFCExt/TemplateDlg.h"
#include ".\viewtree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CViewTree

IMPLEMENT_DYNCREATE(CViewTree, CFormViewEx)


//////////////////////////////////////////////////////////////////////////
CViewTree::CViewTree()
	: CFormViewEx(CViewTree::IDD)
{
	//{{AFX_DATA_INIT(CViewTree)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_View = NULL;
	m_ScheduledEdit = false;
	m_Redrawing = false;

	m_LastImportDir = "";

	m_ScriptError = "";
	m_ShowHiddenFiles = false;
}


//////////////////////////////////////////////////////////////////////////
CViewTree::~CViewTree()
{
	m_View = NULL;
	m_ExpandedList.RemoveAll();
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::DoDataExchange(CDataExchange* pDX)
{
	CFormViewEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewTree)
	DDX_Control(pDX, IDC_PREVIEW, m_Preview);
	DDX_Control(pDX, IDC_TREE, m_Tree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewTree, CFormViewEx)
	//{{AFX_MSG_MAP(CViewTree)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE, OnItemExpanding)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnSelchangedTree)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE, OnDblclkTree)
	ON_NOTIFY(NM_RCLICK, IDC_TREE, OnRclickTree)
	ON_COMMAND(ID_POPUP_TREE_OPEN, OnPopupTreeOpen)
	ON_COMMAND(ID_POPUP_TREE_EDIT, OnPopupTreeEdit)
	ON_COMMAND(ID_POPUP_TREE_CREATE_FOLDER, OnPopupTreeCreateFolder)
	ON_COMMAND(ID_POPUP_TREE_REFRESH, OnPopupTreeRefresh)
	ON_COMMAND(ID_POPUP_TREE_EXPLORE, OnPopupTreeExplore)
	ON_COMMAND(ID_POPUP_TREE_FOLDER_TOP, OnPopupTreeFolderTop)
	ON_COMMAND(ID_POPUP_TREE_REMOVE, OnPopupTreeRemove)
	ON_COMMAND(ID_POPUP_IMPORT_FOLDER, OnPopupImportFolder)
	ON_COMMAND(ID_POPUP_IMPORT_FILE, OnPopupImportFile)
	ON_COMMAND(ID_POPUP_TREE_PROMOTE, OnPopupTreePromote)
	ON_COMMAND(ID_POPUP_TREE_DEGRADE, OnPopupTreeDegrade)
	ON_COMMAND(ID_POPUP_TREE_COMPILE, OnPopupTreeCompile)
	ON_COMMAND(ID_POPUP_TREE_PRIMARY_PACKAGE, OnPopupTreePrimaryPackage)
	ON_COMMAND(ID_POPUP_ADD_ACTOR, OnPopupAddActor)
	ON_COMMAND(ID_POPUP_ADD_ENTITY, OnPopupAddEntity)
	ON_COMMAND(ID_POPUP_ADD_FONT, OnPopupAddFont)
	ON_COMMAND(ID_POPUP_ADD_SCENE, OnPopupAddScene)
	ON_COMMAND(ID_POPUP_ADD_SCRIPT, OnPopupAddScript)
	ON_COMMAND(ID_POPUP_ADD_SPRITE, OnPopupAddSprite)
	ON_COMMAND(ID_POPUP_ADD_WINDOW, OnPopupAddWindow)
	ON_BN_CLICKED(IDC_PREVIEW, OnPreview)
	ON_COMMAND(ID_POPUP_TREE_COPY, OnPopupTreeCopy)
	ON_COMMAND(ID_POPUP_TREE_CHECK_SYNTAX, OnPopupTreeCheckSyntax)
	ON_COMMAND(ID_POPUP_TREE_STARTUP_SCENE, OnPopupTreeStartupScene)
	ON_COMMAND(ID_POPUP_TREE_STARTUP_SCENE_D, OnPopupTreeStartupSceneDebug)
	ON_COMMAND(ID_POPUP_ADD_ACTOR3D, OnPopupAddActor3d)
	//}}AFX_MSG_MAP	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTree diagnostics

#ifdef _DEBUG
void CViewTree::AssertValid() const
{
	CFormViewEx::AssertValid();
}

void CViewTree::Dump(CDumpContext& dc) const
{
	CFormViewEx::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewTree message handlers


//////////////////////////////////////////////////////////////////////////
BOOL CViewTree::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if(m_ScheduledEdit){
		m_ScheduledEdit = false;
		OnPopupTreeOpen();	
	}
	
	if(m_View && m_View->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)) return TRUE;
	else return CFormViewEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnSize(UINT nType, int cx, int cy) 
{
	SetScrollSizes(MM_TEXT, CSize(cx, cy));
	CFormViewEx::OnSize(nType, cx, cy);
	SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
		
	CRect rectClient;
	GetClientRect(&rectClient);

	CRect rectList;

	if(m_Tree.GetSafeHwnd()){
		m_Tree.GetWindowRect(&rectList);
		ScreenToClient(&rectList);
		rectList.right = rectList.left + (rectClient.right - rectClient.left);
		rectList.bottom = (rectClient.bottom - rectClient.top);
		m_Tree.MoveWindow(&rectList);
	}
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnInitialUpdate() 
{
	CFormViewEx::OnInitialUpdate();

	CImageList* pImageList = new CImageList();
	pImageList->Create(IDB_NODES, 16, 1, RGB(255, 0, 255));

	m_Tree.SetImageList(pImageList, TVSIL_NORMAL);
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnDestroy() 
{
	CFormViewEx::OnDestroy();

	if(!m_Tree) return;
	
	DeleteSubitems(NULL);
	
	CImageList* pImageList;
	pImageList = m_Tree.GetImageList(TVSIL_NORMAL);
	pImageList->DeleteImageList();
	delete pImageList;
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::ExploreDir(CString Dir, HTREEITEM Parent)
{
	CProjectDoc* Doc = m_View->GetDocument();
	if(!Doc) return;
	
	TV_INSERTSTRUCT tvstruct;

	// is already filled?
	CTreeItem* ChildItem = NULL;
	HTREEITEM child = m_Tree.GetChildItem(Parent);
	if(child) ChildItem = (CTreeItem*) m_Tree.GetItemData(child);
	if(ChildItem) return;

	// remove any children of this directory
	DeleteSubitems(Parent);


	HTREEITEM hThisItem;
	
	CFileFind finder;
	BOOL Working = finder.FindFile(Dir + "*.*");
	while (Working){
		Working = finder.FindNextFile();

		CString StrFilename = finder.GetFileName();

		if(finder.IsDots()) continue;
		if(Parent==NULL && !finder.IsDirectory()) continue;

		if(finder.IsHidden() && !m_ShowHiddenFiles) continue;

		CTreeItem* item = FillItemInfo(Doc, finder, Parent);

		tvstruct.item.iImage = item->m_IconIndex;
		tvstruct.item.iSelectedImage = item->m_IconIndex;

		tvstruct.hParent = Parent;
		tvstruct.hInsertAfter = TVI_LAST;
		tvstruct.item.pszText = (char*)LPCSTR(StrFilename);
		tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;

		hThisItem = m_Tree.InsertItem(&tvstruct);

		item->m_Item = hThisItem;
		m_Tree.SetItemData(hThisItem, (DWORD)item);


		// add dummy sub-item when applicable
		if(finder.IsDirectory()){
			CFileFind finder2;
			BOOL Working2 = finder2.FindFile(Dir + StrFilename + "\\*.*");
			while(Working2){
				Working2 = finder2.FindNextFile();

				if(finder2.IsDots()) continue;
				if(finder2.IsHidden() && !m_ShowHiddenFiles) continue;
				else{
					tvstruct.hParent = hThisItem;
					tvstruct.hInsertAfter = TVI_LAST;
					tvstruct.item.pszText = "dummy";
					tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
					HTREEITEM DummyItem = m_Tree.InsertItem(&tvstruct);
					m_Tree.SetItemData(DummyItem, (DWORD)NULL);
					break;
				}
			}
		}


		if(item->m_IsDir && m_ExpandedList.Find(item->m_FullPath)!=NULL)
			m_Tree.Expand(hThisItem, TVE_EXPAND);
	}
	
	TVSORTCB tvs;
	tvs.hParent = Parent;
	tvs.lpfnCompare = TreeCompareProc;
	tvs.lParam = (LPARAM) &m_Tree;	
	m_Tree.SortChildrenCB(&tvs);
}


//////////////////////////////////////////////////////////////////////////
int CALLBACK CViewTree::TreeCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CTreeCtrl* Tree = (CTreeCtrl*) lParamSort;
	CTreeItem* Item1 = (CTreeItem*)lParam1;
	CTreeItem* Item2 = (CTreeItem*)lParam2;

	if(!Item1 || !Item2) return 0;

	if(Item1->m_IsPackage && !Item2->m_IsPackage) return -1;
	if(!Item1->m_IsPackage && Item2->m_IsPackage) return 1;

	if(Item1->m_IsDir && !Item2->m_IsDir) return -1;
	if(!Item1->m_IsDir && Item2->m_IsDir) return 1;

	return stricmp(Tree->GetItemText(Item1->m_Item), Tree->GetItemText(Item2->m_Item));
}


//////////////////////////////////////////////////////////////////////////
HRESULT CViewTree::Redraw(bool InitialRedraw)
{
	CWaitCursor cursor;

	m_Redrawing = true;
	
	GetExpandedItems(NULL);
	DeleteSubitems(NULL);

	if(!m_View) return E_FAIL;
	CProjectDoc* Doc = m_View->GetDocument();
	if(!Doc) return E_FAIL;

	m_Preview.SetCheck(Doc->m_EditorPreview);
	
	ExploreDir(Doc->m_ProjectRoot, NULL);

	if(InitialRedraw){
		CPackage* pkg = NULL;
		for(int i=0; i<Doc->m_Packages.GetSize(); i++){
			if(Doc->m_Packages[i]->m_Primary){
				pkg = Doc->m_Packages[i];
				break;
			}
		}
		if(!pkg && Doc->m_Packages.GetSize()>0) pkg = Doc->m_Packages[0];

		if(pkg){
			SelectFile(Doc->m_ProjectRoot + pkg->m_Folder);
			m_View->OnPackageChange(pkg->m_Folder);
		}

	}

	m_ExpandedList.RemoveAll();
	
	m_Redrawing = false;


	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CProjectDoc* Doc = m_View->GetDocument();
	if(!Doc){
		*pResult = 0;
		return;
	}

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	//if (pNMTreeView->hdr.code == TVN_ITEMEXPANDING){
	if(pNMTreeView->action == TVE_EXPAND){
	
		HTREEITEM hIT = pNMTreeView->itemNew.hItem;
		CString cstr, strPath;

		// build up the path to htreeitem
		strPath = m_Tree.GetItemText(hIT);

		while (m_Tree.GetParentItem(hIT)){
			
			hIT = m_Tree.GetParentItem(hIT);

			cstr.Format(_T("%s/%s"), (LPCTSTR)m_Tree.GetItemText(hIT), (LPCTSTR)strPath);
			strPath = cstr;
		}

		// use that dir to call ExploreDir
		ExploreDir(Doc->m_ProjectRoot + strPath + "\\", pNMTreeView->itemNew.hItem);
	}
	*pResult = 0;
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::DeleteSubitems(HTREEITEM Parent)
{
	HTREEITEM hIT = m_Tree.GetChildItem(Parent);
	HTREEITEM hTemp;

	while (hIT != NULL){
		hTemp = m_Tree.GetNextSiblingItem(hIT);
		CTreeItem* ItemData = (CTreeItem*)m_Tree.GetItemData(hIT);
		if(ItemData) delete ItemData;
		m_Tree.SetItemData(hIT, NULL);

		DeleteSubitems(hIT);
		m_Tree.DeleteItem(hIT);
		hIT = hTemp;
	}
}


//////////////////////////////////////////////////////////////////////////
CTreeItem* CViewTree::FillItemInfo(CProjectDoc *Doc, CFileFind& InFinder, HTREEITEM Parent)
{
	CTreeItem* item = new CTreeItem;

	item->m_ShortName = InFinder.GetFileName();
	item->m_FullPath = InFinder.GetFilePath();

	// is dir?
	if(InFinder.IsDirectory()){
		item->m_IsDir = true;
		// is package?
		if(Parent==NULL){
			item->m_IsTopLevel = true;
			for(int i=0; i<Doc->m_Packages.GetSize(); i++){
				if(InFinder.GetFileName().CompareNoCase(Doc->m_Packages[i]->m_Folder)==0){
					item->m_IsPackage = true;
					break;
				}
			}
		}
		
		if(item->m_IsPackage){
			if(Doc->IsPrimaryPackage(item->m_ShortName))
				item->m_IconIndex = ICON_P_PACKAGE;
			else
				item->m_IconIndex = ICON_PACKAGE;
		}
		else{
			// search for containded types
			CString KeyFile = "";
			item->m_Contains = GetFolderType(InFinder.GetFilePath(), KeyFile);
			item->m_IconIndex = ICON_FOLDER;
			item->m_KeyFile = KeyFile;

			if(item->m_Contains & DC_OBJECT_SCENE) item->m_IconIndex = ICON_FSCENE;
			else if(item->m_Contains & DC_OBJECT_ACTOR) item->m_IconIndex = ICON_FACTOR;
			else if(item->m_Contains & DC_OBJECT_ACTOR3D) item->m_IconIndex = ICON_FACTOR3D;
			else if(item->m_Contains & DC_OBJECT_ENTITY)  item->m_IconIndex = ICON_FENTITY;
			else if(item->m_Contains == DC_OBJECT_SCRIPT) item->m_IconIndex = ICON_FSCRIPT;
			else if(item->m_Contains == DC_OBJECT_SOUND)  item->m_IconIndex = ICON_FSOUND;
			else if(item->m_Contains == DC_OBJECT_IMAGE)  item->m_IconIndex = ICON_FIMAGE;
			else if(item->m_Contains == DC_OBJECT_SPRITE) item->m_IconIndex = ICON_FSPRITE;			
			else item->m_IconIndex = ICON_FOLDER;
		}
	}

	// single file
	else{
		item->m_IsObject = GetFileType(InFinder.GetFileName());
		
		if(item->m_IsObject & DC_OBJECT_ACTOR) item->m_IconIndex = ICON_ACTOR;
		else if(item->m_IsObject & DC_OBJECT_ENTITY)  item->m_IconIndex = ICON_ENTITY;
		else if(item->m_IsObject & DC_OBJECT_SCENE)   item->m_IconIndex = ICON_SCENE;
		else if(item->m_IsObject & DC_OBJECT_SCRIPT)  item->m_IconIndex = ICON_SCRIPT;
		else if(item->m_IsObject & DC_OBJECT_WINDOW)  item->m_IconIndex = ICON_WINDOW;
		else if(item->m_IsObject & DC_OBJECT_DEF)     item->m_IconIndex = ICON_DEF;
		else if(item->m_IsObject & DC_OBJECT_SOUND)   item->m_IconIndex = ICON_SOUND;
		else if(item->m_IsObject & DC_OBJECT_FONT)    item->m_IconIndex = ICON_FONT;
		else if(item->m_IsObject & DC_OBJECT_IMAGE)   item->m_IconIndex = ICON_IMAGE;
		else if(item->m_IsObject & DC_OBJECT_SPRITE)  item->m_IconIndex = ICON_SPRITE;
		else if(item->m_IsObject & DC_OBJECT_PACKAGE) item->m_IconIndex = ICON_COMP_PACK;
		else if(item->m_IsObject & DC_OBJECT_TALK)    item->m_IconIndex = ICON_TALK;
		else if(item->m_IsObject & DC_OBJECT_ACTOR3D) item->m_IconIndex = ICON_ACTOR3D;
		else item->m_IconIndex = ICON_FILE;

		item->m_KeyFile = item->m_FullPath;
	}

	return item;
}


//////////////////////////////////////////////////////////////////////////
DWORD CViewTree::GetFileType(CString Filename)
{
	DWORD Type = 0;
	CString Ext = GetExt(Filename);

	int i;

	for(i=0; i<NumEntries(EXT_ACTOR); i++)
		if(Ext.CompareNoCase(Entry(i+1, EXT_ACTOR))==0){
			Type = DC_OBJECT_ACTOR;
			return Type;
		}

	for(i=0; i<NumEntries(EXT_ENTITY); i++)
		if(Ext.CompareNoCase(Entry(i+1, EXT_ENTITY))==0){
			Type = DC_OBJECT_ENTITY;
			return Type;
		}

	for(i=0; i<NumEntries(EXT_SCENE); i++)
		if(Ext.CompareNoCase(Entry(i+1, EXT_SCENE))==0){
			Type = DC_OBJECT_SCENE;
			return Type;
		}

	for(i=0; i<NumEntries(EXT_SCRIPT); i++)
		if(Ext.CompareNoCase(Entry(i+1, EXT_SCRIPT))==0){
			Type = DC_OBJECT_SCRIPT;
			return Type;
		}

	for(i=0; i<NumEntries(EXT_WINDOW); i++)
		if(Ext.CompareNoCase(Entry(i+1, EXT_WINDOW))==0){
			Type = DC_OBJECT_WINDOW;
			return Type;
		}

	for(i=0; i<NumEntries(EXT_DEF); i++)
		if(Ext.CompareNoCase(Entry(i+1, EXT_DEF))==0){
			Type = DC_OBJECT_DEF;
			return Type;
		}

	for(i=0; i<NumEntries(EXT_SOUND); i++)
		if(Ext.CompareNoCase(Entry(i+1, EXT_SOUND))==0){
			Type = DC_OBJECT_SOUND;
			return Type;
		}

	for(i=0; i<NumEntries(EXT_FONT); i++)
		if(Ext.CompareNoCase(Entry(i+1, EXT_FONT))==0){
			Type = DC_OBJECT_FONT;
			return Type;
		}

	for(i=0; i<NumEntries(EXT_IMAGE); i++)
		if(Ext.CompareNoCase(Entry(i+1, EXT_IMAGE))==0){
			Type = DC_OBJECT_IMAGE;
			return Type;
		}

	for(i=0; i<NumEntries(EXT_SPRITE); i++)
		if(Ext.CompareNoCase(Entry(i+1, EXT_SPRITE))==0){
			Type = DC_OBJECT_SPRITE;
			return Type;
		}

	for(i=0; i<NumEntries(EXT_PACKAGE); i++)
		if(Ext.CompareNoCase(Entry(i+1, EXT_PACKAGE))==0){
			Type = DC_OBJECT_PACKAGE;
			return Type;
		}

	for(i=0; i<NumEntries(EXT_TALK); i++)
		if(Ext.CompareNoCase(Entry(i+1, EXT_TALK))==0){
			Type = DC_OBJECT_TALK;
			return Type;
		}

	for(i=0; i<NumEntries(EXT_ACTOR3D); i++)
		if(Ext.CompareNoCase(Entry(i+1, EXT_ACTOR3D))==0){
			Type = DC_OBJECT_ACTOR3D;
			return Type;
		}

	return Type;
}


//////////////////////////////////////////////////////////////////////////
DWORD CViewTree::GetFolderType(CString Filename, CString& Keyfile)
{
	CFileFind finder;
	DWORD Type = 0;
	DWORD KeyType = 0;
	Keyfile = "";


	BOOL Working = finder.FindFile(Filename + "\\*.*");
	while (Working){
		Working = finder.FindNextFile();

		if(finder.IsDirectory()) continue;

		DWORD FileType = GetFileType(finder.GetFileName());
		if(FileType==DC_OBJECT_SCENE){
			Keyfile = finder.GetFilePath();
			KeyType = FileType;
		}
		else if(FileType==DC_OBJECT_ACTOR && KeyType!=DC_OBJECT_SCENE){
			Keyfile = finder.GetFilePath();
			KeyType = FileType;
		}
		else if(FileType==DC_OBJECT_ENTITY && KeyType==0){
			Keyfile = finder.GetFilePath();
			KeyType = FileType;
		}

		Type |= FileType;
	}

	return Type;
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(!m_Redrawing){
		NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

		CTreeItem* item=NULL;
		HTREEITEM si = m_Tree.GetSelectedItem();
		if(si) item = (CTreeItem*)m_Tree.GetItemData(si);

		if(item && m_View){
			m_View->PreviewFile(item->m_KeyFile);
			if(item->m_IsPackage) m_View->OnPackageChange(item->m_ShortName);
			else m_View->OnPackageChange("");
		}
	}

		
	*pResult = 0;
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	CProjectDoc* Doc = m_View->GetDocument();

	CTreeItem* item=NULL;
	HTREEITEM si = m_Tree.GetSelectedItem();
	if(si) item = (CTreeItem*)m_Tree.GetItemData(si);

	if(Doc && item && !item->m_IsDir){
		m_ScheduledEdit = true;
		//EditFile(item->m_KeyFile, Doc->Game, NULL);
	}
	
	*pResult = 0;
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnRclickTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CPoint pt;
	GetCursorPos(&pt);
	m_Tree.ScreenToClient(&pt);

	bool PopupHandled = false;
	HTREEITEM item = m_Tree.HitTest(pt);
	GetCursorPos(&pt);
	if(item){
		m_Tree.SetFocus();
		m_Tree.SelectItem(item);		
		CTreeItem* TreeItem = (CTreeItem*)m_Tree.GetItemData(item);
		if(TreeItem){
			PopupHandled = true;
			DisplayPopupMenu(pt, TreeItem);
		}
	}

	if(!PopupHandled) DisplaySimplePopupMenu(pt);
	
	*pResult = 0;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CViewTree::DisplayPopupMenu(CPoint Point, CTreeItem* Item)
{
	CProjectDoc* Doc = m_View->GetDocument();
	if(!Doc) return E_FAIL;

	int origIconY = m_PopupMenu.m_iconY;
	m_PopupMenu.m_iconY = 16;
	m_PopupMenu.CreatePopupMenu();

	CImageList* pImageList;
	pImageList = m_Tree.GetImageList(TVSIL_NORMAL);


	if(Item->m_KeyFile!=""){
		m_PopupMenu.AppendMenu(MF_STRING, ID_POPUP_TREE_OPEN, LOC("/str1180/Open..."));
		m_PopupMenu.AppendMenu(MF_STRING, ID_POPUP_TREE_EDIT, LOC("/str0060/Edit..."));
		m_PopupMenu.AppendMenu(MF_SEPARATOR);
	}

	if(Item->m_IsDir){
		m_PopupMenu.AppendMenu(MF_STRING, ID_POPUP_TREE_EXPLORE, LOC("/str0061/Explore"), pImageList, ICON_EXPLORE);
		if(Item->m_IsTopLevel){
			if(!Item->m_IsPackage) m_PopupMenu.AppendMenu(MF_STRING, ID_POPUP_TREE_PROMOTE, LOC("/str0062/Promote to package"), pImageList, ICON_PACKAGE);
			else{
				if(!Doc->IsPrimaryPackage(Item->m_ShortName)){
					m_PopupMenu.AppendMenu(MF_STRING, ID_POPUP_TREE_PRIMARY_PACKAGE, LOC("/str0063/Set as primary package"), pImageList, ICON_P_PACKAGE);
					m_PopupMenu.AppendMenu(MF_STRING, ID_POPUP_TREE_DEGRADE, LOC("/str0064/Degrade to folder"), pImageList, ICON_DEGRADE);
				}
				m_PopupMenu.AppendMenu(MF_STRING, ID_POPUP_TREE_COMPILE, LOC("/str1182/Compile this package"), pImageList, ICON_COMP_PACK);
			}
		}
		m_PopupMenu.AppendMenu(MF_SEPARATOR);

		// quick add
		CFileFind finder;
		DWORD Contains = 0;
		BOOL Working = finder.FindFile(Item->m_FullPath + "\\*.*");
		while (Working){
			Working = finder.FindNextFile();
			if(!finder.IsDots() && finder.IsDirectory()){
				CString dummy;
				Contains |= GetFolderType(finder.GetFilePath(), dummy);
			}
		}

		bool Separator = false;

		if(Contains & DC_OBJECT_SCENE){
			m_PopupMenu.AppendMenu(MF_STRING, ID_POPUP_ADD_SCENE,  LOC("/str0065/Add scene..."),  pImageList, ICON_SCENE);
			Separator = true;
		}
		if(Contains & DC_OBJECT_ACTOR){
			m_PopupMenu.AppendMenu(MF_STRING, ID_POPUP_ADD_ACTOR,  LOC("/str0066/Add actor..."),  pImageList, ICON_ACTOR);
			Separator = true;
		}
		if(Contains & DC_OBJECT_ACTOR3D){
			m_PopupMenu.AppendMenu(MF_STRING, ID_POPUP_ADD_ACTOR3D,  LOC("/str1113/Add 3D actor..."),  pImageList, ICON_ACTOR3D);
			Separator = true;
		}
		if(Contains & DC_OBJECT_ENTITY){
			m_PopupMenu.AppendMenu(MF_STRING, ID_POPUP_ADD_ENTITY, LOC("/str0067/Add entity..."), pImageList, ICON_ENTITY);
			Separator = true;
		}


		if(Item->m_Contains & DC_OBJECT_SCRIPT){
			m_PopupMenu.AppendMenu(MF_STRING, ID_POPUP_ADD_SCRIPT,  LOC("/str0068/Add script..."),  pImageList, ICON_SCRIPT);
			Separator = true;
		}
		if(Item->m_Contains & DC_OBJECT_SPRITE){
			m_PopupMenu.AppendMenu(MF_STRING, ID_POPUP_ADD_SPRITE,  LOC("/str0069/Add sprite..."),  pImageList, ICON_SPRITE);
			Separator = true;
		}
		if(Item->m_Contains & DC_OBJECT_FONT){
			m_PopupMenu.AppendMenu(MF_STRING, ID_POPUP_ADD_FONT,    LOC("/str0070/Add font..."),  pImageList, ICON_FONT);
			Separator = true;
		}
		if(Item->m_Contains & DC_OBJECT_WINDOW){
			m_PopupMenu.AppendMenu(MF_STRING, ID_POPUP_ADD_WINDOW,  LOC("/str0071/Add window..."),  pImageList, ICON_WINDOW);
			Separator = true;
		}

		if(Separator) m_PopupMenu.AppendMenu(MF_SEPARATOR);
	}

	m_PopupMenu.AppendMenu(MF_STRING, ID_POPUP_TREE_CREATE_FOLDER, LOC("/str0072/Create subfolder..."), pImageList, ICON_FOLDER);
	BCMenu *PopupAdd = m_PopupMenu.AppendODPopupMenu(LOC("/str0073/Add"));
	PopupAdd->AppendMenu(MF_STRING, ID_POPUP_ADD_SCENE,  LOC("/str0065/Add scene..."),  pImageList, ICON_SCENE);
	PopupAdd->AppendMenu(MF_STRING, ID_POPUP_ADD_ACTOR,  LOC("/str0066/Add actor..."),  pImageList, ICON_ACTOR);
	PopupAdd->AppendMenu(MF_STRING, ID_POPUP_ADD_ACTOR3D,LOC("/str1113/Add 3D actor..."), pImageList, ICON_ACTOR3D);
	PopupAdd->AppendMenu(MF_STRING, ID_POPUP_ADD_ENTITY, LOC("/str0067/Add entity..."), pImageList, ICON_ENTITY);
	PopupAdd->AppendMenu(MF_SEPARATOR);
	PopupAdd->AppendMenu(MF_STRING, ID_POPUP_ADD_SCRIPT, LOC("/str0068/Add script..."), pImageList, ICON_SCRIPT);
	PopupAdd->AppendMenu(MF_SEPARATOR);
	PopupAdd->AppendMenu(MF_STRING, ID_POPUP_ADD_SPRITE, LOC("/str0069/Add sprite..."), pImageList, ICON_SPRITE);
	PopupAdd->AppendMenu(MF_STRING, ID_POPUP_ADD_WINDOW, LOC("/str0071/Add window..."), pImageList, ICON_WINDOW);
	PopupAdd->AppendMenu(MF_STRING, ID_POPUP_ADD_FONT,   LOC("/str0070/Add font..."),   pImageList, ICON_FONT);
	

	BCMenu *PopupImport = m_PopupMenu.AppendODPopupMenu(LOC("/str0074/Import"));
	PopupImport->AppendMenu(MF_STRING, ID_POPUP_IMPORT_FILE,   LOC("/str0075/Import file(s)..."), pImageList, ICON_FILE);
	PopupImport->AppendMenu(MF_STRING, ID_POPUP_IMPORT_FOLDER, LOC("/str0076/Import folder..."), pImageList,  ICON_FOLDER);

	if(Item->m_IsObject & DC_OBJECT_SCRIPT){
		m_PopupMenu.AppendMenu(MF_SEPARATOR);
		m_PopupMenu.AppendMenu(MF_STRING, ID_POPUP_TREE_CHECK_SYNTAX, LOC("/str0077/Check syntax..."), pImageList, ICON_CHECK);
	}

	if(Item->m_IsObject & DC_OBJECT_SCENE || Item->m_Contains & DC_OBJECT_SCENE){
		m_PopupMenu.AppendMenu(MF_SEPARATOR);
		m_PopupMenu.AppendMenu(MF_STRING, ID_POPUP_TREE_STARTUP_SCENE, LOC("/str1150/Set as startup scene"), pImageList, ICON_SCENE);
		m_PopupMenu.AppendMenu(MF_STRING, ID_POPUP_TREE_STARTUP_SCENE_D, LOC("/str1089/Set as startup scene (debug only)"), pImageList, ICON_SCENE);
	}


	if(!Item->m_IsPackage){
		m_PopupMenu.AppendMenu(MF_SEPARATOR);
		m_PopupMenu.AppendMenu(MF_STRING, ID_POPUP_TREE_REMOVE, LOC("/str0078/Remove"), pImageList, ICON_REMOVE);
	}		

	if(Item->m_KeyFile!=""){
		m_PopupMenu.AppendMenu(MF_SEPARATOR);
		m_PopupMenu.AppendMenu(MF_STRING, ID_POPUP_TREE_COPY, LOC("/str0079/Copy filename"), pImageList, ICON_COPY);
	}

	m_PopupMenu.AppendMenu(MF_SEPARATOR);
	m_PopupMenu.AppendMenu(MF_STRING, ID_POPUP_TREE_REFRESH, LOC("/str0080/Refresh"), pImageList, ICON_REFRESH);

	::SetMenuDefaultItem(m_PopupMenu.m_hMenu, ID_POPUP_TREE_OPEN, FALSE);

	m_PopupMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, Point.x, Point.y, AfxGetMainWnd());
	m_PopupMenu.DestroyMenu();
	m_PopupMenu.m_iconY = origIconY;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CViewTree::DisplaySimplePopupMenu(CPoint Point)
{
	int origIconY = m_PopupMenu.m_iconY;
	m_PopupMenu.m_iconY = 16;
	m_PopupMenu.CreatePopupMenu();

	CImageList* pImageList;
	pImageList = m_Tree.GetImageList(TVSIL_NORMAL);

	m_PopupMenu.AppendMenu(MF_STRING, ID_POPUP_TREE_FOLDER_TOP, LOC("/str0081/Create folder..."), pImageList, ICON_FOLDER);
	m_PopupMenu.AppendMenu(MF_STRING, ID_POPUP_TREE_REFRESH, LOC("/str0080/Refresh"), pImageList, ICON_REFRESH);

	m_PopupMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, Point.x, Point.y, AfxGetMainWnd());
	m_PopupMenu.DestroyMenu();
	m_PopupMenu.m_iconY = origIconY;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnPopupTreeOpen() 
{
	CProjectDoc* Doc = m_View->GetDocument();

	CTreeItem* item=NULL;
	HTREEITEM si = m_Tree.GetSelectedItem();
	if(si) item = (CTreeItem*)m_Tree.GetItemData(si);

	if(Doc && item){
		Doc->OnSaveDocument(Doc->GetPathName());
		EditFile(item->m_KeyFile, Doc->Game, NULL, true);
	}
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnPopupTreeEdit() 
{
	CProjectDoc* Doc = m_View->GetDocument();

	CTreeItem* item=NULL;
	HTREEITEM si = m_Tree.GetSelectedItem();
	if(si) item = (CTreeItem*)m_Tree.GetItemData(si);

	if(Doc && item){
		Doc->OnSaveDocument(Doc->GetPathName());
		EditFile(item->m_KeyFile, Doc->Game, NULL);
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT CViewTree::SelectFile(CString Filename)
{
	CProjectDoc* Doc = m_View->GetDocument();
	if(!Doc) return E_FAIL;

	// make relative path
	if(Filename.Left(Doc->m_ProjectRoot.GetLength())==Doc->m_ProjectRoot)
		Filename = Filename.Right(Filename.GetLength() - Doc->m_ProjectRoot.GetLength());

	int i;

	for(i=0; i<Filename.GetLength(); i++){
		if(Filename[i]=='/') Filename.SetAt(i, '\\');
	}

	HTREEITEM parent = NULL;
	for(i=1; i<=NumEntries(Filename, '\\'); i++){

 		HTREEITEM hIT = m_Tree.GetChildItem(parent);
		bool Found = false;
		while (hIT != NULL){
			if(m_Tree.GetItemText(hIT)==Entry(i, Filename, '\\')){
				parent = hIT;
				m_Tree.Expand(parent, TVE_EXPAND);
				Found = true;
				break;
			}
			hIT = m_Tree.GetNextSiblingItem(hIT);
		}
		if(!Found) break;
	}

	if(parent) m_Tree.SelectItem(parent);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnPopupTreeCreateFolder() 
{
	CProjectDoc* Doc = m_View->GetDocument();

	CTreeItem* item=NULL;
	HTREEITEM si = m_Tree.GetSelectedItem();
	if(si) item = (CTreeItem*)m_Tree.GetItemData(si);

	if(Doc && item){
		CreateFolder(item->m_FullPath, item->m_IsDir);
	}
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnPopupTreeFolderTop() 
{
	CProjectDoc* Doc = m_View->GetDocument();
	if(Doc) CreateFolder(Doc->m_ProjectRoot, true);
}



//////////////////////////////////////////////////////////////////////////
HRESULT CViewTree::CreateFolder(CString BasePath, bool IsDir)
{
	if(!IsDir){
		char drive[_MAX_DRIVE];
		char path[_MAX_PATH];

		_splitpath(BasePath, drive, path, NULL, NULL);
		BasePath.Format("%s%s", drive, path);
	}

	if(BasePath=="" || BasePath[BasePath.GetLength()-1]!='\\') BasePath += "\\";

	CFolderDlg dlg;
	dlg.m_FolderName = LOC("/str0102/New folder");
	dlg.m_BasePath = BasePath;

	if(dlg.DoModal()==IDOK){
		if(MakePath(BasePath + dlg.m_FolderName + "\\")==""){
			MessageBox(LOC("/str0049/Error creating folder."), NULL, MB_OK|MB_ICONERROR);
			return E_FAIL;
		}
		else{
			Refresh();
			SelectFile(BasePath + dlg.m_FolderName);
			return S_OK;
		}
	}
	else return S_OK;
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnPopupTreeRefresh() 
{
	Refresh();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CViewTree::Refresh()
{
	m_ShowHiddenFiles = GetRegInt(HKEY_CURRENT_USER, DCGF_TOOLS_REG_PATH, "ShowHiddenFiles", 0) == 1;

	CProjectDoc* Doc = m_View->GetDocument();

	if(Doc) Doc->ValidatePackages();

	CTreeItem* item=NULL;
	HTREEITEM si = m_Tree.GetSelectedItem();
	if(si) item = (CTreeItem*)m_Tree.GetItemData(si);

	m_Tree.SetRedraw(FALSE);
	if(Doc && item){
		CString Filename = item->m_FullPath;
		item = NULL;
		Redraw(false);
		SelectFile(Filename);
	}
	else {
		Redraw();
	}
	m_Tree.SetRedraw(TRUE);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnPopupTreeExplore() 
{
	CProjectDoc* Doc = m_View->GetDocument();

	CTreeItem* item=NULL;
	HTREEITEM si = m_Tree.GetSelectedItem();
	if(si) item = (CTreeItem*)m_Tree.GetItemData(si);
	if(Doc && item && item->m_IsDir) ShellExecute(HWND(this), "explore", item->m_FullPath, NULL, NULL, SW_SHOWNORMAL);
	else MessageBeep(MB_ICONERROR);
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnPopupTreeRemove() 
{
	if(m_View && m_View->m_ThreadWorking){
		MessageBox(LOC("/str0050/Cannot remove the object while loading is in progress."), NULL, MB_OK|MB_ICONWARNING);
		return;
	}
	
	CProjectDoc* Doc = m_View->GetDocument();

	CTreeItem* item=NULL;
	HTREEITEM si = m_Tree.GetSelectedItem();
	if(si) item = (CTreeItem*)m_Tree.GetItemData(si);

	m_View->PreviewFile("");
	
	if(Doc && item){
		CShellFileOp sfo;
		BOOL bAPICalled;
		int nAPIReturnVal;

		sfo.AddSourceFile(item->m_FullPath);
		sfo.SetOperationFlags ( FO_DELETE,
								AfxGetMainWnd(),	// pointer to parent window
								FALSE,				// flag - silent mode?
								!(GetAsyncKeyState(VK_SHIFT) & 0x8000),	// flag - allow undo?
								FALSE,				// flag - should wild cards affect files only?
								FALSE,				// flag - suppress confirmation messages?
								TRUE,				// flag - suppress confirmation messages when making directories?
								FALSE,				// flag - rename files when name collisions occur?
								FALSE);				// flag - simple progress dialog?

		sfo.Go(&bAPICalled, &nAPIReturnVal);
		//if(!sfo.AnyOperationsAborted()) Refresh();
		Refresh();
	}
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::GetExpandedItems(HTREEITEM Parent)
{
	if(Parent==NULL) m_ExpandedList.RemoveAll();

	HTREEITEM hIT = m_Tree.GetChildItem(Parent);

	while (hIT != NULL){
		CTreeItem* ItemData = (CTreeItem*)m_Tree.GetItemData(hIT);
		if(ItemData && ItemData->m_IsDir && (m_Tree.GetItemState(hIT, TVIS_EXPANDED)&TVIS_EXPANDED))
			m_ExpandedList.AddTail(ItemData->m_FullPath);

		GetExpandedItems(hIT);
		hIT = m_Tree.GetNextSiblingItem(hIT);
	}
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnPopupImportFolder() 
{
	CProjectDoc* Doc = m_View->GetDocument();

	CTreeItem* item=NULL;
	HTREEITEM si = m_Tree.GetSelectedItem();
	if(si) item = (CTreeItem*)m_Tree.GetItemData(si);
	
	if(Doc && item){
		CString BasePath;

		if(item->m_IsDir) BasePath = item->m_FullPath;
		else{
			char drive[_MAX_DRIVE];
			char path[_MAX_PATH];

			_splitpath(item->m_FullPath, drive, path, NULL, NULL);
			BasePath.Format("%s%s", drive, path);
		}

		CBrowseForFolder bf;	
		bf.hWndOwner = this->GetSafeHwnd();
		bf.strTitle = LOC("/str0104/Select a folder to import...");

		bf.strStartupDir = m_LastImportDir;

		if (bf.GetFolder(m_LastImportDir)){
			CShellFileOp sfo;
			BOOL bAPICalled;
			int nAPIReturnVal;

			if(m_LastImportDir[m_LastImportDir.GetLength()-1] != '\\') m_LastImportDir += '\\';
			if(BasePath[BasePath.GetLength()-1] != '\\') BasePath += '\\';

			BasePath += Entry(NumEntries(m_LastImportDir, '\\')-1, m_LastImportDir, '\\');
			
			sfo.AddSourceFile(m_LastImportDir + "*.*");
			sfo.AddDestFile(BasePath);
			sfo.SetOperationFlags ( FO_COPY,
								AfxGetMainWnd(),	// pointer to parent window
								FALSE,				// flag - silent mode?
								FALSE,				// flag - allow undo?
								FALSE,				// flag - should wild cards affect files only?
								FALSE,				// flag - suppress confirmation messages?
								TRUE,				// flag - suppress confirmation messages when making directories?
								FALSE,				// flag - rename files when name collisions occur?
								FALSE);				// flag - simple progress dialog?

			sfo.Go(&bAPICalled, &nAPIReturnVal);
			Refresh();
			SelectFile(BasePath);
		}
	}
	
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnPopupImportFile() 
{
	CProjectDoc* Doc = m_View->GetDocument();

	CTreeItem* item=NULL;
	HTREEITEM si = m_Tree.GetSelectedItem();
	if(si) item = (CTreeItem*)m_Tree.GetItemData(si);
	
	if(Doc && item){

		CString BasePath;

		if(item->m_IsDir) BasePath = item->m_FullPath;
		else{
			char drive[_MAX_DRIVE];
			char path[_MAX_PATH];

			_splitpath(item->m_FullPath, drive, path, NULL, NULL);
			BasePath.Format("%s%s", drive, path);
		}


		CString Filter = "All Files (*.*)|*.*||";
		CFileDialog dlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT|OFN_HIDEREADONLY|OFN_ENABLESIZING|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_NOCHANGEDIR, Filter, this);
		dlg.m_ofn.lpstrInitialDir = LPCSTR(m_LastImportDir);
	
		if(dlg.DoModal()==IDOK){

			m_LastImportDir = dlg.GetPathName();

			CShellFileOp sfo;
			BOOL bAPICalled;
			int nAPIReturnVal;

			POSITION pos = dlg.GetStartPosition();
			while(pos) sfo.AddSourceFile(dlg.GetNextPathName(pos));

			sfo.AddDestFile(BasePath);
			
			sfo.SetOperationFlags ( FO_COPY,
								AfxGetMainWnd(),	// pointer to parent window
								FALSE,				// flag - silent mode?
								FALSE,				// flag - allow undo?
								FALSE,				// flag - should wild cards affect files only?
								FALSE,				// flag - suppress confirmation messages?
								TRUE,				// flag - suppress confirmation messages when making directories?
								FALSE,				// flag - rename files when name collisions occur?
								FALSE);				// flag - simple progress dialog?

			sfo.Go(&bAPICalled, &nAPIReturnVal);
			Refresh();
		}
	}			
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnPopupTreePromote() 
{
	CProjectDoc* Doc = m_View->GetDocument();

	CTreeItem* item=NULL;
	HTREEITEM si = m_Tree.GetSelectedItem();
	if(si) item = (CTreeItem*)m_Tree.GetItemData(si);
	
	if(Doc && item){
		CPackage* pkg = new CPackage();
		pkg->m_Folder = item->m_ShortName;		
		Doc->m_Packages.Add(pkg);
		Doc->SetModifiedFlag();
		Doc->OnSaveDocument(Doc->GetPathName());
		Doc->Game->m_FileManager->ReloadPaths();
		m_View->Game->m_FileManager->ReloadPaths();

		Refresh();
	}
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnPopupTreeDegrade() 
{
	CProjectDoc* Doc = m_View->GetDocument();

	CTreeItem* item=NULL;
	HTREEITEM si = m_Tree.GetSelectedItem();
	if(si) item = (CTreeItem*)m_Tree.GetItemData(si);
	
	if(Doc && item){
		if(MessageBox(LOC("/str0051/Do you really want to degrade this package to a common folder?\n(you will lose all setting for this package)"), NULL, MB_ICONQUESTION|MB_YESNO)==IDYES){
			for(int i=0; i<Doc->m_Packages.GetSize(); i++){
				if(item->m_ShortName.CompareNoCase(Doc->m_Packages[i]->m_Folder)==0){
					delete Doc->m_Packages[i];
					Doc->m_Packages.RemoveAt(i);
				}
			}
			Doc->SetModifiedFlag();
			Doc->OnSaveDocument(Doc->GetPathName());
			Doc->Game->m_FileManager->ReloadPaths();
			m_View->Game->m_FileManager->ReloadPaths();

			Refresh();
		}
	}	
}

//////////////////////////////////////////////////////////////////////////
void CViewTree::OnPopupTreeCompile() 
{
	CProjectDoc* Doc = m_View->GetDocument();

	CTreeItem* item=NULL;
	HTREEITEM si = m_Tree.GetSelectedItem();
	if(si) item = (CTreeItem*)m_Tree.GetItemData(si);

	if(Doc && item){
		for(int i=0; i<Doc->m_Packages.GetSize(); i++)
		{
			if(item->m_ShortName.CompareNoCase(Doc->m_Packages[i]->m_Folder)==0)
			{
				Doc->CompilePackages(Doc->m_Packages[i]);
				break;
			}
		}
	}	
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnPopupTreePrimaryPackage() 
{
	CProjectDoc* Doc = m_View->GetDocument();

	CTreeItem* item=NULL;
	HTREEITEM si = m_Tree.GetSelectedItem();
	if(si) item = (CTreeItem*)m_Tree.GetItemData(si);
	
	if(Doc && item){
		for(int i=0; i<Doc->m_Packages.GetSize(); i++){
			if(item->m_ShortName.CompareNoCase(Doc->m_Packages[i]->m_Folder)==0)
				Doc->m_Packages[i]->m_Primary = true;
			else
				Doc->m_Packages[i]->m_Primary = false;
		}
		Doc->SetModifiedFlag();
		Refresh();
	}	
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnPopupAddActor() 
{
	CProjectDoc* Doc = m_View->GetDocument();
	CString Path = GetCurrentPath();

	if(Doc && Path!=""){
		CTemplateDlg dlg;
		dlg.m_Subdir = "actor";
		dlg.m_InitName = "new_actor";
		dlg.Game = m_View->Game;
		dlg.m_ProjectRoot = Doc->m_ProjectRoot;
		dlg.m_Target = Path;

		if(dlg.DoModal()==IDOK){			
			Refresh();
			SelectFile(dlg.m_SelectedObject);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void CViewTree::OnPopupAddActor3d()
{
	CProjectDoc* Doc = m_View->GetDocument();
	CString Path = GetCurrentPath();

	if(Doc && Path!=""){
		CTemplateDlg dlg;
		dlg.m_Subdir = "actor3d";
		dlg.m_InitName = "new_actor";
		dlg.Game = m_View->Game;
		dlg.m_ProjectRoot = Doc->m_ProjectRoot;
		dlg.m_Target = Path;

		if(dlg.DoModal()==IDOK){			
			Refresh();
			SelectFile(dlg.m_SelectedObject);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnPopupAddEntity() 
{
	CProjectDoc* Doc = m_View->GetDocument();
	CString Path = GetCurrentPath();

	if(Doc && Path!=""){
		CTemplateDlg dlg;
		dlg.m_Subdir = "entity";
		dlg.m_InitName = "new_entity";
		dlg.Game = m_View->Game;
		dlg.m_ProjectRoot = Doc->m_ProjectRoot;
		dlg.m_Target = Path;

		if(dlg.DoModal()==IDOK){			
			Refresh();
			SelectFile(dlg.m_SelectedObject);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnPopupAddFont() 
{
	CProjectDoc* Doc = m_View->GetDocument();
	CString Path = GetCurrentPath();

	if(Doc && Path!=""){
		CTemplateDlg dlg;
		dlg.m_Subdir = "font";
		dlg.m_InitName = "new_font";
		dlg.Game = m_View->Game;
		dlg.m_ProjectRoot = Doc->m_ProjectRoot;
		dlg.m_Target = Path;

		if(dlg.DoModal()==IDOK){			
			Refresh();
			SelectFile(dlg.m_SelectedObject);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnPopupAddScene() 
{
	CProjectDoc* Doc = m_View->GetDocument();
	CString Path = GetCurrentPath();

	if(Doc && Path!=""){
		CTemplateDlg dlg;
		dlg.m_Subdir = "scene";
		dlg.m_InitName = "new_scene";
		dlg.Game = m_View->Game;
		dlg.m_ProjectRoot = Doc->m_ProjectRoot;
		dlg.m_Target = Path;

		if(dlg.DoModal()==IDOK){			
			Refresh();
			SelectFile(dlg.m_SelectedObject);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnPopupAddScript() 
{
	CProjectDoc* Doc = m_View->GetDocument();
	CString Path = GetCurrentPath();

	if(Doc && Path!=""){
		CTemplateDlg dlg;
		dlg.m_Subdir = "script";
		dlg.m_InitName = "new_script";
		dlg.Game = m_View->Game;
		dlg.m_ProjectRoot = Doc->m_ProjectRoot;
		dlg.m_Target = Path;

		if(dlg.DoModal()==IDOK){			
			Refresh();
			SelectFile(dlg.m_SelectedObject);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnPopupAddSprite() 
{
	CProjectDoc* Doc = m_View->GetDocument();
	CString Path = GetCurrentPath();

	if(Doc && Path!=""){
		CTemplateDlg dlg;
		dlg.m_Subdir = "sprite";
		dlg.m_InitName = "new_sprite";
		dlg.Game = m_View->Game;
		dlg.m_ProjectRoot = Doc->m_ProjectRoot;
		dlg.m_Target = Path;

		if(dlg.DoModal()==IDOK){			
			Refresh();
			SelectFile(dlg.m_SelectedObject);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnPopupAddWindow() 
{
	CProjectDoc* Doc = m_View->GetDocument();
	CString Path = GetCurrentPath();

	if(Doc && Path!=""){
		CTemplateDlg dlg;
		dlg.m_Subdir = "window";
		dlg.m_InitName = "new_window";
		dlg.Game = m_View->Game;
		dlg.m_ProjectRoot = Doc->m_ProjectRoot;
		dlg.m_Target = Path;

		if(dlg.DoModal()==IDOK){			
			Refresh();
			SelectFile(dlg.m_SelectedObject);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
CString CViewTree::GetCurrentPath()
{
	CTreeItem* item=NULL;
	HTREEITEM si = m_Tree.GetSelectedItem();
	if(si) item = (CTreeItem*)m_Tree.GetItemData(si);
	if(!item) return "";
			
	CString Path;
	if(!item->m_IsDir){
		char drive[_MAX_DRIVE];
		char path[_MAX_PATH];

		_splitpath(item->m_FullPath, drive, path, NULL, NULL);
		
		Path.Format("%s%s", drive, path);
	}
	else Path = item->m_FullPath;

	if(Path[Path.GetLength()-1]!='\\') Path += "\\";

	return Path;
}


//////////////////////////////////////////////////////////////////////////
CString CViewTree::GetCurrentFile()
{
	CTreeItem* item=NULL;
	HTREEITEM si = m_Tree.GetSelectedItem();
	if(si) item = (CTreeItem*)m_Tree.GetItemData(si);
	if(!item) return "";

	return item->m_KeyFile;
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnPreview() 
{
	CProjectDoc* Doc = m_View->GetDocument();
	if(!Doc) return;

	Doc->m_EditorPreview = (m_Preview.GetCheck()!=0);
	Doc->OnSaveDocument(Doc->GetPathName());
	if(Doc->m_EditorPreview) m_View->PreviewFile(GetCurrentFile());
	else m_View->PreviewFile("");
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnPopupTreeCopy() 
{
	if(!m_View || !m_View->Game){
		MessageBeep(MB_ICONERROR);
		return;
	}

	CString Filename = GetRelativePath(m_View->Game, GetCurrentFile());
	
	if(OpenClipboard ()){
		EmptyClipboard();

		HGLOBAL hClipbuffer = ::GlobalAlloc (GMEM_DDESHARE, Filename.GetLength() + 1);
		LPTSTR lpszBuffer = (LPTSTR)GlobalLock(hClipbuffer);

		_tcscpy (lpszBuffer, (LPCTSTR)Filename);

		::GlobalUnlock (hClipbuffer);
		::SetClipboardData (CF_TEXT, hClipbuffer);

		CloseClipboard();
	}	
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnPopupTreeCheckSyntax() 
{
	if(!m_View || !m_View->Game){
		MessageBeep(MB_ICONERROR);
		return;
	}

	m_View->GetDocument()->OnSaveDocument(m_View->GetDocument()->GetPathName());

	DWORD Size;
	m_ScriptError = "";
	m_View->Game->m_ScEngine->SetCompileErrorCallback(ErrorCallback, this);
	m_View->Game->m_DEBUG_AbsolutePathWarning = false;
	m_View->Game->m_ScEngine->GetCompiledScript((char*)LPCSTR(GetCurrentFile()), &Size, true);
	m_View->Game->m_DEBUG_AbsolutePathWarning = true;
	m_View->Game->m_ScEngine->SetCompileErrorCallback(NULL, NULL);

	if(m_ScriptError=="") MessageBox(LOC("/str0052/Syntax is correct."), NULL, MB_OK|MB_ICONINFORMATION);
	else MessageBox(m_ScriptError, NULL, MB_OK|MB_ICONERROR);
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::ErrorCallback(int Line, char *Text, void *Data)
{
	CViewTree* _this = (CViewTree*)Data;
	if(_this){		
		CString Error;
		Error.Format(CString(LOC("/str0101/Line")) + " %d: %s", Line, Text);
		_this->m_ScriptError = Error + "\n";
	}
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnPopupTreeStartupScene() 
{
	if(!m_View || !m_View->Game){
		MessageBeep(MB_ICONERROR);
		return;
	}

	CString Filename = GetRelativePath(m_View->Game, GetCurrentFile());

	CBUtils::SetString(&m_View->GetDocument()->m_Settings->m_EdGame->m_StartupScene, (char*)LPCSTR(Filename));
	m_View->m_ViewProps->Redraw();
	m_View->GetDocument()->SetModifiedFlag();
}


//////////////////////////////////////////////////////////////////////////
void CViewTree::OnPopupTreeStartupSceneDebug() 
{
	if(!m_View || !m_View->Game){
		MessageBeep(MB_ICONERROR);
		return;
	}

	CString Filename = GetRelativePath(m_View->Game, GetCurrentFile());

	CBUtils::SetString(&m_View->GetDocument()->m_Settings->m_EdGame->m_DebugStartupScene, (char*)LPCSTR(Filename));
	m_View->m_ViewProps->Redraw();
	m_View->GetDocument()->SetModifiedFlag();
}
