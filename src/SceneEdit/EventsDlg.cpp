// EventsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SceneEdit.h"
#include "EventsDlg.h"
#include "EventPropDlg.h"
#include "../mfc_shared/utils_mfc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEventsDlg dialog



//////////////////////////////////////////////////////////////////////////
CEventsDlg::CEventsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEventsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEventsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_Game = NULL;
	m_EventSet = NULL;
	m_SelectedEvent = NULL;
}


//////////////////////////////////////////////////////////////////////////
CEventsDlg::~CEventsDlg()
{
	m_Supported.RemoveAll();
}


//////////////////////////////////////////////////////////////////////////
void CEventsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventsDlg)
	DDX_Control(pDX, IDC_REMOVE_EVENT, m_RemoveEvent);
	DDX_Control(pDX, IDC_EVENTS, m_Events);
	DDX_Control(pDX, IDC_EVENT_UP, m_EventUp);
	DDX_Control(pDX, IDC_EVENT_PROPERTIES, m_EventProperties);
	DDX_Control(pDX, IDC_EVENT_DOWN, m_EventDown);
	DDX_Control(pDX, IDC_ADD_EVENT, m_AddEvent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEventsDlg, CDialog)
	//{{AFX_MSG_MAP(CEventsDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_EVENTS, OnChangeEvent)
	ON_BN_CLICKED(IDC_ADD_EVENT, OnAddEvent)
	ON_BN_CLICKED(IDC_EVENT_UP, OnEventUp)
	ON_BN_CLICKED(IDC_EVENT_DOWN, OnEventDown)
	ON_BN_CLICKED(IDC_EVENT_PROPERTIES, OnEventProperties)
	ON_BN_CLICKED(IDC_REMOVE_EVENT, OnRemoveEvent)
	ON_NOTIFY(NM_DBLCLK, IDC_EVENTS, OnDblclkEvents)
	ON_BN_DOUBLECLICKED(IDC_EVENT_UP, OnEventUp)
	ON_BN_DOUBLECLICKED(IDC_EVENT_DOWN, OnEventDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEventsDlg message handlers


//////////////////////////////////////////////////////////////////////////
BOOL CEventsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Events.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_Events.InsertColumn(0,"Event", LVCFMT_LEFT, 110);
	m_Events.InsertColumn(1,"Script", LVCFMT_LEFT, 200);

	SetupButton(&m_EventUp,         "Event up",         IDB_MOVE_UP);
	SetupButton(&m_EventDown,       "Event down",       IDB_MOVE_DOWN);
	SetupButton(&m_AddEvent,        "Add event",        IDB_BCGBARRES_NEW);
	SetupButton(&m_RemoveEvent,     "Remove event",     IDB_BCGBARRES_DELETE);
	SetupButton(&m_EventProperties, "Event properties", IDB_PROPERTIES);

	Redraw();

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
void CEventsDlg::Redraw()
{
	m_Events.DeleteAllItems();	

	if(m_EventSet){

		for(int i=0; i<m_EventSet->m_Events.GetSize(); i++){
			m_Events.InsertItem(i, CBEvent::GetEventName(m_EventSet->m_Events[i]->m_Type));
			m_Events.SetItemData(i, (DWORD)m_EventSet->m_Events[i]);

			m_Events.SetItemText(i, 1, m_EventSet->m_Events[i]->m_Script);

			if(m_EventSet->m_Events[i]==m_SelectedEvent){
				m_Events.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
				m_Events.EnsureVisible(i, FALSE);
			}
		}
	}
	

	SetState();
}


//////////////////////////////////////////////////////////////////////////
void CEventsDlg::SetState()
{
	m_Events.EnableWindow(m_EventSet != NULL);

	m_EventUp.EnableWindow(m_EventSet && m_SelectedEvent && m_SelectedEvent != m_EventSet->m_Events[0]);
	m_EventDown.EnableWindow(m_EventSet && m_SelectedEvent && m_SelectedEvent != m_EventSet->m_Events[m_EventSet->m_Events.GetSize()-1]);
	m_AddEvent.EnableWindow(m_EventSet != NULL);
	m_RemoveEvent.EnableWindow(m_EventSet && m_SelectedEvent);
	m_EventProperties.EnableWindow(m_EventSet && m_SelectedEvent);
}



//////////////////////////////////////////////////////////////////////////
void CEventsDlg::OnChangeEvent(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if(m_EventSet && pNMListView->iItem!=-1){		
		if(m_Events.GetItemState(pNMListView->iItem, LVIS_SELECTED) & LVIS_SELECTED) m_SelectedEvent = (CBEvent*)m_Events.GetItemData(pNMListView->iItem);
	}
	*pResult = 0;

	SetState();
}


//////////////////////////////////////////////////////////////////////////
void CEventsDlg::OnEventUp() 
{
	for(int i=0; i<m_EventSet->m_Events.GetSize(); i++){
		if(m_EventSet->m_Events[i]==m_SelectedEvent){
			m_EventSet->m_Events[i] = m_EventSet->m_Events[i-1];
			m_EventSet->m_Events[i-1] = m_SelectedEvent;
			Redraw();
			break;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CEventsDlg::OnEventDown() 
{
	for(int i=0; i<m_EventSet->m_Events.GetSize(); i++){
		if(m_EventSet->m_Events[i]==m_SelectedEvent){
			m_EventSet->m_Events[i] = m_EventSet->m_Events[i+1];
			m_EventSet->m_Events[i+1] = m_SelectedEvent;
			Redraw();
			break;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CEventsDlg::OnAddEvent() 
{
	if(!m_EventSet) return;

	CEventPropDlg dlg;
	
	dlg.m_ScriptFile = "";
	dlg.m_EventType = EVENT_NONE;
	dlg.m_Game = m_Game;
	for(int i=0; i<m_Supported.GetSize(); i++) dlg.m_Supported.Add(m_Supported[i]);

	if(dlg.DoModal()==IDOK){
		CBEvent* event = ::new CBEvent(m_EventSet->Game);
		event->m_Type = dlg.m_EventType;
		event->SetScript((char*)LPCSTR(dlg.m_ScriptFile));

		int index = -1;
		for(int i=0; i<m_EventSet->m_Events.GetSize(); i++){
			if(m_EventSet->m_Events[i]==m_SelectedEvent){
				index = i;
				break;
			}
		}

		if(index>=0) m_EventSet->m_Events.InsertAt(index+1, event);
		else m_EventSet->m_Events.Add(event);

		m_SelectedEvent = event;
		Redraw();
	}	
}


//////////////////////////////////////////////////////////////////////////
void CEventsDlg::OnEventProperties() 
{
	if(!m_SelectedEvent) return;

	CEventPropDlg dlg;
	dlg.m_ScriptFile = CString(m_SelectedEvent->m_Script);
	dlg.m_EventType = m_SelectedEvent->m_Type;
	dlg.m_Game = m_Game;
	for(int i=0; i<m_Supported.GetSize(); i++) dlg.m_Supported.Add(m_Supported[i]);
	
	if(dlg.DoModal()==IDOK){
		m_SelectedEvent->m_Type = dlg.m_EventType;
		m_SelectedEvent->SetScript((char*)LPCSTR(dlg.m_ScriptFile));

		Redraw();
	}
}


//////////////////////////////////////////////////////////////////////////
void CEventsDlg::OnRemoveEvent() 
{
	if(MessageBox("Do you really want to remove this event?", NULL, MB_ICONQUESTION|MB_YESNO)!=IDYES) return;
	
	for(int i=0; i<m_EventSet->m_Events.GetSize(); i++){
		if(m_EventSet->m_Events[i]==m_SelectedEvent){

			delete m_EventSet->m_Events[i];
			m_EventSet->m_Events.RemoveAt(i);
			if(i>=m_EventSet->m_Events.GetSize()) i--;
			
			if(i<0) m_SelectedEvent = NULL;
			else m_SelectedEvent = m_EventSet->m_Events[i];

			Redraw();
			break;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CEventsDlg::OnDblclkEvents(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnEventProperties();
	*pResult = 0;
}


//////////////////////////////////////////////////////////////////////////
void CEventsDlg::AddSupportedType(TEventType Type)
{
	for(int i=0; i<m_Supported.GetSize(); i++){
		if(m_Supported[i]==Type) return;
	}

	m_Supported.Add(Type);
}
