// ScenePage.cpp : implementation file
//

#include "stdafx.h"
#include "SceneEdit.h"
#include "ScenePage.h"
#include "MainFrm.h"
#include "LayerDlg.h"
#include "../mfc_shared/utils_mfc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScenePage dialog


CScenePage::CScenePage(CWnd* pParent /*=NULL*/)
	: CDialog(CScenePage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScenePage)
	//}}AFX_DATA_INIT

	m_View = NULL;
}


void CScenePage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScenePage)
	DDX_Control(pDX, IDC_ADD_ROT, m_AddRot);
	DDX_Control(pDX, IDC_DIVIDE, m_Divide);
	DDX_Control(pDX, IDC_ADD_SCALE, m_AddScale);
	DDX_Control(pDX, IDC_REMOVE_POINT, m_RemovePoint);
	DDX_Control(pDX, IDC_REMOVE_NODE, m_RemoveNode);
	DDX_Control(pDX, IDC_NODE_UP, m_NodeUp);
	DDX_Control(pDX, IDC_NODE_DOWN, m_NodeDown);
	DDX_Control(pDX, IDC_ADD_REGION, m_AddRegion);
	DDX_Control(pDX, IDC_ADD_POINT, m_AddPoint);
	DDX_Control(pDX, IDC_ADD_ENTITY, m_AddEntity);
	DDX_Control(pDX, IDC_REMOVE_LAYER, m_RemoveLayer);
	DDX_Control(pDX, IDC_LAYER_UP, m_LayerUp);
	DDX_Control(pDX, IDC_LAYER_PROPERTIES, m_LayerProperties);
	DDX_Control(pDX, IDC_LAYER_DOWN, m_LayerDown);
	DDX_Control(pDX, IDC_ADD_LAYER, m_AddLayer);
	DDX_Control(pDX, IDC_NODES, m_NodeList);
	DDX_Control(pDX, IDC_LAYERS, m_LayerList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScenePage, CDialog)
	//{{AFX_MSG_MAP(CScenePage)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LAYERS, OnChangeLayer)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_NODES, OnChangeNode)
	ON_BN_CLICKED(IDC_ADD_LAYER, OnAddLayer)
	ON_BN_CLICKED(IDC_ADD_ENTITY, OnAddEntity)
	ON_BN_CLICKED(IDC_ADD_POINT, OnAddPoint)
	ON_BN_CLICKED(IDC_ADD_REGION, OnAddRegion)
	ON_BN_CLICKED(IDC_LAYER_DOWN, OnLayerDown)
	ON_BN_CLICKED(IDC_LAYER_PROPERTIES, OnLayerProperties)
	ON_BN_CLICKED(IDC_LAYER_UP, OnLayerUp)
	ON_BN_CLICKED(IDC_NODE_DOWN, OnNodeDown)
	ON_BN_CLICKED(IDC_NODE_UP, OnNodeUp)
	ON_BN_CLICKED(IDC_REMOVE_LAYER, OnRemoveLayer)
	ON_BN_CLICKED(IDC_REMOVE_NODE, OnRemoveNode)
	ON_BN_CLICKED(IDC_REMOVE_POINT, OnRemovePoint)
	ON_BN_CLICKED(IDC_ADD_SCALE, OnAddScale)
	ON_BN_DOUBLECLICKED(IDC_NODE_UP, OnNodeUp)
	ON_BN_DOUBLECLICKED(IDC_NODE_DOWN, OnNodeDown)
	ON_BN_DOUBLECLICKED(IDC_LAYER_UP, OnLayerUp)
	ON_BN_DOUBLECLICKED(IDC_LAYER_DOWN, OnLayerDown)
	ON_BN_CLICKED(IDC_ADD_ROT, OnAddRot)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScenePage message handlers


//////////////////////////////////////////////////////////////////////////
void CScenePage::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	//CDialog::OnSize(nType, cx, cy);

	CRect rectClient;
	GetClientRect(&rectClient);

	CRect rectList;

	if(m_LayerList.GetSafeHwnd()){
		m_LayerList.GetWindowRect(&rectList);
		ScreenToClient(&rectList);
		rectList.right = rectList.left + (rectClient.right - rectClient.left);
		m_LayerList.MoveWindow(&rectList);
	}

	if(m_NodeList.GetSafeHwnd()){
		m_NodeList.GetWindowRect(&rectList);
		ScreenToClient(&rectList);
		rectList.right = rectList.left + (rectClient.right - rectClient.left);
		rectList.bottom = /*rectList.top +*/ (rectClient.bottom - rectClient.top);
		m_NodeList.MoveWindow(&rectList);
	}

	if(m_Divide.GetSafeHwnd()){
		m_Divide.GetWindowRect(&rectList);
		ScreenToClient(&rectList);
		rectList.right = rectList.left + (rectClient.right - rectClient.left);
		m_Divide.MoveWindow(&rectList);
	}

}


