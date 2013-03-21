#if !defined(AFX_PROPLIST_H__14FEB075_EE74_4D1B_A617_B32E34485929__INCLUDED_)
#define AFX_PROPLIST_H__14FEB075_EE74_4D1B_A617_B32E34485929__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropList.h : header file
//

#include "BtnST.h"
#include <afxtempl.h>

class CPLProperty;
class CPLCategory;
class CPLPropertyCombo;
class CPLPropertyEdit;
class CPLPropertyEditEx;
class CPLPropertyColor;


typedef void (*PROPERTY_CHANGED_CALLBACK)(CPLProperty* Prop, CPLCategory* Cat, void* Data);
typedef void (*PROPERTY_FOCUS_CALLBACK)(CPLProperty* Prop, CPLCategory* Cat, void* Data);
typedef bool (*BUTTON_PRESS_CALLBACK)(CButton* Button, CPLPropertyEditEx* Prop, CPLCategory* Cat, void* Data);

/////////////////////////////////////////////////////////////////////////////
// CPropList window

class CPropList : public CListBox
{
// Construction
public:
	CPropList();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropList)
	public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	HRESULT Redraw(bool ForceAll=true);
	HRESULT Cleanup();
	CPLProperty* m_WidgProp;
	CWnd* m_FocusedWidget;
	int m_MaxWidth;
	bool m_ControlsInitialized;
	CButtonST m_Button;
	CComboBox m_Combo;
	CEdit m_Edit;
	bool m_ShowCategories;
	void DoCollapseExpand(int Index);
	bool AddProperty(CPLCategory* Category, CPLProperty* Property);
	CPLCategory* AddCategory(CString ID, CString Name="");
	HRESULT ShowCategory(CString ID, bool Show=true, bool RedrawNow=true);
	HRESULT ShowProperty(CString ID, CString CatID="", bool Show=true, bool RedrawNow=true);
	void Populate(bool Force=false);
	int m_SelectedItem;
	CPLProperty* GetProperty(int Index);
	CPLProperty* GetProperty(CString ID, CString CategoryID="");
	CPLPropertyEdit* GetPropertyEdit(CString ID, CString CategoryID="");
	CPLPropertyCombo* GetPropertyCombo(CString ID, CString CategoryID="");
	CPLPropertyColor* GetPropertyColor(CString ID, CString CategoryID="");
	virtual ~CPropList();
	void GetItemValueRect(CRect& rect);
	void ResetContent();
	CArray<CPLCategory*, CPLCategory*> m_Categories;
	PROPERTY_CHANGED_CALLBACK m_PropChangeCallback;
	void* m_CallbackData;

	PROPERTY_FOCUS_CALLBACK m_PropFocusCallback;
	void* m_FocusCallbackData;
	
	HRESULT SetPropChangeCallback(PROPERTY_CHANGED_CALLBACK lpCallback, void* Data);
	HRESULT SetPropFocusCallback(PROPERTY_FOCUS_CALLBACK lpCallback, void* Data);
	bool m_ValidEdit;

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropList)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnButton();
	afx_msg void OnSelchangeCombo();
	afx_msg void OnKillfocusEdit();
	afx_msg void OnSetfocusEdit();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSelChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////


class CPLProperty  
{
public:
	CString m_HintFile;
	virtual bool DisplayValue(CDC* dc, CRect Rect);
	virtual CString GetText();
	void SetName(CString Name);
	void SetValue(CString Value);
	virtual CString GetValue();
	CString GetName();
	CString GetID();
	CString m_Name;
	CString m_ID;
	CString m_Value;
	CPLCategory* m_Category;
	CPropList* m_PropertyList;
	CPLProperty(CString ID, CString Name="");
	virtual ~CPLProperty();
	virtual bool OnButton();
	bool IsHidden();
	bool m_Hidden;
	CString m_HelpFile;

	typedef enum { PROP_UNKNOWN, PROP_CATEGORY, PROP_EDIT, PROP_EDIT_EX, PROP_COMBO, PROP_FILE, PROP_COLOR } TPropertyType;
	TPropertyType m_Type;
};


//////////////////////////////////////////////////////////////////////////
class CPLPropertyEdit : public CPLProperty  
{
public:
	bool SetValueValidate(CString Value, CString& ErrorString);
	typedef enum { EDIT_ANY, EDIT_UNSIGNED, EDIT_SIGNED } TPropertyEditType;
	TPropertyEditType m_EditType;
	bool m_CheckRange;
	int m_MinValue;
	int m_MaxValue;
	void SetValidation(TPropertyEditType Type, bool CheckRange=false, int MinValue=0, int MaxValue=0);
	CPLPropertyEdit(CString ID, CString Name="");
	virtual ~CPLPropertyEdit();
};


//////////////////////////////////////////////////////////////////////////
class CPLCategory : public CPLProperty  
{
public:
	bool m_Expanded;
	bool ChildrenVisible();
	virtual CString GetText(){ return ""; };
	CPLCategory(CString ID, CString Name="");
	virtual ~CPLCategory();
	CArray<CPLProperty*, CPLProperty*> m_Properties;
};


//////////////////////////////////////////////////////////////////////////
class CPLPropertyCombo : public CPLProperty  
{
public:
	class TComboValue{
	public:
		TComboValue(CString Label, CString Value=""){
			m_Label = Label;
			m_Value = Value==""?Label:Value;
		};
		CString m_Label;
		CString m_Value;
	};

	CPLPropertyCombo(CString ID, CString Name="");
	virtual ~CPLPropertyCombo();
	CArray<TComboValue*, TComboValue*> m_Values;
	virtual CString GetText();
	virtual CString GetValue();
	int GetValueInt();
	bool SetValue(CString Value);
	bool AddValue(CString Label, CString Value="");
	bool SetupBool(bool Init=false);
	bool PopulateCombo(CComboBox* Combo);
	int m_CurrentValue;
};


//////////////////////////////////////////////////////////////////////////
class CPLPropertyColor : public CPLProperty  
{
public:
	BYTE m_Red;
	BYTE m_Green;
	BYTE m_Blue;
	CPLPropertyColor(CString ID, CString Name="");
	bool GetRGB(BYTE& R, BYTE& G, BYTE& B);
	bool SetRGB(BYTE R, BYTE G, BYTE B);
	virtual ~CPLPropertyColor();
	virtual bool DisplayValue(CDC* dc, CRect Rect);
	virtual bool OnButton();
};


//////////////////////////////////////////////////////////////////////////
class CPLPropertyEditEx : public CPLPropertyEdit
{
public:	
	bool m_ReadOnly;
	CPLPropertyEditEx(CString ID, CString Name="");
	void SetReadOnly(bool ReadOnly=true) { m_ReadOnly = ReadOnly; };
	virtual ~CPLPropertyEditEx();
	BUTTON_PRESS_CALLBACK m_ButtonPressCallback;
	void* m_CallbackData;
	HRESULT SetButtonPressCallback(BUTTON_PRESS_CALLBACK lpCallback, void* Data);
	virtual bool OnButton();
};


//////////////////////////////////////////////////////////////////////////
class CPLPropertyFile : public CPLPropertyEditEx
{
public:
	CPLPropertyFile(CString ID, CString Name="");
	virtual ~CPLPropertyFile();
	virtual bool OnButton();
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPLIST_H__14FEB075_EE74_4D1B_A617_B32E34485929__INCLUDED_)
