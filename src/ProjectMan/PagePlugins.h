#pragma once
#include "afxwin.h"
#include "../engine_core/wme_base/dcgf.h"

// CPagePlugins dialog

class CPagePlugins : public CPropertyPage
{
	DECLARE_DYNAMIC(CPagePlugins)

public:
	CPagePlugins();
	virtual ~CPagePlugins();

// Dialog Data
	enum { IDD = IDD_PAGE_PLUGINS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CBGame* m_Game;
	CStringArray m_SelectedPlugins;
	CCheckListBox m_PluginList;
	CEdit m_PluginInfo;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnLbnSelchangePluginList();
	afx_msg void OnLbnDblclkPluginList();
};
