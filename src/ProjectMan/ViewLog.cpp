// ViewLog.cpp : implementation file
//

#include "stdafx.h"
#include "ProjectMan.h"
#include "ViewLog.h"
#include "LogItem.h"
#include "../MFCExt/resource.h"
#include "../MFCExt/objects.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewLog

IMPLEMENT_DYNCREATE(CViewLog, CFormViewEx)


//////////////////////////////////////////////////////////////////////////
CViewLog::CViewLog()
	: CFormViewEx(CViewLog::IDD)
{
	//{{AFX_DATA_INIT(CViewLog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_View = NULL;
}


//////////////////////////////////////////////////////////////////////////
CViewLog::~CViewLog()
{
}


//////////////////////////////////////////////////////////////////////////
void CViewLog::DoDataExchange(CDataExchange* pDX)
{
	CFormViewEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewLog)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewLog, CFormViewEx)
	//{{AFX_MSG_MAP(CViewLog)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewLog diagnostics

#ifdef _DEBUG
void CViewLog::AssertValid() const
{
	CFormViewEx::AssertValid();
}

void CViewLog::Dump(CDumpContext& dc) const
{
	CFormViewEx::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewLog message handlers


//////////////////////////////////////////////////////////////////////////
BOOL CViewLog::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if(m_View && m_View->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)) return TRUE;
	else return CFormViewEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


//////////////////////////////////////////////////////////////////////////
void CViewLog::OnSize(UINT nType, int cx, int cy) 
{
	SetScrollSizes(MM_TEXT, CSize(cx, cy));
	CFormViewEx::OnSize(nType, cx, cy);
	SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
		
	CRect rectClient;
	GetClientRect(&rectClient);

	CRect rectList;

	if(m_List.GetSafeHwnd()){
		m_List.GetWindowRect(&rectList);
		ScreenToClient(&rectList);
		rectList.right = rectList.left + (rectClient.right - rectClient.left);
		rectList.bottom = (rectClient.bottom - rectClient.top);
		m_List.MoveWindow(&rectList);

		int scrollbar_width = GetSystemMetrics(SM_CXVSCROLL);
		m_List.SetColumnWidth(0, rectList.Width() - scrollbar_width);
	}
}


//////////////////////////////////////////////////////////////////////////
void CViewLog::OnInitialUpdate() 
{
	CFormViewEx::OnInitialUpdate();
	
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	m_List.InsertColumn(0,"Log", LVCFMT_LEFT);
	
	m_ImageList.Create(IDB_NODES, 16, 1, RGB(255, 0, 255));
	m_List.SetImageList(&m_ImageList, LVSIL_SMALL);
}


//////////////////////////////////////////////////////////////////////////
void CViewLog::AddError(CString Text, CString Filename)
{
	CTime t = CTime::GetCurrentTime();
	CString Time;
	Time.Format("%02d:%02d:%02d", t.GetHour(), t.GetMinute(), t.GetSecond());
	Text = Time + "  " + Text;

	CLogItem* item = new CLogItem(CLogItem::LOG_ERROR, Text, Filename);
	int index = m_List.GetItemCount();
	m_List.InsertItem(index, Text, ICON_ERROR);
	m_List.SetItemData(index, (DWORD)item);
	m_List.EnsureVisible(index, FALSE);
}


//////////////////////////////////////////////////////////////////////////
void CViewLog::AddWarning(CString Text, CString Filename)
{
	CTime t = CTime::GetCurrentTime();
	CString Time;
	Time.Format("%02d:%02d:%02d", t.GetHour(), t.GetMinute(), t.GetSecond());
	Text = Time + "  " + Text;

	
	CLogItem* item = new CLogItem(CLogItem::LOG_WARNING, Text, Filename);
	int index = m_List.GetItemCount();
	m_List.InsertItem(index, Text, ICON_WARNING);
	m_List.SetItemData(index, (DWORD)item);
	m_List.EnsureVisible(index, FALSE);
}


//////////////////////////////////////////////////////////////////////////
void CViewLog::AddLog(CString Text, CString Filename)
{
	CTime t = CTime::GetCurrentTime();
	CString Time;
	Time.Format("%02d:%02d:%02d", t.GetHour(), t.GetMinute(), t.GetSecond());
	Text = Time + "  " + Text;

	
	CLogItem* item = new CLogItem(CLogItem::LOG_ENGINE, Text, Filename);
	int index = m_List.GetItemCount();
	m_List.InsertItem(index, Text, ICON_SCRIPT);
	m_List.SetItemData(index, (DWORD)item);
	m_List.EnsureVisible(index, FALSE);
}


//////////////////////////////////////////////////////////////////////////
void CViewLog::AddInfo(CString Text, CString Filename)
{
	CTime t = CTime::GetCurrentTime();
	CString Time;
	Time.Format("%02d:%02d:%02d", t.GetHour(), t.GetMinute(), t.GetSecond());
	Text = Time + "  " + Text;


	CLogItem* item = new CLogItem(CLogItem::LOG_INFO, Text, Filename);
	int index = m_List.GetItemCount();
	m_List.InsertItem(index, Text, ICON_INFO);
	m_List.SetItemData(index, (DWORD)item);
	m_List.EnsureVisible(index, FALSE);
}


//////////////////////////////////////////////////////////////////////////
void CViewLog::OnDestroy() 
{
	CFormViewEx::OnDestroy();
	if(!m_List) return;
	
	for(int i=0; i<m_List.GetItemCount(); i++){
		CLogItem* item = (CLogItem*)m_List.GetItemData(i);
		if(item) delete item;
	}

	m_View = NULL;
}


//////////////////////////////////////////////////////////////////////////
void CViewLog::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if(pNMListView->iItem!=-1){
		CLogItem* item = (CLogItem*)m_List.GetItemData(pNMListView->iItem);
		if(item){
			if(m_View && item->m_Filename!="") m_View->SelectFile(item->m_Filename);
		}
	}
	
	*pResult = 0;
}