//////////////////////////////////////////////////////////////////////////
BOOL CScenePage::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_ImageList.Create(IDB_NODES, 16, 1, RGB(255, 0, 255));

	m_LayerList.SetImageList(&m_ImageList, LVSIL_SMALL);	
	m_LayerList.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_LayerList.InsertColumn(0,"Layer", LVCFMT_LEFT, 110);
	m_LayerList.InsertColumn(1,"Size", LVCFMT_RIGHT, 70);

	m_NodeList.SetImageList(&m_ImageList, LVSIL_SMALL);	
	m_NodeList.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_NodeList.InsertColumn(0,"Node", LVCFMT_LEFT, 110);
	m_NodeList.InsertColumn(1,"Type", LVCFMT_LEFT, 70);


	// layer buttons
	SetupButton(&m_LayerUp,         "Layer up",         IDB_MOVE_UP);
	SetupButton(&m_LayerDown,       "Layer down",       IDB_MOVE_DOWN);
	SetupButton(&m_AddLayer,        "Add layer",        IDB_BCGBARRES_NEW);
	SetupButton(&m_RemoveLayer,     "Remove layer",     IDB_BCGBARRES_DELETE);
	SetupButton(&m_LayerProperties, "Layer properties", IDB_PROPERTIES);

	SetupButton(&m_AddScale, "Add scale level", IDB_ADD_SCALE);
	SetupButton(&m_AddRot, "Add rotation level", IDB_ADD_ROT);

	// node buttons		
	SetupButton(&m_NodeUp,      "Node up",      IDB_MOVE_UP);
	SetupButton(&m_NodeDown,    "Node down",    IDB_MOVE_DOWN);
	SetupButton(&m_AddRegion,   "Add region",   IDB_ADD_REGION);
	SetupButton(&m_AddEntity,   "Add entity",   IDB_ADD_ENTITY);
	SetupButton(&m_RemoveNode,  "Remove node",  IDB_BCGBARRES_DELETE);
	SetupButton(&m_AddPoint,    "Add point",    IDB_ADD_POINT);
	SetupButton(&m_RemovePoint, "Remove point", IDB_REMOVE_POINT);

	m_EntityMenu.LoadMenu (IDR_NEW_ENTITY);
	m_AddEntity.m_hMenu = m_EntityMenu.GetSubMenu(0)->GetSafeHmenu();
	m_AddEntity.m_bStayPressed = true;


	SetState();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//////////////////////////////////////////////////////////////////////////
void CScenePage::SetCurrentView(CSceneView *View)
{
	if(View==m_View) return;
	
	m_View = View;
	Redraw();
}


//////////////////////////////////////////////////////////////////////////
void CScenePage::OnChangeLayer(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	if(!m_View){
		((CMainFrame*)AfxGetMainWnd())->m_wndProperty.ShowNone();		
		return;
	}


	CAdLayer* origLayer = m_View->m_SelectedLayer;
	bool origWaypoints = m_View->m_SelectedWaypoints;
	bool origFree = m_View->m_SelectedFree;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if(m_View && pNMListView->iItem!=-1){		
		if(m_LayerList.GetItemState(pNMListView->iItem, LVIS_SELECTED) & LVIS_SELECTED) m_View->m_SelectedLayer = (CAdLayer*)m_LayerList.GetItemData(pNMListView->iItem);


		BOOL bPrevState = (BOOL)(((pNMListView->uOldState & LVIS_STATEIMAGEMASK)>>12)-1);
		if (bPrevState < 0)	bPrevState = 0;

		BOOL bChecked=(BOOL)(((pNMListView->uNewState & LVIS_STATEIMAGEMASK)>>12)-1);   
		if (bChecked < 0) bChecked = 0; 

		CAdLayer* layer = (CAdLayer*)m_LayerList.GetItemData(pNMListView->iItem);
		if (layer && bPrevState != bChecked){
			if(layer->m_Active != (bChecked != FALSE)) m_View->SetUndoState("Set active layer");
			layer->m_Active = (bChecked != FALSE);
		}

		// waypoints and free's are always active
		if(!layer){
			if(bPrevState != bChecked && bChecked==FALSE) MessageBeep(MB_ICONASTERISK);
			m_LayerList.SetCheck(pNMListView->iItem);			
		}

		if(m_View->m_SelectedLayer==NULL){
			m_View->m_SelectedWaypoints = (pNMListView->iItem == m_View->Game->m_Scene->m_Layers.GetSize());
			m_View->m_SelectedFree = (pNMListView->iItem == m_View->Game->m_Scene->m_Layers.GetSize()+1);
		}
		else {
			m_View->m_SelectedWaypoints = false;
			m_View->m_SelectedFree = false;

		}

	}

	m_EntityMenu.EnableMenuItem(ID_NEW_ENTITY_REGION, m_View->m_SelectedFree?MF_GRAYED:MF_ENABLED);
	m_EntityMenu.EnableMenuItem(ID_NEW_ENTITY_SOUND, m_View->m_SelectedFree?MF_ENABLED:MF_GRAYED);
	

	if((m_View->m_SelectedLayer && m_View->m_SelectedLayer!=origLayer) || (m_View->m_SelectedWaypoints != origWaypoints) || (m_View->m_SelectedFree != origFree)){
		RedrawNodes();
	}
	
	*pResult = 0;
}


