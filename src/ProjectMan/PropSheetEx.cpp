// PropSheetEx.cpp : implementation file
//

#include "stdafx.h"
#include "projectman.h"
#include "PropSheetEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropSheetEx

IMPLEMENT_DYNAMIC(CPropSheetEx, CPropertySheet)

CPropSheetEx::CPropSheetEx(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPropSheetEx::CPropSheetEx(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CPropSheetEx::~CPropSheetEx()
{
}


BEGIN_MESSAGE_MAP(CPropSheetEx, CPropertySheet)
	//{{AFX_MSG_MAP(CPropSheetEx)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropSheetEx message handlers

BOOL CPropSheetEx::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	CTabCtrl* Tab = GetTabControl();
	for(int i=0; i<Tab->GetItemCount(); i++){
		char szName[MAX_PATH];
		TCITEM Item;
		Item.mask = TCIF_TEXT;
		Item.pszText = szName;
		Item.cchTextMax = MAX_PATH;
		Tab->GetItem(i, &Item);

		Item.pszText = (char*)LOC(Item.pszText);
		Tab->SetItem(i, &Item);
	}
	
	// TODO: Add your specialized code here
	
	return bResult;
}
