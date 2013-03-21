// FormViewEx.cpp : implementation file
//

#include "stdafx.h"
#include "FormViewEx.h"
#include "DCGFStringTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormViewEx

IMPLEMENT_DYNAMIC(CFormViewEx, CFormView)


CFormViewEx::CFormViewEx(UINT nIDTemplate): CFormView(nIDTemplate)
{
	//{{AFX_DATA_INIT(CFormViewEx)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


CFormViewEx::~CFormViewEx()
{
}

void CFormViewEx::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormViewEx)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormViewEx, CFormView)
	//{{AFX_MSG_MAP(CFormViewEx)
	ON_WM_MEASUREITEM()
	ON_WM_MENUCHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormViewEx diagnostics

#ifdef _DEBUG
void CFormViewEx::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormViewEx::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFormViewEx message handlers


//////////////////////////////////////////////////////////////////////////
void CFormViewEx::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	BOOL setflag=FALSE;
	if(lpMeasureItemStruct->CtlType==ODT_MENU){
		if(IsMenu((HMENU)lpMeasureItemStruct->itemID)&&BCMenu::IsMenu((HMENU)lpMeasureItemStruct->itemID)){
			m_PopupMenu.MeasureItem(lpMeasureItemStruct);
			setflag=TRUE;
		}
	}
	if(!setflag)CFormView::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}


//////////////////////////////////////////////////////////////////////////
LRESULT CFormViewEx::OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu) 
{
	LRESULT lresult;
	if(BCMenu::IsMenu(pMenu))
		lresult=BCMenu::FindKeyboardShortcut(nChar, nFlags, pMenu);
	else
		lresult=CFormView::OnMenuChar(nChar, nFlags, pMenu);
	return(lresult);	
}


//////////////////////////////////////////////////////////////////////////
BOOL CFormViewEx::ShowPopupMenu(int MenuID, int ToolbarID, LPRECT Rect)
{
	m_PopupMenu.LoadMenu(MenuID);
	CDCGFStringTable::LocMenu(&m_PopupMenu);

	if(ToolbarID>=0) m_PopupMenu.LoadToolbar(ToolbarID);
	//m_PopupMenu.LoadToolbar(IDR_MAINFRAME);


	CPoint point;

	if(Rect){
		point.x = Rect->left;
		point.y = Rect->bottom;		
	}
	else GetCursorPos(&point);

	BCMenu* psub = (BCMenu*)m_PopupMenu.GetSubMenu(0);
	//BOOL RetValue = psub->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON /*| TPM_NONOTIFY | TPM_RETURNCMD*/, point.x, point.y, this, NULL);
	BOOL RetValue = psub->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON /*| TPM_NONOTIFY | TPM_RETURNCMD*/, point.x, point.y, AfxGetMainWnd(), NULL);

	m_PopupMenu.DestroyMenu();

	return RetValue;
}


//////////////////////////////////////////////////////////////////////////
void CFormViewEx::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	LOCWIN(this);	
}
