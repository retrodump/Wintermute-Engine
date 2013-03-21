#pragma once
#include "..\mfcext\statlink.h"
#include "afxwin.h"


// CReminderDlg dialog

class CReminderDlg : public CDialog
{
	DECLARE_DYNAMIC(CReminderDlg)

public:
	CReminderDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CReminderDlg();

// Dialog Data
	enum { IDD = IDD_REMINDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_SelectedOption;
	CStaticLink m_DonationPage;
	virtual BOOL OnInitDialog();
	CButton m_BtnRemindLater;
	CButton m_BtnAlreadyDonated;
	CButton m_BtnDontWannaDonate;
protected:
	virtual void OnOK();
};