//////////////////////////////////////////////////////////////////////////
void CScenePage::Redraw()
{
	m_LayerList.DeleteAllItems();
	m_NodeList.DeleteAllItems();	

	int i;

	if(m_View){
		CAdScene* scene = m_View->Game->m_Scene;
		if(!scene) return;

		if(!m_View->m_SelectedWaypoints && !m_View->m_SelectedFree && !m_View->m_SelectedLayer && scene->m_MainLayer)
			m_View->m_SelectedLayer = scene->m_MainLayer;

		CString str;
		for(i=0; i<scene->m_Layers.GetSize(); i++){
			m_LayerList.InsertItem(i, scene->m_Layers[i]->m_Name, (scene->m_Layers[i]==scene->m_MainLayer)?0:1);
			m_LayerList.SetItemData(i, (DWORD)scene->m_Layers[i]);
			m_LayerList.SetCheck(i, scene->m_Layers[i]->m_Active);

			str.Format("%d x %d", scene->m_Layers[i]->m_Width, scene->m_Layers[i]->m_Height);
			m_LayerList.SetItemText(i, 1, str);

			if(scene->m_Layers[i]==m_View->m_SelectedLayer){
				m_LayerList.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
				m_LayerList.EnsureVisible(i, FALSE);
			}
		}

		m_LayerList.InsertItem(scene->m_Layers.GetSize(), "Waypoint groups", 12);
		m_LayerList.SetItemData(scene->m_Layers.GetSize(), (DWORD)NULL);
		m_LayerList.SetCheck(i, TRUE);
		if(m_View->m_SelectedWaypoints){
			m_LayerList.SetItemState(scene->m_Layers.GetSize(), LVIS_SELECTED, LVIS_SELECTED);
			m_LayerList.EnsureVisible(scene->m_Layers.GetSize(), FALSE);
			m_View->m_SelectedLayer = NULL;
			m_View->m_SelectedNode = NULL;
		}

		m_LayerList.InsertItem(scene->m_Layers.GetSize()+1, "Free entities", 3);
		m_LayerList.SetItemData(scene->m_Layers.GetSize()+1, (DWORD)NULL);
		m_LayerList.SetCheck(i+1, TRUE);
		if(m_View->m_SelectedFree){
			m_LayerList.SetItemState(scene->m_Layers.GetSize()+1, LVIS_SELECTED, LVIS_SELECTED);
			m_LayerList.EnsureVisible(scene->m_Layers.GetSize()+1, FALSE);
			m_View->m_SelectedLayer = NULL;
			m_View->m_SelectedNode = NULL;
			m_View->m_SelectedWaypoints = false;
			m_View->m_SelectedWptGroup = NULL;
		}


		RedrawNodes();
	}
	else{
		((CMainFrame*)AfxGetMainWnd())->m_wndProperty.ShowNone();
		SetState();
	}
}



//////////////////////////////////////////////////////////////////////////
void CScenePage::RedrawNodes()
{
	m_NodeList.SetRedraw(FALSE);

	m_NodeList.DeleteAllItems();

	if(!m_View){
		m_NodeList.SetRedraw(TRUE);
		SetState();
		return;
	}

	int i;

	// nodes
	if(m_View->m_SelectedLayer){		
		for(i=0; i<m_View->m_SelectedLayer->m_Nodes.GetSize(); i++){
			CAdSceneNode* node = m_View->m_SelectedLayer->m_Nodes[i];
			switch(node->m_Type){
			case OBJECT_ENTITY:
				if(node->m_Entity->m_Region){
					m_NodeList.InsertItem(i, node->m_Entity->m_Name, 4);
					m_NodeList.SetItemText(i, 1, "Entity (rgn)");

				}
				else{
					m_NodeList.InsertItem(i, node->m_Entity->m_Name, 3);
					m_NodeList.SetItemText(i, 1, "Entity");
				}

				m_NodeList.SetItemData(i, (DWORD)node);
				m_NodeList.SetCheck(i, node->m_Entity->m_Active);
								
				break;
				
			case OBJECT_REGION:
				if(node->m_Region->m_Blocked)
					m_NodeList.InsertItem(i, node->m_Region->m_Name, 11);
				else if(node->m_Region->m_Decoration)
					m_NodeList.InsertItem(i, node->m_Region->m_Name, 14);
				else
					m_NodeList.InsertItem(i, node->m_Region->m_Name, 10);

				m_NodeList.SetItemData(i, (DWORD)node);
				m_NodeList.SetCheck(i, node->m_Region->m_Active);
				
				if(node->m_Region->m_Blocked) m_NodeList.SetItemText(i, 1, "Blocked");
				else if(node->m_Region->m_Decoration) m_NodeList.SetItemText(i, 1, "Decoration");
				else m_NodeList.SetItemText(i, 1, "Region");
				
				break;
			}
			if(node==m_View->m_SelectedNode){
				m_NodeList.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
				m_NodeList.EnsureVisible(i, FALSE);
			}
		}
	}
	// waypoint groups
	else if(m_View->m_SelectedWaypoints){
		CAdScene* scene = m_View->Game->m_Scene;
		if(!scene) return;

		for(i=0; i<scene->m_WaypointGroups.GetSize(); i++){
			CAdWaypointGroup* group = scene->m_WaypointGroups[i];
			
			m_NodeList.InsertItem(i, group->m_Name, 12);
			m_NodeList.SetItemData(i, (DWORD)group);
			m_NodeList.SetCheck(i, group->m_Active);
				
			m_NodeList.SetItemText(i, 1, "Wpt group");
			
			if(group==m_View->m_SelectedWptGroup){
				m_NodeList.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
				m_NodeList.EnsureVisible(i, FALSE);
			}		
		}
	}
	// free entities
	else if(m_View->m_SelectedFree){
		CAdScene* scene = m_View->Game->m_Scene;
		if(!scene) return;

		for(i=0; i<scene->m_Objects.GetSize(); i++){
			if(scene->m_Objects[i]->m_Type!=OBJECT_ENTITY) continue;

			CAdEntity* ent = (CAdEntity*)scene->m_Objects[i];

			if(ent->m_Region){
				m_NodeList.InsertItem(i, ent->m_Name, 4);
			}
			else{
				if(ent->m_Subtype==ENTITY_SOUND)
					m_NodeList.InsertItem(i, ent->m_Name, 13);
				else
					m_NodeList.InsertItem(i, ent->m_Name, 3);
			}

			m_NodeList.SetItemData(i, (DWORD)ent);
			m_NodeList.SetCheck(i, ent->m_Active);
				
			if(ent->m_Subtype==ENTITY_SOUND)
				m_NodeList.SetItemText(i, 1, "Sound entity");
			else
				m_NodeList.SetItemText(i, 1, "Entity (free)");
			
			if(ent==m_View->m_SelectedFreeObject){
				m_NodeList.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
				m_NodeList.EnsureVisible(i, FALSE);
			}		
		}
	}

	m_NodeList.SetRedraw(TRUE);
	if(!m_View || (!m_View->m_SelectedNode && !m_View->m_SelectedWptGroup && !m_View->m_SelectedFreeObject)) ((CMainFrame*)AfxGetMainWnd())->m_wndProperty.ShowNone();
	SetState();
}


