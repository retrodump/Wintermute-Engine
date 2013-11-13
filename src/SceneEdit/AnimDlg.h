#pragma once
#include "afxwin.h"


// CAnimDlg dialog

class CAnimDlg : public CDialog
{
	DECLARE_DYNAMIC(CAnimDlg)

public:
	CAnimDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAnimDlg();

// Dialog Data
	enum { IDD = IDD_ANIMATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_AnimList;
	CAdActor3DX* m_Actor;
	virtual BOOL OnInitDialog();

private:
	void SetState();
public:
	CButton m_BtnOK;
	afx_msg void OnAnimSelChanged();
protected:
	virtual void OnOK();
public:
	afx_msg void OnAnimSelDblClick();
};
