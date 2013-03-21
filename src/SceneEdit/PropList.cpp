// PropList.cpp : implementation file
//

#include "stdafx.h"
#include "PropList.h"
#include "DCGFStringTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PROPERTY_LEFT_BORDER 16


/////////////////////////////////////////////////////////////////////////////
// CPropList


//////////////////////////////////////////////////////////////////////////
CPropList::CPropList()
{
	m_SelectedItem = -1;
	m_ShowCategories = true;

	m_ControlsInitialized = false;
	m_MaxWidth = 10000;

	m_FocusedWidget = NULL;
	m_WidgProp = NULL;

	m_PropChangeCallback = NULL;
	m_CallbackData = NULL;

	m_PropFocusCallback = NULL;
	m_FocusCallbackData = NULL;
		
	m_ValidEdit = false;
}


//////////////////////////////////////////////////////////////////////////
CPropList::~CPropList()
{
	Cleanup();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CPropList::SetPropChangeCallback(PROPERTY_CHANGED_CALLBACK lpCallback, void* Data){

	m_PropChangeCallback = lpCallback;
	m_CallbackData = Data;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CPropList::SetPropFocusCallback(PROPERTY_FOCUS_CALLBACK lpCallback, void* Data){

	m_PropFocusCallback = lpCallback;
	m_FocusCallbackData = Data;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CPropList::Cleanup()
{
	for(int i=0; i<m_Categories.GetSize(); i++) delete m_Categories[i];
	m_Categories.RemoveAll();
	m_SelectedItem = -1;
	m_FocusedWidget = NULL;
	m_ValidEdit = false;
	m_WidgProp = NULL;

	return S_OK;
}



BEGIN_MESSAGE_MAP(CPropList, CListBox)
	//{{AFX_MSG_MAP(CPropList)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(3, OnButton)
	ON_CBN_SELCHANGE(2, OnSelchangeCombo)
	ON_EN_KILLFOCUS(1, OnKillfocusEdit)
	ON_EN_SETFOCUS(1, OnSetfocusEdit)
	ON_WM_ERASEBKGND()	
	ON_CONTROL_REFLECT(LBN_SELCHANGE, OnSelChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropList message handlers


//////////////////////////////////////////////////////////////////////////
void CPropList::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	if(!m_ControlsInitialized){
		CRect rc(0,0,1,1);
		m_Edit.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, rc, this, 1);	
		m_Combo.Create(WS_CHILD | WS_TABSTOP | CBS_DROPDOWNLIST, rc, this, 2);	
		m_Button.Create("...", WS_CHILD | WS_TABSTOP /*| WS_BORDER*/, rc, this, 3);

		m_Edit.SetFont(GetFont());
		m_Combo.SetFont(GetFont());
		m_Button.SetFont(GetFont());
		m_Button.OffsetColor(CButtonST::BTNST_COLOR_BK_FOCUS, 20);

		m_ControlsInitialized = true;
	}
}


//////////////////////////////////////////////////////////////////////////
int CPropList::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct) 
{
	// TODO: Add your code to determine the sorting order of the specified items
	// return -1 = item 1 sorts before item 2
	// return 0 = item 1 and item 2 sort the same
	// return 1 = item 1 sorts after item 2
	
	return 0;
}


//////////////////////////////////////////////////////////////////////////
void CPropList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if(lpDrawItemStruct->itemAction & ODA_FOCUS)
		return;


	CPLProperty* pPropertyItem = (CPLProperty*)lpDrawItemStruct->itemData;
	ASSERT(pPropertyItem != NULL);


	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

	// Rámèek
	CRect rect;
	GetClientRect(rect);

	int nLeftBorder = rect.left;
		
	if(m_ShowCategories) nLeftBorder += PROPERTY_LEFT_BORDER;

	// + / -
	if(pPropertyItem->m_Type==CPLProperty::PROP_CATEGORY){

		CRect rcSign(lpDrawItemStruct->rcItem);
		rcSign.right = nLeftBorder;

		rcSign.DeflateRect(4, 4);

		rcSign.bottom = rcSign.top + (rcSign.right - rcSign.left);

		rcSign.right += 1;
		rcSign.bottom += 1;

		dc.FillRect(rcSign, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
		dc.FrameRect(rcSign, CBrush::FromHandle((HBRUSH)GetStockObject(BLACK_BRUSH)));
		
		CPoint ptCenter(rcSign.CenterPoint());

		// minus		
		dc.MoveTo(ptCenter.x - 2, ptCenter.y);
		dc.LineTo(ptCenter.x + 3, ptCenter.y);

		if(!((CPLCategory*)(pPropertyItem))->ChildrenVisible()){
			// plus
			dc.MoveTo(ptCenter.x, ptCenter.y - 2);
			dc.LineTo(ptCenter.x, ptCenter.y + 3);
		}
	}

	// Vertikála v ¾avo
	CPen pen(PS_SOLID, 1, RGB(198, 198, 198));
	CPen* pOldPen = dc.SelectObject(&pen);

	if(m_ShowCategories){
		dc.MoveTo(nLeftBorder, rect.top);
		dc.LineTo(nLeftBorder, rect.bottom);
	}

	// Spodná èiara
	rect = lpDrawItemStruct->rcItem;
	if(m_ShowCategories) rect.left += PROPERTY_LEFT_BORDER;

	int nBottom = rect.bottom - 1;

	dc.MoveTo(nLeftBorder, nBottom);
	dc.LineTo(rect.right, nBottom);

	// Odde¾ovaè ståpcov
	nLeftBorder += min(m_MaxWidth, rect.Width() / 2);

	if(pPropertyItem->m_Type!=CPLProperty::PROP_CATEGORY){
		dc.MoveTo(nLeftBorder, rect.top);
		dc.LineTo(nLeftBorder, nBottom);
	}

	// Text položky
	if(m_ShowCategories) rect.left += 1;
	rect.bottom -= 1;

	if(pPropertyItem->m_Type!=CPLProperty::PROP_CATEGORY)
		rect.right = nLeftBorder;

	int nCrBackground, nCrText;

	if((lpDrawItemStruct->itemAction | ODA_SELECT) && (lpDrawItemStruct->itemState & ODS_SELECTED)){
		nCrBackground = COLOR_HIGHLIGHT;
		nCrText = COLOR_HIGHLIGHTTEXT;
	}
	else{
		nCrBackground = COLOR_WINDOW;
		nCrText = COLOR_WINDOWTEXT;
	}

	COLORREF crBackground = ::GetSysColor(nCrBackground);
	COLORREF crText = ::GetSysColor(nCrText);

	dc.FillSolidRect(rect, crBackground);
	COLORREF crOldBkColor = dc.SetBkColor(crBackground);
	COLORREF crOldTextColor = dc.SetTextColor(crText);

	rect.left += 3;
	rect.right -= 3;

	CFont* pOldFont = NULL;
	CFont fontLabel;

	if(pPropertyItem->m_Type==CPLProperty::PROP_CATEGORY){
		LOGFONT logFont;
		CFont* pFont = GetFont();
		pFont->GetLogFont(&logFont);

		logFont.lfWeight = FW_BOLD;
		fontLabel.CreateFontIndirect(&logFont);

		pOldFont = dc.SelectObject(&fontLabel);
	}

	// Popis položky
	dc.DrawText(pPropertyItem->GetName(), &rect, DT_SINGLELINE|DT_VCENTER);

	dc.SelectObject(pOldPen);
	dc.SetTextColor(crOldTextColor);
	dc.SetBkColor(crOldBkColor);
	//dc.SetBkMode(TRANSPARENT);

	if(pOldFont != NULL)
		dc.SelectObject(pOldFont);

	// Hodnota položky
	rect = lpDrawItemStruct->rcItem;
	GetItemValueRect(rect);

	if(pPropertyItem->m_Type!=CPLProperty::PROP_CATEGORY){
		dc.FillRect(rect, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	}


	bool selected = (lpDrawItemStruct->itemState & ODS_SELECTED);

	if(!selected){
		pPropertyItem->DisplayValue(&dc, rect);
	}
	else{
						
		// draw property value
		switch(pPropertyItem->m_Type){
			case CPLProperty::PROP_EDIT:
				m_Edit.SetWindowText(pPropertyItem->GetText());
				
				if(m_FocusedWidget==&m_Edit && m_WidgProp==pPropertyItem) break;

				rect.DeflateRect(0, 1);
				m_Edit.MoveWindow(rect, FALSE);
				m_Edit.ShowWindow(SW_SHOW);
				//m_Edit.SetFocus();
				m_Edit.SetReadOnly(FALSE);				
				m_FocusedWidget = &m_Edit;
				m_WidgProp = pPropertyItem;

				m_Combo.ShowWindow(SW_HIDE);
				m_Button.ShowWindow(SW_HIDE);
			break;
			
			case CPLProperty::PROP_COMBO:
			{
				if(m_FocusedWidget==&m_Combo && m_WidgProp==pPropertyItem) break;

				int Height = rect.Height();
				CRect rc2 = rect;
				rc2.top--;
				rc2.bottom = rc2.top + 100;
				m_Combo.ShowWindow(SW_HIDE);
				m_Combo.MoveWindow(rc2, FALSE);
				m_Combo.SetItemHeight(-1, Height-2);

				((CPLPropertyCombo*)pPropertyItem)->PopulateCombo(&m_Combo);
				
				m_Combo.ShowWindow(SW_SHOW);
				//m_Combo.SetFocus();				
				m_FocusedWidget = &m_Combo;
				m_WidgProp = pPropertyItem;

				m_Edit.ShowWindow(SW_HIDE);
				m_Button.ShowWindow(SW_HIDE);
			}
			break;

			case CPLProperty::PROP_EDIT_EX:{				
				CPLPropertyEditEx* pItem = (CPLPropertyEditEx*)pPropertyItem;
				m_Edit.SetWindowText(pItem->GetText());				

				if(pItem->m_ReadOnly)
					if(m_FocusedWidget==&m_Button && m_WidgProp==pItem) break;
				else
					if(m_FocusedWidget==&m_Edit && m_WidgProp==pItem) break;

				m_Combo.ShowWindow(SW_HIDE);
				
				CRect rect2 = rect;
				rect2.left = rect2.right - rect.Height();
				
				rect.DeflateRect(0, 1);
				rect.right -= rect.Height()+3;
				m_Edit.MoveWindow(rect, FALSE);
				m_Button.MoveWindow(rect2, TRUE);

				m_Edit.ShowWindow(SW_SHOW);								
				m_Button.ShowWindow(SW_SHOW);
				m_Button.SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
				//m_Button.SetFocus();								
				m_Button.Invalidate();
				if(pItem->m_ReadOnly){
					m_Edit.SetReadOnly(TRUE);
					m_FocusedWidget = &m_Button;
				}
				else{
					m_Edit.SetReadOnly(FALSE);
					m_FocusedWidget = &m_Edit;
				}
				m_WidgProp = pPropertyItem;
			}

			break;

			case CPLProperty::PROP_COLOR:{			
				CRect rect2 = rect;
				rect2.right -= rect.Height()+1;
				pPropertyItem->DisplayValue(&dc, rect2);

				if(m_FocusedWidget==&m_Button && m_WidgProp==pPropertyItem) break;

				m_Edit.ShowWindow(SW_HIDE);
				m_Combo.ShowWindow(SW_HIDE);

				rect2 = rect;
				rect2.left = rect2.right - rect.Height();
				m_Button.MoveWindow(rect2, TRUE);
				m_Button.ShowWindow(SW_SHOW);
				m_Button.SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
				//m_Button.SetFocus();
				m_Button.Invalidate();
				m_FocusedWidget = &m_Button;
				m_WidgProp = pPropertyItem;
			}
			break;

			default:
				m_Edit.ShowWindow(SW_HIDE);
				m_Combo.ShowWindow(SW_HIDE);
				m_Button.ShowWindow(SW_HIDE);

				m_FocusedWidget = NULL;
				m_WidgProp = NULL;
		}
	}
		

	//TRACE("\nDraw value of %s (%d)", pPropertyItem->GetName(), (int)selected);

	dc.Detach();
}


//////////////////////////////////////////////////////////////////////////
void CPropList::GetItemValueRect(CRect& rect)
{
	if(m_ShowCategories) rect.left += PROPERTY_LEFT_BORDER;
	rect.left += min(m_MaxWidth, rect.Width() / 2);

	rect.DeflateRect(1, 0, 0, 1);
}


//////////////////////////////////////////////////////////////////////////
CPLProperty* CPropList::GetProperty(int Index)
{
	return Index >= 0 && Index < GetCount() ? (CPLProperty*)GetItemData(Index): NULL;
}


//////////////////////////////////////////////////////////////////////////
CPLProperty* CPropList::GetProperty(CString ID, CString CategoryID)
{
	for(int i=0; i<m_Categories.GetSize(); i++){
		CPLCategory* cat = m_Categories[i];

		if(CategoryID=="" || CategoryID==cat->GetID()){
			for(int j=0; j<cat->m_Properties.GetSize(); j++){
				CPLProperty* prop = cat->m_Properties[j];
				if(prop->GetID()==ID) return prop;
			}
		}
	}
	return NULL;
}


//////////////////////////////////////////////////////////////////////////
CPLPropertyEdit* CPropList::GetPropertyEdit(CString ID, CString CategoryID)
{
	return (CPLPropertyEdit*)GetProperty(ID, CategoryID);
}


//////////////////////////////////////////////////////////////////////////
CPLPropertyCombo* CPropList::GetPropertyCombo(CString ID, CString CategoryID)
{
	return (CPLPropertyCombo*)GetProperty(ID, CategoryID);
}


//////////////////////////////////////////////////////////////////////////
CPLPropertyColor* CPropList::GetPropertyColor(CString ID, CString CategoryID)
{
	return (CPLPropertyColor*)GetProperty(ID, CategoryID);
}


//////////////////////////////////////////////////////////////////////////
void CPropList::PreSubclassWindow() 
{
	CListBox::PreSubclassWindow();
	ModifyStyle(0, LBS_OWNERDRAWVARIABLE|LBS_NOINTEGRALHEIGHT|LBS_NOTIFY);
}


//////////////////////////////////////////////////////////////////////////
void CPropList::ResetContent()
{
	m_SelectedItem = -1;
	CListBox::ResetContent();
}


//////////////////////////////////////////////////////////////////////////
void CPropList::OnSelChange()
{
	int nItem = GetCurSel();

	if(m_SelectedItem != nItem){

		CPLProperty* pPropertyItem = GetProperty(nItem);
		ASSERT(pPropertyItem != NULL);

		CRect rect;

		GetItemRect(nItem, rect);
		GetItemValueRect(rect);

		//pPropertyItem->CreateInPlaceControl(this, rect, m_pWndInPlaceControl);

		m_SelectedItem = nItem;

		if(m_PropFocusCallback) m_PropFocusCallback(pPropertyItem, pPropertyItem->m_Category, m_FocusCallbackData);
	}
}


//////////////////////////////////////////////////////////////////////////
void CPropList::Populate(bool Force)
{
	m_ValidEdit = false;
	m_FocusedWidget = NULL;
	
	CDC* dc = GetDC();

	CFont* pFont = GetFont();
	CFont* pOldFont = dc->SelectObject(pFont);	
		
	m_MaxWidth = 0;

	int CurSel = GetCurSel();
	CPLProperty* OrigProp = GetProperty(CurSel);
	if(Force) OrigProp = NULL;

	int item;

	ResetContent();
	for(int i=0; i<m_Categories.GetSize(); i++){
		CPLCategory* cat = m_Categories[i];

		if(cat->IsHidden()) continue;

		if(m_ShowCategories){
			//m_MaxWidth = max(m_MaxWidth, dc->GetTextExtent(cat->GetName()).cx);
			item = AddString(cat->GetName());			
			SetItemData(item, (DWORD)cat);

			if(OrigProp!=NULL && cat==OrigProp) CurSel = item;
		}
		if(cat->m_Expanded || !m_ShowCategories){
			for(int j=0; j<cat->m_Properties.GetSize(); j++){
				CPLProperty* prop = cat->m_Properties[j];
				if(prop->IsHidden()) continue;
				m_MaxWidth = max(m_MaxWidth, dc->GetTextExtent(prop->GetName()).cx);
				item = AddString(prop->GetName());
				SetItemData(item, (DWORD)prop);

				if(OrigProp!=NULL && prop==OrigProp) CurSel = item;
			}
		}
	}

	if(CurSel!= LB_ERR && CurSel<GetCount()) SetCurSel(CurSel);
	else SetCurSel(0);

	//if(Force) SetCurSel(0);

	dc->SelectObject(pOldFont);	
	ReleaseDC(dc);
	if(!m_MaxWidth) m_MaxWidth = 10000;
	else m_MaxWidth+=/*PROPERTY_LEFT_BORDER+*/10;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CPropList::ShowCategory(CString ID, bool Show, bool RedrawNow)
{
	for(int i=0; i<m_Categories.GetSize(); i++){
		if(m_Categories[i]->GetID()==ID){
			bool Orig = !m_Categories[i]->IsHidden();
			if(Orig!=Show){
				m_Categories[i]->m_Hidden = !Show;
				if(RedrawNow){
					Populate();
					Redraw();
				}
			}
			return S_OK;
		}
	}
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CPropList::ShowProperty(CString ID, CString CatID, bool Show, bool RedrawNow){
	CPLProperty* prop = GetProperty(ID, CatID);
	if(prop){
		bool Orig = !prop->IsHidden();
		if(Orig!=Show){
			prop->m_Hidden = !Show;
			if(RedrawNow){
				Populate();
				Redraw();
			}
		}
		return S_OK;
	}
	else return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
CPLCategory* CPropList::AddCategory(CString ID, CString Name)
{
	for(int i=0; i<m_Categories.GetSize(); i++){
		if(m_Categories[i]->GetID()==ID) return m_Categories[i];
	}

	CPLCategory* cat = new CPLCategory(ID, Name);
	if(cat){
		cat->m_PropertyList = this;
		m_Categories.Add(cat);
		return cat;
	}
	else return NULL;
}


//////////////////////////////////////////////////////////////////////////
bool CPropList::AddProperty(CPLCategory* Category, CPLProperty* Property)
{
	if(!Property) return false;

	ASSERT(Property->m_Type!=CPLProperty::PROP_UNKNOWN && Property->m_Type!=CPLProperty::PROP_CATEGORY);

	Property->m_PropertyList = this;
	Property->m_Category = Category;
	Category->m_Properties.Add(Property);
	return true;
}


//////////////////////////////////////////////////////////////////////////
void CPropList::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CListBox::OnLButtonDown(nFlags, point);

	if(m_ShowCategories && point.x <= PROPERTY_LEFT_BORDER){
		DoCollapseExpand(GetCurSel());
	}

	if(m_FocusedWidget){
		m_FocusedWidget->SetFocus();
	}
}


//////////////////////////////////////////////////////////////////////////
void CPropList::DoCollapseExpand(int Index)
{
		CPLProperty* prop = GetProperty(Index);
		if(prop && prop->m_Type==CPLProperty::PROP_CATEGORY){
			((CPLCategory*)prop)->m_Expanded = !((CPLCategory*)prop)->m_Expanded;
			Populate();
			Invalidate();
		}
}


//////////////////////////////////////////////////////////////////////////
void CPropList::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CListBox::OnLButtonDblClk(nFlags, point);
	DoCollapseExpand(GetCurSel());
}


//////////////////////////////////////////////////////////////////////////
void CPropList::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CListBox::OnHScroll(nSBCode, nPos, pScrollBar);
	Redraw();
}


//////////////////////////////////////////////////////////////////////////
void CPropList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CListBox::OnVScroll(nSBCode, nPos, pScrollBar);
	Redraw();
}


//////////////////////////////////////////////////////////////////////////
BOOL CPropList::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	BOOL ret = CListBox::OnMouseWheel(nFlags, zDelta, pt);
	Redraw();
	return ret;
}