//////////////////////////////////////////////////////////////////////////
void CScenePage::OnChangeNode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	if(!m_View)	return;


	m_View->RestoreState();

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if(m_View && pNMListView->iItem!=-1){		
		if(m_NodeList.GetItemState(pNMListView->iItem, LVIS_SELECTED) & LVIS_SELECTED){
			if(m_View->m_SelectedWaypoints){
				m_View->m_SelectedWptGroup = (CAdWaypointGroup*)m_NodeList.GetItemData(pNMListView->iItem);
				m_View->m_SelectedNode = NULL;
				m_View->m_SelectedFreeObject = NULL;
			}
			else if(m_View->m_SelectedFree){
				m_View->m_SelectedFreeObject = (CAdObject*)m_NodeList.GetItemData(pNMListView->iItem);
				m_View->m_SelectedNode = NULL;
				m_View->m_SelectedWptGroup = NULL;
			}
			else{
				m_View->m_SelectedNode = (CAdSceneNode*)m_NodeList.GetItemData(pNMListView->iItem);
				m_View->m_SelectedWptGroup = NULL;
				m_View->m_SelectedFreeObject = NULL;
			}
		}

		BOOL bPrevState = (BOOL)(((pNMListView->uOldState & LVIS_STATEIMAGEMASK)>>12)-1);
		if (bPrevState < 0)	bPrevState = 0;

		BOOL bChecked=(BOOL)(((pNMListView->uNewState & LVIS_STATEIMAGEMASK)>>12)-1);   
		if (bChecked < 0) bChecked = 0; 

		if (bPrevState != bChecked){			
			if(m_View->m_SelectedWaypoints){
				CAdWaypointGroup* group = (CAdWaypointGroup*)m_NodeList.GetItemData(pNMListView->iItem);
				if(group->m_Active != (bChecked!=0)) m_View->SetUndoState("Set active node");
				group->m_Active = (bChecked!=0);
			}
			else if(m_View->m_SelectedFree){
				CAdObject* obj = (CAdObject*)m_NodeList.GetItemData(pNMListView->iItem);
				if(obj->m_Active != (bChecked!=0)) m_View->SetUndoState("Set active node");
				obj->m_Active = (bChecked!=0);
			}
			else{
				CAdSceneNode* node = (CAdSceneNode*)m_NodeList.GetItemData(pNMListView->iItem);
				switch(node->m_Type){
					case OBJECT_ENTITY:
						if(node->m_Entity->m_Active != (bChecked!=0)) m_View->SetUndoState("Set active node");
						node->m_Entity->m_Active = (bChecked!=0);
						break;

					case OBJECT_REGION:
						if(node->m_Region->m_Active != (bChecked!=0)) m_View->SetUndoState("Set active node");
						node->m_Region->m_Active = (bChecked!=0);
						break;
				}
			}
		}
	}

	// show properties
	if(m_View->m_SelectedNode){
		switch(m_View->m_SelectedNode->m_Type){
			case OBJECT_ENTITY:
				((CMainFrame*)AfxGetMainWnd())->m_wndProperty.ShowEntity(m_View->m_SelectedNode->m_Entity, m_View);
			break;
			case OBJECT_REGION:
				((CMainFrame*)AfxGetMainWnd())->m_wndProperty.ShowRegion(m_View->m_SelectedNode->m_Region, m_View);
			break;
		}
	}
	else if(m_View->m_SelectedWptGroup) ((CMainFrame*)AfxGetMainWnd())->m_wndProperty.ShowWaypoints(m_View->m_SelectedWptGroup, m_View);
	else if(m_View->m_SelectedFreeObject && m_View->m_SelectedFreeObject->m_Type==OBJECT_ENTITY) ((CMainFrame*)AfxGetMainWnd())->m_wndProperty.ShowEntity((CAdEntity*)m_View->m_SelectedFreeObject, m_View);
	else ((CMainFrame*)AfxGetMainWnd())->m_wndProperty.ShowNone();

	SetState();
	
	*pResult = 0;
}


