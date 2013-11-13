#pragma once
#include "afxwin.h"


// CGuidDlg dialog

class CGuidDlg : public CDialog
{
	DECLARE_DYNAMIC(CGuidDlg)

public:
	CGuidDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGuidDlg();

// Dialog Data
	enum { IDD = IDD_GUID };

	GUID m_Guid;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EditGuid;
	CButton m_BtnGenerate;
	CButton m_BtnOK;
	virtual BOOL OnInitDialog();
private:
	void DisplayGuid();
	GUID GetGuid();
	void SetState();
public:
	afx_msg void OnGenerate();
	afx_msg void OnGuidChange();
protected:
	virtual void OnOK();
};