//////////////////////////////////////////////////////////////////////////
void CPropList::OnButton() 
{
	CPLProperty* prop = GetProperty(GetCurSel());
	if(prop){
		if(prop->OnButton()){
			Invalidate();
			if(m_PropChangeCallback) m_PropChangeCallback(prop, prop->m_Category, m_CallbackData);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CPropList::OnSelchangeCombo() 
{
	CPLProperty* prop = GetProperty(GetCurSel());
	if(prop && prop->m_Type==CPLProperty::PROP_COMBO){
		CPLPropertyCombo* cprop = (CPLPropertyCombo*)prop;

		int NewVal = m_Combo.GetItemData(m_Combo.GetCurSel());
		if(NewVal!=cprop->m_CurrentValue){
			cprop->m_CurrentValue = m_Combo.GetItemData(m_Combo.GetCurSel());
			if(m_PropChangeCallback) m_PropChangeCallback(prop, prop->m_Category, m_CallbackData);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CPropList::OnKillfocusEdit() 
{
	if(!m_ValidEdit) return;

	CString ErrorString, Value;
	CPLPropertyEdit* prop = (CPLPropertyEdit*)GetProperty(GetCurSel());
	if(prop){
		CString OldVal = prop->m_Value;
		m_Edit.GetWindowText(Value);
		bool b = prop->SetValueValidate(Value, ErrorString);
		if(!b){
			MessageBox(ErrorString, NULL, MB_OK|MB_ICONERROR);
			m_Edit.SetFocus();
			return;
		}
		else{
			if(prop->m_Value!=OldVal){
				Invalidate();
				if(m_PropChangeCallback) m_PropChangeCallback(prop, prop->m_Category, m_CallbackData);
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CPropList::OnSetfocusEdit() 
{
	m_ValidEdit = true;
	m_Edit.SetSel(0, -1);
}

//////////////////////////////////////////////////////////////////////////
BOOL CPropList::OnEraseBkgnd(CDC* pDC) 
{
	return CListBox::OnEraseBkgnd(pDC);
}


//////////////////////////////////////////////////////////////////////////
BOOL CPropList::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN){		
		int nItem = GetCurSel();
		char nChar = char(pMsg->wParam);
		bool bDone = true;

		CPLProperty* pPropertyItem = GetProperty(nItem);

		if(pPropertyItem != NULL && pPropertyItem->m_Type != CPLProperty::PROP_CATEGORY){
			switch(nChar){
					case VK_RETURN:
						if(m_FocusedWidget){
							if(GetFocus()==m_FocusedWidget) SetFocus();
							else m_FocusedWidget->SetFocus();
						}
					break;

					case VK_TAB:
						if(m_FocusedWidget){
							if(GetFocus()!=m_FocusedWidget) m_FocusedWidget->SetFocus();
							else bDone = false;
						}
					break;

					case VK_ESCAPE:
						SetFocus();
					break;

					case VK_DOWN:
					case VK_UP:
					case VK_LEFT:
					case VK_RIGHT:
						if(m_FocusedWidget && m_FocusedWidget == &m_Button && GetFocus()==m_FocusedWidget);
						else bDone = false;
					break;

					default:
						bDone = false;
			}
		}
		else if(pPropertyItem != NULL && pPropertyItem->m_Type == CPLProperty::PROP_CATEGORY){
			switch(nChar){
				case VK_RETURN:
					DoCollapseExpand(nItem);
				break;

				case VK_ADD:
					if(!((CPLCategory*)pPropertyItem)->m_Expanded)
						DoCollapseExpand(nItem);
				break;
					
				case VK_SUBTRACT:
					if(((CPLCategory*)pPropertyItem)->m_Expanded)
						DoCollapseExpand(nItem);
				break;

				default:
					bDone = false;				
			}
		}

		else
			bDone = false;

		if(bDone)
			return TRUE;
	}

	return CListBox::PreTranslateMessage(pMsg);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CPropList::Redraw(bool ForceAll)
{
	if(ForceAll) m_WidgProp = NULL;
	Invalidate();
	return S_OK;
}







//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// CPLProperty Class
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPLProperty::CPLProperty(CString ID, CString Name)
{
	m_Type = CPLProperty::PROP_UNKNOWN;
	m_Category = NULL;
	m_ID = ID;
	m_Name = Name==""?ID:Name;
	m_PropertyList = NULL;
	m_Value = "";
	m_Hidden = false;
	m_HelpFile = "";
}

//////////////////////////////////////////////////////////////////////////
CPLProperty::~CPLProperty()
{

}

//////////////////////////////////////////////////////////////////////////
bool CPLProperty::IsHidden()
{
	return m_Hidden;
}


//////////////////////////////////////////////////////////////////////////
CString CPLProperty::GetName()
{
	return m_Name;
}

//////////////////////////////////////////////////////////////////////////
CString CPLProperty::GetID()
{
	return m_ID;
}


//////////////////////////////////////////////////////////////////////////
bool CPLProperty::DisplayValue(CDC* dc, CRect Rect)
{
	dc->DrawText(GetText(), &Rect, DT_SINGLELINE|DT_VCENTER);
	return true;
}


//////////////////////////////////////////////////////////////////////////
void CPLProperty::SetName(CString Name)
{
	m_Name = Name;
}


//////////////////////////////////////////////////////////////////////////
void CPLProperty::SetValue(CString Value)
{
	m_Value = Value;
}


//////////////////////////////////////////////////////////////////////////
CString CPLProperty::GetValue()
{
	return m_Value;
}


//////////////////////////////////////////////////////////////////////////
CString CPLProperty::GetText()
{
	return m_Value;
}

//////////////////////////////////////////////////////////////////////////
bool CPLProperty::OnButton()
{
	return false;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// CPLPropertyEdit Class
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPLPropertyEdit::CPLPropertyEdit(CString ID, CString Name):CPLProperty(ID, Name)
{
	m_Type = CPLProperty::PROP_EDIT;
	m_EditType = EDIT_ANY;

	m_CheckRange = false;
	m_MinValue = m_MaxValue = 0;
}


//////////////////////////////////////////////////////////////////////////
CPLPropertyEdit::~CPLPropertyEdit()
{

}


//////////////////////////////////////////////////////////////////////////
void CPLPropertyEdit::SetValidation(TPropertyEditType Type, bool CheckRange, int MinValue, int MaxValue){
	m_CheckRange = CheckRange;
	m_EditType = Type;
	m_MinValue = MinValue;
	m_MaxValue = MaxValue;
}

//////////////////////////////////////////////////////////////////////////
bool CPLPropertyEdit::SetValueValidate(CString Value, CString& ErrorString)
{
	int i;

	CString NewVal = "";
	if(m_EditType==EDIT_SIGNED || m_EditType==EDIT_UNSIGNED){
		bool IsOK = true;

		if(Value.GetLength()<=0) IsOK = false;

		for(i=0; i<Value.GetLength(); i++){
			if(Value[i]==' ') continue;
			else if(Value[i]>='0' && Value[i]<='9'){
				NewVal += Value[i];
				continue;
			}
			else if(Value[i]=='-' && m_EditType==EDIT_SIGNED && NewVal==""){
				NewVal += Value[i];
				continue;
			}
			else{
				IsOK = false;
				break;
			}
		}

		if(!IsOK){
			ErrorString.Format(CString("'%s' ") + LOC("/str1065/must be an") + " %s " + LOC("/str1066/integer number."), GetName(), m_EditType==EDIT_UNSIGNED?LOC("/str1067/unsigned"):"");
			return false;
		}

		i = atoi(LPCSTR(NewVal));
		if(m_CheckRange && (i<m_MinValue || i>m_MaxValue)){
			ErrorString.Format(CString("'%s' ") + LOC("/str1068/must be a number in range") + " %d - %d", GetName(), m_MinValue, m_MaxValue);
			return false;
		}
		else m_Value = NewVal;
	}
	else if(m_EditType==EDIT_ANY){
		if(m_CheckRange){
			if(Value.GetLength()>m_MaxValue){
				ErrorString.Format(CString("'%s' ") + LOC("/str1069/cannot be longer than") + " %d %s.", GetName(), m_MaxValue, m_MaxValue>1?LOC("/str1070/characters"):LOC("/str1071/character"));
				return false;
			}
			else if(Value.GetLength()<m_MinValue){
				ErrorString.Format(CString("'%s' ") + LOC("/str1072/must be at least") + " %d %s " + LOC("/str1073/long."), GetName(), m_MinValue, m_MinValue>1?LOC("/str1070/characters"):LOC("/str1071/character"));
				return false;
			}
			else m_Value = Value;
		}
		else m_Value = Value;
	}
	else m_Value = Value;

	return true;

}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// CPLPropertyEditEx Class
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPLPropertyEditEx::CPLPropertyEditEx(CString ID, CString Name):CPLPropertyEdit(ID, Name)
{
	m_Type = CPLProperty::PROP_EDIT_EX;
	m_ReadOnly = false;

	m_ButtonPressCallback = NULL;
	m_CallbackData = NULL;
}


//////////////////////////////////////////////////////////////////////////
CPLPropertyEditEx::~CPLPropertyEditEx()
{

}


//////////////////////////////////////////////////////////////////////////
HRESULT CPLPropertyEditEx::SetButtonPressCallback(BUTTON_PRESS_CALLBACK lpCallback, void* Data)
{
	m_ButtonPressCallback = lpCallback;
	m_CallbackData = Data;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
bool CPLPropertyEditEx::OnButton()
{
	if(m_ButtonPressCallback){
		return m_ButtonPressCallback(&m_PropertyList->m_Button, this, m_Category, m_CallbackData);
	}
	else return CPLPropertyEdit::OnButton();
}


//////////////////////////////////////////////////////////////////////
// CPLCategory Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
CPLCategory::CPLCategory(CString ID, CString Name):CPLProperty(ID, Name)
{
	m_Type = CPLProperty::PROP_CATEGORY;
	m_Expanded = true;
}


//////////////////////////////////////////////////////////////////////////
CPLCategory::~CPLCategory()
{
	for(int i=0; i<m_Properties.GetSize(); i++) delete m_Properties[i];
	m_Properties.RemoveAll();
}


//////////////////////////////////////////////////////////////////////////
bool CPLCategory::ChildrenVisible()
{
	return m_Expanded;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// CPLPropertyCombo Class
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
CPLPropertyCombo::CPLPropertyCombo(CString ID, CString Name):CPLProperty(ID, Name)
{
	m_Type = CPLProperty::PROP_COMBO;
	m_CurrentValue = -1;
}

//////////////////////////////////////////////////////////////////////////
CPLPropertyCombo::~CPLPropertyCombo()
{
	for(int i=0; i<m_Values.GetSize(); i++) delete m_Values[i];
	m_Values.RemoveAll();
}


//////////////////////////////////////////////////////////////////////////
CString CPLPropertyCombo::GetText(){
	if(m_CurrentValue<0 || m_CurrentValue>=m_Values.GetSize()) return "";
	else return m_Values[m_CurrentValue]->m_Label;
}


//////////////////////////////////////////////////////////////////////////
CString CPLPropertyCombo::GetValue(){
	if(m_CurrentValue<0 || m_CurrentValue>=m_Values.GetSize()) return "";
	else return m_Values[m_CurrentValue]->m_Value;
}


//////////////////////////////////////////////////////////////////////////
int CPLPropertyCombo::GetValueInt(){
	if(m_CurrentValue<0 || m_CurrentValue>=m_Values.GetSize()) return -1;
	else return m_CurrentValue;
}


//////////////////////////////////////////////////////////////////////////
bool CPLPropertyCombo::SetValue(CString Value){
	for(int i=0; i<m_Values.GetSize(); i++){
		if(m_Values[i]->m_Value==Value){
			m_CurrentValue = i;
			return true;
		}
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////
bool CPLPropertyCombo::AddValue(CString Label, CString Value){
	TComboValue* val = new TComboValue(Label, Value);
	m_Values.Add(val);
	if(m_CurrentValue<0) m_CurrentValue = 0;
	return true;
}


//////////////////////////////////////////////////////////////////////////
bool CPLPropertyCombo::SetupBool(bool Init){
	for(int i=0; i<m_Values.GetSize(); i++) delete m_Values[i];
	m_Values.RemoveAll();

	AddValue(LOC("/str1001/No"));
	AddValue(LOC("/str1000/Yes"));

	m_CurrentValue = Init?1:0;

	return true;
}


//////////////////////////////////////////////////////////////////////////
bool CPLPropertyCombo::PopulateCombo(CComboBox* Combo){
	Combo->ResetContent();
	for(int i=0; i<m_Values.GetSize(); i++){
		int item = Combo->AddString(m_Values[i]->m_Label);
		if(item!=CB_ERR){
			Combo->SetItemData(item, i);
		}
	}
	
	if(m_CurrentValue==-1) m_CurrentValue = min(0, m_Values.GetSize()-1);
	if(m_CurrentValue>=m_Values.GetSize()) m_CurrentValue = m_Values.GetSize() - 1;

	if(m_CurrentValue>=0) Combo->SetCurSel(m_CurrentValue);

	return true;
}

//////////////////////////////////////////////////////////////////////
// CPLPropertyFile Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
CPLPropertyFile::CPLPropertyFile(CString ID, CString Name):CPLPropertyEditEx(ID, Name)
{
	m_ReadOnly = true;
}


//////////////////////////////////////////////////////////////////////////
CPLPropertyFile::~CPLPropertyFile()
{

}

//////////////////////////////////////////////////////////////////////////
bool CPLPropertyFile::OnButton()
{
	if(!m_PropertyList) return false;

	CString Filter;
	Filter = CString(LOC("/str1003/All Files")) + " (*.*)|*.*||";
	CFileDialog dlg(TRUE, NULL, m_Value, OFN_HIDEREADONLY|OFN_ENABLESIZING|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_NOCHANGEDIR, Filter, m_PropertyList->GetParent());
	
	if(dlg.DoModal()==IDOK){
		m_Value = dlg.GetPathName();
		return true;
	}
	else return false;
}


//////////////////////////////////////////////////////////////////////
// CPLPropertyColor Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
CPLPropertyColor::CPLPropertyColor(CString ID, CString Name):CPLProperty(ID, Name)
{
	m_Type = CPLProperty::PROP_COLOR;

	m_Red = m_Green = m_Blue = 0;
}


//////////////////////////////////////////////////////////////////////////
CPLPropertyColor::~CPLPropertyColor()
{

}


//////////////////////////////////////////////////////////////////////////
bool CPLPropertyColor::GetRGB(BYTE& R, BYTE& G, BYTE& B){
	R = m_Red;
	G = m_Green;
	B = m_Blue;

	return true;
}


//////////////////////////////////////////////////////////////////////////
bool CPLPropertyColor::SetRGB(BYTE R, BYTE G, BYTE B){
	m_Red = R;
	m_Green = G;
	m_Blue = B;

	return true;
}


//////////////////////////////////////////////////////////////////////////
bool CPLPropertyColor::DisplayValue(CDC* dc, CRect Rect)
{
	CBrush brush;
	brush.CreateSolidBrush(RGB(m_Red, m_Green, m_Blue));
	dc->FillRect(Rect, &brush);
	brush.DeleteObject();
	
	COLORREF OrigTextColor;
	COLORREF OrigBackColor;
/*
	double cmax = max(m_Red, max(m_Green, m_Blue));
	double cmin = min(m_Red, min(m_Green, m_Blue));   
	double luminosity=(cmax+cmin)/2.0;   

	if(luminosity<128)
		OrigTextColor = dc->SetTextColor(RGB(255,255,255));
	else
		OrigTextColor = dc->SetTextColor(RGB(0,0,0));
*/

	if((m_Red+m_Green+m_Blue)/3 < 128)
		OrigTextColor = dc->SetTextColor(RGB(255,255,255));
	else
		OrigTextColor = dc->SetTextColor(RGB(0,0,0));

	OrigBackColor = dc->SetBkColor(RGB(m_Red, m_Green, m_Blue));

	CString str;
	str.Format("%d,%d,%d", m_Red, m_Green, m_Blue);
	dc->DrawText(str, &Rect, DT_SINGLELINE|DT_VCENTER);

	dc->SetTextColor(OrigTextColor);
	dc->SetBkColor(OrigBackColor);

	return true;
}


//////////////////////////////////////////////////////////////////////////
bool CPLPropertyColor::OnButton(){
	if(!m_PropertyList) return false;

	CColorDialog ColorDialog(RGB(m_Red, m_Green, m_Blue), CC_FULLOPEN, m_PropertyList->GetParent());
	if(ColorDialog.DoModal() == IDOK){
		m_Red = GetRValue(ColorDialog.GetColor());
		m_Green = GetGValue(ColorDialog.GetColor());
		m_Blue = GetBValue(ColorDialog.GetColor());
		return true;
	}
	return false;
}