//////////////////////////////////////////////////////////////////////////
void CScenePage::OnCancel() 
{
	// do nothing
}


//////////////////////////////////////////////////////////////////////////
void CScenePage::OnOK() 
{
	// do nothing
}


//////////////////////////////////////////////////////////////////////////
void CScenePage::SetState()
{
	CAdScene* Scene = NULL;
	if(m_View) Scene = m_View->Game->m_Scene;

	m_LayerUp.EnableWindow(Scene && m_View->m_SelectedLayer && Scene->m_Layers.GetSize() > 0 && Scene->m_Layers[0] != m_View->m_SelectedLayer);
	m_LayerDown.EnableWindow(Scene && m_View->m_SelectedLayer && Scene->m_Layers.GetSize() > 0 && Scene->m_Layers[Scene->m_Layers.GetSize()-1] != m_View->m_SelectedLayer);

	m_AddLayer.EnableWindow(Scene!=NULL);
	m_RemoveLayer.EnableWindow(Scene && m_View->m_SelectedLayer && !m_View->m_SelectedLayer->m_Main);
	m_LayerProperties.EnableWindow(Scene && m_View->m_SelectedLayer);

	m_AddScale.EnableWindow(Scene!=NULL);
	m_AddRot.EnableWindow(Scene!=NULL);

	CAdSceneNode* Node = NULL;
	if(Scene && m_View->m_SelectedLayer){
		for(int i=0; i<m_View->m_SelectedLayer->m_Nodes.GetSize(); i++){
			if(m_View->m_SelectedLayer->m_Nodes[i]==m_View->m_SelectedNode){
				Node = m_View->m_SelectedNode;
				break;
			}
		}
	}


	m_NodeUp.EnableWindow(Scene && Node && m_View->m_SelectedLayer && m_View->m_SelectedNode && m_View->m_SelectedLayer->m_Nodes.GetSize() > 0 && m_View->m_SelectedLayer->m_Nodes[0] != m_View->m_SelectedNode);
	m_NodeDown.EnableWindow(Scene && Node && m_View->m_SelectedLayer && m_View->m_SelectedNode && m_View->m_SelectedLayer->m_Nodes.GetSize() > 0 && m_View->m_SelectedLayer->m_Nodes[m_View->m_SelectedLayer->m_Nodes.GetSize()-1] != m_View->m_SelectedNode);
	m_AddRegion.EnableWindow(Scene && ((m_View->m_SelectedLayer && m_View->m_SelectedLayer->m_Main) || m_View->m_SelectedWaypoints));
	m_AddEntity.EnableWindow(Scene && (m_View->m_SelectedLayer || m_View->m_SelectedFree));
	m_RemoveNode.EnableWindow(Scene && ((m_View->m_SelectedNode && m_View->m_SelectedLayer) || m_View->m_SelectedWptGroup || m_View->m_SelectedFreeObject));

	CBRegion* rgn=NULL;
	if(Scene && m_View->m_SelectedNode){
		if(m_View->m_SelectedNode->m_Region) rgn = m_View->m_SelectedNode->m_Region;
		else if(m_View->m_SelectedNode->m_Entity && m_View->m_SelectedNode->m_Entity->m_Region) rgn = m_View->m_SelectedNode->m_Entity->m_Region;
	}

	m_AddPoint.EnableWindow(Scene && (rgn || (m_View->m_SelectedWptGroup)));
	if(!m_AddPoint.IsWindowEnabled()) m_RemovePoint.EnableWindow(FALSE);
	else{
		if(Scene && rgn && rgn->m_Points.GetSize() <= 3)
			m_RemovePoint.EnableWindow(FALSE);
		else
			m_RemovePoint.EnableWindow(TRUE);
	}

	if(Scene && m_View->m_SelectedWaypoints) m_AddRegion.SetTooltip("Add waypoint group");
	else m_AddRegion.SetTooltip("Add region");
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CScenePage::OnLayerDown() 
{
	m_View->SetUndoState("Layer down");
	for(int i=0; i<m_View->Game->m_Scene->m_Layers.GetSize(); i++){
		if(m_View->Game->m_Scene->m_Layers[i]==m_View->m_SelectedLayer){
			m_View->Game->m_Scene->m_Layers[i] = m_View->Game->m_Scene->m_Layers[i+1];
			m_View->Game->m_Scene->m_Layers[i+1] = m_View->m_SelectedLayer;
			Redraw();
			break;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CScenePage::OnLayerUp() 
{
	m_View->SetUndoState("Layer up");
	for(int i=0; i<m_View->Game->m_Scene->m_Layers.GetSize(); i++){
		if(m_View->Game->m_Scene->m_Layers[i]==m_View->m_SelectedLayer){
			m_View->Game->m_Scene->m_Layers[i] = m_View->Game->m_Scene->m_Layers[i-1];
			m_View->Game->m_Scene->m_Layers[i-1] = m_View->m_SelectedLayer;
			Redraw();
			break;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CScenePage::OnAddLayer() 
{
	CLayerDlg dlg;
	dlg.m_MainLayer = false;
	dlg.m_Name = GetUniqueName("new layer");
	dlg.m_Width = 800;
	dlg.m_Height = 600;
	dlg.m_CloseUp = FALSE;
	
	if(dlg.DoModal()==IDOK){
		m_View->SetUndoState("Add layer");
		CAdLayer* Layer = ::new CAdLayer(m_View->Game);
		Layer->SetName((char*)LPCSTR(dlg.m_Name));
		Layer->m_Width = dlg.m_Width;
		Layer->m_Height = dlg.m_Height;
		Layer->m_Main = false;
		Layer->m_CloseUp = dlg.m_CloseUp!=FALSE;

		int index = -1;
		for(int i=0; i<m_View->Game->m_Scene->m_Layers.GetSize(); i++){
			if(m_View->Game->m_Scene->m_Layers[i]==m_View->m_SelectedLayer){
				index = i;
				break;
			}
		}

		if(index>=0) m_View->Game->m_Scene->m_Layers.InsertAt(index+1, Layer);
		else m_View->Game->m_Scene->m_Layers.Add(Layer);

		m_View->m_SelectedLayer = Layer;
		Redraw();
	}
}


//////////////////////////////////////////////////////////////////////////
void CScenePage::OnRemoveLayer() 
{
	if(MessageBox("Do you really want to remove this layer?", NULL, MB_ICONQUESTION|MB_YESNO)!=IDYES) return;
	
	m_View->SetUndoState("Remove layer");
	for(int i=0; i<m_View->Game->m_Scene->m_Layers.GetSize(); i++){
		if(m_View->Game->m_Scene->m_Layers[i]==m_View->m_SelectedLayer){
			for(int j=0; j<m_View->m_SelectedLayer->m_Nodes.GetSize(); j++){
				if(m_View->m_SelectedLayer->m_Nodes[j]==m_View->m_SelectedNode){
					m_View->m_SelectedNode = NULL;
					break;
				}
			}
			delete m_View->Game->m_Scene->m_Layers[i];
			m_View->Game->m_Scene->m_Layers.RemoveAt(i);
			if(i>=m_View->Game->m_Scene->m_Layers.GetSize()) i--;
			m_View->m_SelectedLayer = m_View->Game->m_Scene->m_Layers[i];
			
			Redraw();
			break;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CScenePage::OnLayerProperties() 
{
	CLayerDlg dlg;
	dlg.m_MainLayer = false;
	dlg.m_Name = CString(m_View->m_SelectedLayer->m_Name);
	dlg.m_Width = m_View->m_SelectedLayer->m_Width;
	dlg.m_Height = m_View->m_SelectedLayer->m_Height;
	dlg.m_MainLayer = m_View->m_SelectedLayer->m_Main;
	dlg.m_CloseUp = (BOOL)m_View->m_SelectedLayer->m_CloseUp;

	if(dlg.DoModal()==IDOK){
		m_View->SetUndoState("Change layer properties");
		m_View->m_SelectedLayer->SetName((char*)LPCSTR(dlg.m_Name));
		m_View->m_SelectedLayer->m_Width = dlg.m_Width;
		m_View->m_SelectedLayer->m_Height = dlg.m_Height;
		m_View->m_SelectedLayer->m_CloseUp = dlg.m_CloseUp!=FALSE;
		Redraw();
	}
	
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CScenePage::OnNodeDown() 
{
	m_View->SetUndoState("Node down");
	for(int i=0; i<m_View->m_SelectedLayer->m_Nodes.GetSize(); i++){
		if(m_View->m_SelectedLayer->m_Nodes[i]==m_View->m_SelectedNode){
			m_View->m_SelectedLayer->m_Nodes[i] = m_View->m_SelectedLayer->m_Nodes[i+1];
			m_View->m_SelectedLayer->m_Nodes[i+1] = m_View->m_SelectedNode;
			RedrawNodes();
			break;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CScenePage::OnNodeUp() 
{
	m_View->SetUndoState("Node up");
	for(int i=0; i<m_View->m_SelectedLayer->m_Nodes.GetSize(); i++){
		if(m_View->m_SelectedLayer->m_Nodes[i]==m_View->m_SelectedNode){
			m_View->m_SelectedLayer->m_Nodes[i] = m_View->m_SelectedLayer->m_Nodes[i-1];
			m_View->m_SelectedLayer->m_Nodes[i-1] = m_View->m_SelectedNode;
			RedrawNodes();
			break;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CScenePage::OnRemoveNode() 
{
	if(m_View->m_SelectedNode && m_View->m_SelectedLayer){
		if(MessageBox("Do you really want to remove this node?", NULL, MB_ICONQUESTION|MB_YESNO)!=IDYES) return;
		
		m_View->SetUndoState("Remove node");
		for(int i=0; i<m_View->m_SelectedLayer->m_Nodes.GetSize(); i++){
			if(m_View->m_SelectedLayer->m_Nodes[i]==m_View->m_SelectedNode){	
				delete m_View->m_SelectedLayer->m_Nodes[i];
				m_View->m_SelectedLayer->m_Nodes.RemoveAt(i);
				if(i>=m_View->m_SelectedLayer->m_Nodes.GetSize()) i--;
				if(i>=0) m_View->m_SelectedNode = m_View->m_SelectedLayer->m_Nodes[i];
				else m_View->m_SelectedNode = NULL;
				
				RedrawNodes();
				break;
			}
		}
	}
	else if(m_View->m_SelectedWptGroup){
		if(MessageBox("Do you really want to remove this waypoint group?", NULL, MB_ICONQUESTION|MB_YESNO)!=IDYES) return;
		
		m_View->SetUndoState("Remove waypoint group");
		for(int i=0; i<m_View->Game->m_Scene->m_WaypointGroups.GetSize(); i++){
			if(m_View->Game->m_Scene->m_WaypointGroups[i]==m_View->m_SelectedWptGroup){	
				delete m_View->Game->m_Scene->m_WaypointGroups[i];
				m_View->Game->m_Scene->m_WaypointGroups.RemoveAt(i);
				if(i>=m_View->Game->m_Scene->m_WaypointGroups.GetSize()) i--;
				if(i>=0) m_View->m_SelectedWptGroup = m_View->Game->m_Scene->m_WaypointGroups[i];
				else m_View->m_SelectedWptGroup = NULL;
				
				RedrawNodes();
				break;
			}
		}
	}
	else if(m_View->m_SelectedFreeObject){
		if(MessageBox("Do you really want to remove this entity?", NULL, MB_ICONQUESTION|MB_YESNO)!=IDYES) return;
		
		m_View->SetUndoState("Remove free entity");
		for(int i=0; i<m_View->Game->m_Scene->m_Objects.GetSize(); i++){
			if(m_View->Game->m_Scene->m_Objects[i]==m_View->m_SelectedFreeObject){
				m_View->Game->m_Scene->RemoveObject(m_View->m_SelectedFreeObject);
				//delete m_View->Game->m_Scene->m_Objects[i];
				//m_View->Game->m_Scene->m_Objects.RemoveAt(i);
				if(i>=m_View->Game->m_Scene->m_Objects.GetSize()) i--;
				while(i>=0 && m_View->Game->m_Scene->m_Objects[i]->m_Type != OBJECT_ENTITY) i--;
				if(i>=0) m_View->m_SelectedFreeObject = m_View->Game->m_Scene->m_Objects[i];
				else m_View->m_SelectedFreeObject = NULL;
				
				RedrawNodes();
				break;
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CScenePage::OnAddEntity() 
{
	m_View->SetUndoState("Add entity");
	int index = -1;
	int i;

	if(m_View->m_SelectedFree){
		for(i=0; i<m_View->Game->m_Scene->m_Objects.GetSize(); i++){
			if(m_View->Game->m_Scene->m_Objects[i]==m_View->m_SelectedFreeObject){
				index = i;
				break;
			}
		}
	}
	else{
		for(i=0; i<m_View->m_SelectedLayer->m_Nodes.GetSize(); i++){
			if(m_View->m_SelectedLayer->m_Nodes[i]==m_View->m_SelectedNode){
				index = i;
				break;
			}
		}
	}
	CPoint pos = m_View->GetDeviceScrollPosition();

	CAdEntity* ent = ::new CAdEntity(m_View->Game);
	ent->SetName((char*)LPCSTR(GetUniqueName("new entity")));
	ent->m_EditorAlwaysRegister = true;
	if(m_AddEntity.m_nMenuResult==ID_NEW_ENTITY_SPRITE){
		ent->m_PosX = max(0, pos.x - m_View->Game->m_Scene->m_EditorMarginH);
		ent->m_PosY = max(0, pos.y - m_View->Game->m_Scene->m_EditorMarginV);

		ent->m_Zoomable = false;
		ent->m_Shadowable = false;

		if(m_View->m_SelectedLayer != m_View->Game->m_Scene->m_MainLayer)
			ent->m_Registrable = false;
	}
	else if(m_AddEntity.m_nMenuResult==ID_NEW_ENTITY_REGION){
		ent->m_Region = ::new CBRegion(ent->Game);
		ent->m_Region->m_Points.Add(::new CBPoint(pos.x,     pos.y));
		ent->m_Region->m_Points.Add(::new CBPoint(pos.x+100, pos.y));
		ent->m_Region->m_Points.Add(::new CBPoint(pos.x+100, pos.y+100));
		ent->m_Region->m_Points.Add(::new CBPoint(pos.x,     pos.y+100));
		ent->m_Region->CreateRegion();
		ent->UpdatePosition();
	}
	else{
		ent->SetName((char*)LPCSTR(GetUniqueName("new sound")));
		ent->m_PosX = max(15, pos.x - m_View->Game->m_Scene->m_EditorMarginH);
		ent->m_PosY = max(15, pos.y - m_View->Game->m_Scene->m_EditorMarginV);
		ent->m_Subtype = ENTITY_SOUND;
		ent->m_Shadowable = false;
		ent->m_Zoomable = false;
		ent->m_Registrable = true;
		ent->m_EditorOnly = true;

		CBSprite* spr = ::new CBSprite(ent->Game, ent);
		if(!spr || FAILED(spr->LoadFile("entity_sound.sprite"))) ent->m_Sprite = NULL;
		else ent->m_Sprite = spr;
	}


	if(m_View->m_SelectedFree){
		if(index>=0) m_View->Game->m_Scene->m_Objects.InsertAt(index+1, ent);
		else m_View->Game->m_Scene->m_Objects.Add(ent);

		m_View->m_SelectedFree = true;
		m_View->m_SelectedFreeObject = ent;
	}
	else{
		CAdSceneNode* Node = ::new CAdSceneNode(m_View->Game);
		Node->m_Type = OBJECT_ENTITY;
		Node->m_Entity = ent;

		if(index>=0) m_View->m_SelectedLayer->m_Nodes.InsertAt(index+1, Node);
		else m_View->m_SelectedLayer->m_Nodes.Add(Node);

		m_View->m_SelectedNode = Node;	
	}

	RedrawNodes();
}


//////////////////////////////////////////////////////////////////////////
void CScenePage::OnAddRegion() 
{
	m_View->SetUndoState("Add region");

	int index = -1;

	CPoint pos = m_View->GetDeviceScrollPosition();
	pos -= CPoint(m_View->Game->m_Scene->m_EditorMarginH, m_View->Game->m_Scene->m_EditorMarginV);
	pos.x = max(0, pos.x);
	pos.y = max(0, pos.y);

	if(!m_View->m_SelectedWaypoints){
		for(int i=0; i<m_View->m_SelectedLayer->m_Nodes.GetSize(); i++){
			if(m_View->m_SelectedLayer->m_Nodes[i]==m_View->m_SelectedNode){
				index = i;
				break;
			}
		}
		
		CAdSceneNode* Node = ::new CAdSceneNode(m_View->Game);
		Node->m_Type = OBJECT_REGION;
		Node->m_Region = ::new CAdRegion(m_View->Game);
		Node->m_Region->SetName((char*)LPCSTR(GetUniqueName("new region")));
		Node->m_Region->m_Points.Add(::new CBPoint(pos.x,     pos.y));
		Node->m_Region->m_Points.Add(::new CBPoint(pos.x+100, pos.y));
		Node->m_Region->m_Points.Add(::new CBPoint(pos.x+100, pos.y+100));
		Node->m_Region->m_Points.Add(::new CBPoint(pos.x,     pos.y+100));
		Node->m_Region->CreateRegion();
		
		if(index>=0) m_View->m_SelectedLayer->m_Nodes.InsertAt(index+1, Node);
		else m_View->m_SelectedLayer->m_Nodes.Add(Node);
		
		m_View->m_SelectedNode = Node;
	}
	else{
		for(int i=0; i<m_View->Game->m_Scene->m_WaypointGroups.GetSize(); i++){
			if(m_View->Game->m_Scene->m_WaypointGroups[i]==m_View->m_SelectedWptGroup){
				index = i;
				break;
			}
		}
		
		CAdWaypointGroup* Wpt = ::new CAdWaypointGroup(m_View->Game);
		Wpt->SetName((char*)LPCSTR(GetUniqueName("new waypoints")));
		//Wpt->m_Points.Add(::new CBPoint(100, 100));
		//Wpt->m_Points.Add(new CBPoint(200, 200));
		
		if(index>=0) m_View->Game->m_Scene->m_WaypointGroups.InsertAt(index+1, Wpt);
		else m_View->Game->m_Scene->m_WaypointGroups.Add(Wpt);
		
		m_View->m_SelectedWptGroup = Wpt;
	}

	RedrawNodes();	
}


//////////////////////////////////////////////////////////////////////////
void CScenePage::OnAddPoint() 
{
	m_View->AddPoint();
}


//////////////////////////////////////////////////////////////////////////
void CScenePage::OnRemovePoint() 
{
	m_View->RemovePoint();
}


//////////////////////////////////////////////////////////////////////////
void CScenePage::OnAddScale() 
{
	m_View->AddScale();
}


//////////////////////////////////////////////////////////////////////////
CString CScenePage::GetUniqueName(CString Name)
{
	CString NewName = Name;

	if(m_View->Game->m_Scene->GetNodeByName((char*)LPCSTR(Name))!=NULL){
		int i = 2;
		do{
			NewName.Format("%s %d", Name, i);
			i++;
		} while (m_View->Game->m_Scene->GetNodeByName((char*)LPCSTR(NewName))!=NULL);
	}
	
	return NewName;
}


//////////////////////////////////////////////////////////////////////////
void CScenePage::OnAddRot() 
{
	m_View->AddRot();
}
