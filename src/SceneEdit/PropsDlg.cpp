// PropsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SceneEdit.h"
#include "PropsDlg.h"
#include "NewPropDlg.h"
#include "../mfc_shared/utils_mfc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropsDlg dialog


//////////////////////////////////////////////////////////////////////////
CPropsDlg::CPropsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPropsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


//////////////////////////////////////////////////////////////////////////
CPropsDlg::~CPropsDlg()
{
	DeleteProps();
}


//////////////////////////////////////////////////////////////////////////
void CPropsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropsDlg)
	DDX_Control(pDX, IDC_REMOVE_PROP, m_RemoveProp);
	DDX_Control(pDX, IDC_PROP_LIST, m_PropList);
	DDX_Control(pDX, IDC_EDIT_PROP, m_EditProp);
	DDX_Control(pDX, IDC_ADD_PROP, m_AddProp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropsDlg, CDialog)
	//{{AFX_MSG_MAP(CPropsDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PROP_LIST, OnItemchangedPropList)
	ON_NOTIFY(NM_DBLCLK, IDC_PROP_LIST, OnDblclkPropList)
	ON_BN_CLICKED(IDC_ADD_PROP, OnAddProp)
	ON_BN_CLICKED(IDC_EDIT_PROP, OnEditProp)
	ON_BN_CLICKED(IDC_REMOVE_PROP, OnRemoveProp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropsDlg message handlers


//////////////////////////////////////////////////////////////////////////
BOOL CPropsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetupButton(&m_AddProp,        "Add property",        IDB_BCGBARRES_NEW);
	SetupButton(&m_EditProp,       "Edit property",       IDB_PROPERTIES);
	SetupButton(&m_RemoveProp,     "Remove property",     IDB_BCGBARRES_DELETE);

	m_PropList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_PropList.InsertColumn(0,"Property", LVCFMT_LEFT, 100);
	m_PropList.InsertColumn(1,"Value", LVCFMT_LEFT, 195);

	DeleteProps();

	if(m_Object){
		if(m_Object->m_ScProp){
			m_Object->m_ScProp->m_ValIter = m_Object->m_ScProp->m_ValObject.begin();
			while(m_Object->m_ScProp->m_ValIter!=m_Object->m_ScProp->m_ValObject.end()){
				CPropPair* prop = new CPropPair;
				prop->m_Name = CString(m_Object->m_ScProp->m_ValIter->first.c_str());
				prop->m_Value = CString(m_Object->m_ScProp->m_ValIter->second->GetString());

				m_Properties.Add(prop);

				m_Object->m_ScProp->m_ValIter++;
			}
		}
		if(m_Object->m_Name) SetWindowText(CString("Properties of the \"" + CString(m_Object->m_Name) + "\" object"));
	}

	Redraw();


	return TRUE;
}




//////////////////////////////////////////////////////////////////////////
void CPropsDlg::SetState(){

	int sel = GetCurSel();
	m_AddProp.EnableWindow(TRUE);
	m_EditProp.EnableWindow(sel>=0);
	m_RemoveProp.EnableWindow(sel>=0);
}



//////////////////////////////////////////////////////////////////////////
void CPropsDlg::Redraw(int sel){

	m_PropList.DeleteAllItems();

	for(int i=0; i<m_Properties.GetSize(); i++){
		m_PropList.InsertItem(i, m_Properties[i]->m_Name);
		m_PropList.SetItemText(i, 1, m_Properties[i]->m_Value);	
	}

	if(sel<0) sel = 0;

	if(sel>=m_Properties.GetSize()) sel = m_Properties.GetSize() - 1;
	if(sel>=0){
		m_PropList.SetItemState(sel, LVIS_SELECTED, LVIS_SELECTED);
		m_PropList.EnsureVisible(sel, FALSE);
	}

	SetState();
}


//////////////////////////////////////////////////////////////////////////
void CPropsDlg::OnOK() 
{
	if(!m_View || !m_Object) return;

	m_View->SetUndoState("Change properties");

	if(m_Object->m_ScProp) m_Object->m_ScProp->DeleteProps();
	CScValue* val = ::new CScValue(m_Object->Game);
	for(int i=0; i<m_Properties.GetSize(); i++){
		val->SetString((char*)LPCSTR(m_Properties[i]->m_Value));
		m_Object->ScSetProperty((char*)LPCSTR(m_Properties[i]->m_Name), val);
	}
	delete val;
	
	CDialog::OnOK();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CPropsDlg::DeleteProps()
{
	for(int i=0; i<m_Properties.GetSize(); i++) delete m_Properties[i];
	m_Properties.RemoveAll();

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
int CPropsDlg::GetCurSel()
{
	POSITION pos = m_PropList.GetFirstSelectedItemPosition();
	if(pos == NULL) return -1;
	else return m_PropList.GetNextSelectedItem(pos);
}


//////////////////////////////////////////////////////////////////////////
void CPropsDlg::OnItemchangedPropList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;	
	*pResult = 0;

	SetState();
}


//////////////////////////////////////////////////////////////////////////
void CPropsDlg::OnDblclkPropList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnEditProp();
	
	*pResult = 0;
}


//////////////////////////////////////////////////////////////////////////
void CPropsDlg::OnAddProp() 
{
	int sel = GetCurSel();
	CNewPropDlg dlg;
	while(true){
		if(dlg.DoModal()!=IDOK) break;
		bool Error = false;
		for(int i=0; i<m_Properties.GetSize(); i++){
			if(m_Properties[i]->m_Name==dlg.m_PropName){
				MessageBox("Property with this name already exists.", NULL, MB_OK|MB_ICONERROR);
				Error = true;
				break;
			}
		}
		if(!Error){
			CPropPair* prop = new CPropPair;
			prop->m_Name = dlg.m_PropName;
			prop->m_Value = dlg.m_PropValue;

			m_Properties.Add(prop);
			Redraw(m_Properties.GetSize()-1);

			break;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CPropsDlg::OnEditProp() 
{
	int sel = GetCurSel();
	if(sel<0) return;
	
	CNewPropDlg dlg;
	dlg.m_PropName = m_Properties[sel]->m_Name;
	dlg.m_PropValue = m_Properties[sel]->m_Value;
	
	while(true){
		if(dlg.DoModal()!=IDOK) break;
		bool Error = false;
		for(int i=0; i<m_Properties.GetSize(); i++){
			if(m_Properties[i]->m_Name==dlg.m_PropName && i!=sel){
				MessageBox("Property with this name already exists.", NULL, MB_OK|MB_ICONERROR);
				Error = true;
				break;
			}
		}
		if(!Error){
			m_Properties[sel]->m_Name = dlg.m_PropName;
			m_Properties[sel]->m_Value = dlg.m_PropValue;
			Redraw(sel);

			break;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CPropsDlg::OnRemoveProp() 
{
	int sel = GetCurSel();
	if(sel<0) return;

	if(MessageBox("Do you really want to remove this property?", NULL, MB_ICONQUESTION|MB_YESNO)==IDYES){
		delete m_Properties[sel];
		m_Properties.RemoveAt(sel);
		Redraw(sel);
	}		
}
